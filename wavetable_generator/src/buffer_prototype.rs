use std::f32::consts::{PI, E};
// use std::collections::VecDeque;
use plotters::prelude::*;
use std::clone::Clone;
use core::marker::Copy;

// const MAX_AMPLITUDE: f32 = 1.0;
// const MIN_AMPLITUDE: f32 = -1.0;

const WAVETABLE_START_VALUE: f32 = -PI;
const WAVETABLE_NUM_SAMPLES: usize = 2048;
const WAVETABLE_STEP_VALUE: f32 = (2.0 * PI) / WAVETABLE_NUM_SAMPLES as f32;

// const BUFFER_SIZE: usize = 100;
const SAMPLE_FREQUENCY: usize = 44100;

const LOW_PASS_FILTER_FREQUENCY: usize = 400 as usize;
const ADSR_STEP_FACTOR: f32 = 1e-4;

const OUT_FILE_NAME: &'static str = "sample.png";

// #[derive(Copy, Clone)]
// enum BufferChannel
// {
//     BufferA,
//     BufferB,
// }

#[derive(Copy, Clone, Debug)]
enum AdsrMode {
    Linear,
    Asymptotic,
}

#[derive(Clone, Copy, Debug)]
enum AdsrStage {
    Attack,
    Decay,
    Sustain,
    Release,
}

#[derive(Copy, Debug)]
struct ADSR {
    mode: AdsrMode,
    stage: AdsrStage,
    amplitude: f32,
    attack: f32,
    decay: f32,
    sustain: f32,
    release: f32,
}

#[derive(Clone)]
struct LPF {
    sampling_frequency: f32,
    filter_frequency: f32,
    old_value: f32,
}

struct Note {
    initialized: bool,
    frequency: f32,
    volume: f32,
    wavetable_index: usize,
    wavetable_steps: usize,
    wavetable: [f32; WAVETABLE_NUM_SAMPLES],
    envelope: ADSR,
    lpf: LPF,
}

impl Clone for ADSR {
    fn clone(&self) -> ADSR {
        ADSR {
            mode: self.mode,
            stage: self.stage,
            amplitude: self.amplitude,
            attack: self.attack,
            decay: self.decay,
            sustain: self.sustain,
            release: self.release,
        }
    }
}

impl Clone for Note {
    fn clone(&self) -> Note {
        Note {
            initialized: self.initialized,
            frequency: self.frequency,
            volume: self.volume,
            wavetable_index: self.wavetable_index,
            wavetable_steps: self.wavetable_steps,
            wavetable: self.wavetable,
            envelope: self.envelope.clone(),
            lpf: self.lpf.clone(),
        }
    }
}



struct Oscillator {
    notes: Vec<Note>,
}


impl ADSR {
    fn update_linear(&mut self) {
        match self.stage {
            AdsrStage::Attack  => {
                self.amplitude = self.amplitude + ADSR_STEP_FACTOR;
                if self.amplitude >= self.attack {
                    self.amplitude = self.attack;
                    self.stage = AdsrStage::Decay;
                }
            },
            AdsrStage::Decay => {
                self.amplitude = self.amplitude - ADSR_STEP_FACTOR;
                if self.amplitude <= self.decay {
                    self.amplitude = self.decay;
                    self.stage = AdsrStage::Sustain
                }
            },
            AdsrStage::Sustain => (), // TODO: noise generator?
            AdsrStage::Release => {
                self.amplitude = f32::max(self.amplitude - (ADSR_STEP_FACTOR * self.release), 0.0);
            },
        }
    }

    fn update_asymptotic(&mut self) {
        // y[n] = a y[n-1] + (1-a)target
        // a = e^(-T/tau)   T = sample period, tau = time constant
        let tau: f32 = 2.0e-1;
        let exponent: f32 = (-1.0 / SAMPLE_FREQUENCY as f32) / tau;
        let a: f32 = f32::powf(E, exponent);
        match self.stage {
            AdsrStage::Attack  => {
                let old_amplitude: f32 = self.amplitude;
                self.amplitude = a * old_amplitude + (1.0f32 - a) * self.attack
            },
            AdsrStage::Decay   => self.amplitude = a * self.amplitude + (-1.0f32 - a) * self.decay,
            AdsrStage::Sustain => (),
            AdsrStage::Release => self.amplitude = a * self.amplitude + (-1.0f32 - a) * self.release,
        };
    }

