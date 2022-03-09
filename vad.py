import webrtcvad
from scipy.io import wavfile

class Frame(object):
    """Represents a "frame" of audio data."""
    def __init__(self, bytes, timestamp, duration):
        self.bytes = bytes
        self.timestamp = timestamp
        self.duration = duration

vad = webrtcvad.Vad()
vad.set_mode(1)

sample_rate, data = wavfile.read("A12_T610_HR1U_CHCH10_NETWORK-CONTROLLER-L_ddd-hh-mm-ss_DDD-HH-MM-SS-0.wav")

offset = 0;
time = 0;
n = int(sample_rate * (30 / 1000.0) * 2)
duration = (float(n) / sample_rate) / 2.0

for i in range(0, len(data), n):
    f = Frame(data[offset:offset + n], time, duration)
    print(vad.is_speech(f.bytes, sample_rate))
    offset += n
    time += duration
