import numpy as np
import simpleaudio as sa
import matplotlib.pyplot as plt

FREQUENCY_HZ = 440
SAMPLES_PER_SEC = 44100
DURATION_S = 3

TWO_PI = 2 * np.pi

NOTES = {
    "A3": 220.0,
    "A#4": 233.08,
    "Bb4": 233.08,
    "B4": 246.94,
    "C4": 261.63,
    "C#4": 277.18,
    "Db4": 277.18,
    "D4": 293.66,
    "D#4": 311.13,
    "Eb4": 311.13,
    "E4": 329.63,
    "F4": 349.23,
    "F#4": 369.99,
    "Gb4": 369.99,
    "G4": 392.00,
    "G#4": 415.30,
    "Ab4": 415.30,
    "A5": 440.30,
}


samples = np.linspace(0, DURATION_S, DURATION_S * SAMPLES_PER_SEC, False)
x_axis = np.arange(0.0, DURATION_S * SAMPLES_PER_SEC, 1)


def get_waveforms_from_note(_note: str):
    return np.sin(NOTES.get(_note * samples * TWO_PI))


# note = np.sin(FREQUENCY_HZ * samples * TWO_PI)
notes = list()

note_sequence = [
    "C4",
    "E4",
]

# gather sin wave data
for note in note_sequence:
    notes.append(get_waveforms_from_note(note))

if len(notes) > 1:
    samples = np.add(*notes)

audio = note * (2 ** 15 - 1) / np.max(np.abs(note))
audio = audio.astype(np.int16)

fig, ax = plt.subplots()
ax.plot(x_axis, audio)

plt.show()

play_obj = sa.play_buffer(audio, 1, 2, SAMPLES_PER_SEC)

# play_obj.wait_done()