    fn update(&mut self) -> f32{
        match self.mode {
            AdsrMode::Linear => self.update_linear(),
            AdsrMode::Asymptotic => self.update_asymptotic(),
        };
        return self.amplitude;
    }
}

impl LPF {
    fn update(&mut self, new_value: f32) -> f32{
        /*
        * Filter:
        *  y_f(k) = (Ts / (Tf + Ts)) + y(k)
        *  smoothing factor = a = (1 / SAMPLE_FREQUENCY) / ((1 / LOW_PASS_FILTER_FREQUENCY) + (1 / SAMPLE_FREQUENCY))
        *  y = (1 - a) old_value + new_value * a
        */
        let smoothing_factor: f32 = (1.0 / self.sampling_frequency as f32 ) / ((1.0 / self.filter_frequency  as f32) + (1.0 / self.sampling_frequency as f32));
        let output: f32 = ((1.0 - smoothing_factor) * self.old_value) + (new_value * smoothing_factor);
        self.old_value = output;
        return output;
        // return new_value;
    }
}

impl Note {
    fn init(&mut self) {
        let number_of_wavelengths_per_second: usize = SAMPLE_FREQUENCY / self.frequency as usize;
        self.wavetable_steps = ((WAVETABLE_NUM_SAMPLES as f32 * number_of_wavelengths_per_second as f32) / SAMPLE_FREQUENCY as f32).round() as usize;
        self.initialized = true;
    }

    fn update(&mut self) -> f32 {
        if self.initialized == false {
            self.init();
        }

        let wavetable_value: f32 = self.wavetable[self.wavetable_index];
        let new_value: f32 = wavetable_value * self.envelope.update();
        let out_value: f32 = self.lpf.update(new_value);

        // update wavelength index, wrap around if needed
        self.wavetable_index = self.wavetable_index + self.wavetable_steps;
        if self.wavetable_index >= WAVETABLE_NUM_SAMPLES {
            self.wavetable_index = self.wavetable_index - WAVETABLE_NUM_SAMPLES;
        }

        return out_value * self.volume;
    }
}


// struct Buffer {
//     buffer_a: VecDeque<f32>,
//     buffer_b: VecDeque<f32>,
//     active_buffer_channel: BufferChannel,
// }


