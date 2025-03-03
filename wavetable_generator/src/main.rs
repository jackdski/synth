use std::f32::consts::PI;
use std::fs::File;
use std::io::Write;

use crate::buffer_prototype::run_buffer;

mod buffer_prototype;

fn main() {
    // const SAMPLE_FREQUENCY: u32 = 88200;  // fs - 2 * 44.1kHz (CD quality)
    const MAX_AMPLITUDE: f32 = 1.0;
    const MIN_AMPLITUDE: f32 = -1.0;

    const WAVETABLE_START_VALUE: f32 = -PI;
    const WAVETABLE_NUM_SAMPLES: usize = 2048;
    const WAVETABLE_STEP_VALUE: f32 = (2.0 * PI) / WAVETABLE_NUM_SAMPLES as f32;

    const VALUES_PER_LINE: usize = 16;

    // SINE wavetable
    let mut sine_wavetable_file = File::create("sine_wavetable.txt").expect("failed");

    for i in 0..WAVETABLE_NUM_SAMPLES {
        if i % VALUES_PER_LINE == 0 {
            sine_wavetable_file
                .write(b"\n")
                .expect("Unable to write newline to file");
        }
        let n = WAVETABLE_START_VALUE + (i as f32 * WAVETABLE_STEP_VALUE);
        let new_val: f32 = n.sin();
        assert!(new_val <= MAX_AMPLITUDE && new_val >= MIN_AMPLITUDE);
        let num_str = format!("{}, ", new_val);
        sine_wavetable_file
            .write(num_str.as_bytes())
            .expect("Unable to write to file");
    }

    sine_wavetable_file
        .write(b"\n")
        .expect("Unable to write newline to file");

    // SAW wavetable
    let mut saw_wavetable_file = File::create("saw_wavetable.txt").expect("failed");
    for i in 0..WAVETABLE_NUM_SAMPLES {
        if i % VALUES_PER_LINE == 0 {
            saw_wavetable_file
                .write(b"\n")
                .expect("Unable to write newline");
        }
        let n: f32 = WAVETABLE_START_VALUE + (i as f32 * WAVETABLE_STEP_VALUE);
        let new_val: f32 = -1.0 * n / PI;
        assert!(new_val <= MAX_AMPLITUDE && new_val >= MIN_AMPLITUDE);
        let num_str: String = format!("{}, ", new_val);
        saw_wavetable_file
            .write(num_str.as_bytes())
            .expect("Unable to write to file");
    }
    saw_wavetable_file
        .write(b"\n")
        .expect("Unable to write newline to file");

    // SQUARE wavetable
    let mut square_wavetable_file = File::create("square_wavetable.txt").expect("failed");

    for i in 0..WAVETABLE_NUM_SAMPLES {
        if i % VALUES_PER_LINE == 0 {
            square_wavetable_file
                .write(b"\n")
                .expect("Unable to write newline to file");
        }
        let n: f32 = WAVETABLE_START_VALUE + (i as f32 * WAVETABLE_STEP_VALUE);
        let mut new_val: f32 = n.sin();
        assert!(new_val <= MAX_AMPLITUDE && new_val >= MIN_AMPLITUDE);

        if new_val.is_sign_positive() {
            new_val = 0.999999;
        } else {
            new_val = 0.0001;
        }

        let num_str = format!("{}, ", new_val);
        square_wavetable_file
            .write(num_str.as_bytes())
            .expect("Unable to write to file");
    }

    square_wavetable_file
        .write(b"\n")
        .expect("Unable to write newline to file");

    let _ = run_buffer();
}
