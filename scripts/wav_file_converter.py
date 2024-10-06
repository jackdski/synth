import scipy.io.wavfile as wavfile
import numpy as np
import os
import sys

SAMPLES_PER_SEC = 44100
WAVETABLE_NUM_SAMPLES = 2048
MAX_AMPLITUDE = 1.0
MIN_AMPLITUDE = -1.0
VALUES_PER_LINE = 16

# WAVETABLE_START_VALUE = scipy.pi
# WAVETABLE_STEP_VALUE = (2.0 * PI) / WAVETABLE_NUM_SAMPLES

def convert_wavefile(file: str):
    samplerate, data = wavfile.read(file)
    data = data.astype(np.int16)

    wavetable_output_dir = f"{sys.path[0]}/wavetables"
    wavetable_file = f"{wavetable_output_dir}/{file.split('/')[-1].replace('.wav', '.txt')}"
    print(f" Writing wavetable: {wavetable_file}")
    print(f"\tSample Rate: {samplerate : >15}")
    print(f"\tLength: {len(data) : >20}")

    time_seconds = "{:.3f}".format(len(data) / samplerate)
    print(f"\tTime: {time_seconds : >21}s")

    f = open(wavetable_file, "w")
    for index, value in enumerate(data):
        val = 0
        if value[0] == 0.0:
            val = "0.0f"
        elif value[0] > 0:
            val = f"{value[0] / np.iinfo(np.int16).max}f"
        else:
            val = f"{value[0] / np.iinfo(np.int16).min}f"

        f.write(f"\t{val},\n")
        # if index % VALUES_PER_LINE == 0 and index > 0:
            # f.write("\n")
    f.close()


if __name__ == "__main__":
    wavefile_directory = os.path.expanduser("~/Downloads/wavfiles/")

    # directory = os.fsencode(wavefile_directory)
    for file in os.listdir(wavefile_directory):
        filename = os.fsdecode(file)
        if filename.endswith(".wav"):
            filename = os.path.join(wavefile_directory, filename)
            convert_wavefile(file=filename)
        else:
            continue