pub fn run_buffer() -> Result<(), Box<dyn std::error::Error>>{
    println!("Running buffer sim");

    // SINE wavetable
    let sine_wavetable: [f32; WAVETABLE_NUM_SAMPLES] = core::array::from_fn(|i| ((WAVETABLE_START_VALUE + (i as f32 * WAVETABLE_STEP_VALUE)).sin()));

    // let buffer_a: VecDeque<f32> = VecDeque::new();
    // let buffer_b: VecDeque<f32> = VecDeque::new();

    // let mut sine_buffer: Buffer = Buffer {
    //     buffer_a: buffer_a,
    //     buffer_b: buffer_b,
    //     active_buffer_channel: BufferChannel::BufferA,
    // };

    let osc_notes: Vec<Note> = Vec::new();

    let mut osc: Oscillator = Oscillator{
        notes: osc_notes,
    };

    const NUM_CYCLES: usize = 1;
    let mut waveform: Vec<f32> = Vec::new();

    let notes: Vec<f32> = vec![300.0, 600.0, 120.0, 30.0];
    // const NUM_NOTES: usize = 3;
    for note in notes
    {
        osc.notes.push(
            Note {
                initialized: false,
                // frequency: notes.pop().ok_or("Not f32")?,
                frequency: note,
                wavetable_index: 0usize,
                wavetable_steps: 0usize,
                wavetable: sine_wavetable,
                volume: 0.7f32,
                lpf: LPF {
                    sampling_frequency: SAMPLE_FREQUENCY as f32,
                    filter_frequency: LOW_PASS_FILTER_FREQUENCY as f32,
                    old_value: 0.0,
                },
                envelope: ADSR {
                    // mode: AdsrMode::Linear,
                    mode: AdsrMode::Asymptotic,
                    stage: AdsrStage::Attack,
                    amplitude: 0.0,
                    attack: 1.0,
                    decay: 1.0,
                    sustain: 1.0,
                    release: 1.0,
                },
            },
        );
    }

    while waveform.len() <= (SAMPLE_FREQUENCY * NUM_CYCLES)
    {
        let mut values: Vec<f32> = Vec::new();
        for i in 0..osc.notes.len() {
            let new_val: f32 = osc.notes[i].update();
            values.push(new_val);
        }

        let sum: f32 = values.iter().sum();
        let sample_value: f32 = sum / values.len() as f32;

        // get value from wavelength table
        // match sine_buffer.active_buffer_channel {
        //     BufferChannel::BufferA => sine_buffer.buffer_a.push_back(sample_value),
        //     BufferChannel::BufferB => sine_buffer.buffer_b.push_back(sample_value),
        // }

        // // copy buffer to waveform
        // let value: Option<f32> = match sine_buffer.active_buffer_channel {
        //     BufferChannel::BufferA => sine_buffer.buffer_a.pop_front(),
        //     BufferChannel::BufferB => sine_buffer.buffer_b.pop_front(),
        // };
        // if waveform.len() % 10 == 0 {
            // println!("{}", sample_value);
        // }
        waveform.push(sample_value);

        // match value {
        //     Some(value) => waveform.push(value),
        //     None => waveform.push(0.0),
        // }

        // update active buffer
        // match sine_buffer.active_buffer_channel {
        //     BufferChannel::BufferA => sine_buffer.active_buffer_channel = BufferChannel::BufferB,
        //     BufferChannel::BufferB => sine_buffer.active_buffer_channel = BufferChannel::BufferA,
        // }

        // if waveform.len() % (NUM_CYCLES * WAVETABLE_NUM_SAMPLES) == 0 {
        //     let new_note: Note = Note {
        //         initialized: false,
        //         frequency: notes.pop().ok_or("Not f32")?,
        //         wavetable_index: 0usize,
        //         wavetable_steps: 0usize,
        //         wavetable: sine_wavetable,
        //         volume: 0.7f32,
        //         lpf: LPF {
        //             sampling_frequency: SAMPLE_FREQUENCY as f32,
        //             filter_frequency: LOW_PASS_FILTER_FREQUENCY as f32,
        //             old_value: 0.0,
        //         },
        //         envelope: ADSR {
        //             mode: AdsrMode::Asymptotic,
        //             stage: AdsrStage::Attack,
        //             amplitude: 0.0,
        //             attack: 1.0,
        //             decay: 0.8,
        //             sustain: 1.0,
        //             release: 1.0,
        //         },
        //     };

        //     osc.notes.push(new_note);
        // }
    }

    println!("plotting");
    // plot
    let root = BitMapBackend::new(OUT_FILE_NAME, (1024, 768)).into_drawing_area();
    let _ = root.fill(&WHITE);

    let x_len: f32 = waveform.len() as f32;
    // let x_len: f32 = 50000.0;

    let mut chart = ChartBuilder::on(&root)
        .x_label_area_size(35)
        .y_label_area_size(40)
        .right_y_label_area_size(40)
        .margin(5)
        .caption("Waveform", ("sans-serif", 50.0).into_font())
        .build_cartesian_2d(0f32..x_len, -1.0f32..1.0f32)?;

    chart
        .configure_mesh()
        .draw()?;

    chart
        .draw_series(LineSeries::new(
            (0..waveform.len()).map(|x: usize| (x as f32, waveform[x])),
            &BLUE,
        ))?;

    // To avoid the IO failure being ignored silently, we manually call the present function
    root.present().expect("Unable to write result to file, please make sure 'plotters-doc-data' dir exists under current dir");
    println!("Result has been saved to {}", OUT_FILE_NAME);

    Ok(())
}
