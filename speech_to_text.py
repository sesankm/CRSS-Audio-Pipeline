import speech_recognition as sr
from pydub import AudioSegment
from pydub import silence

r = sr.Recognizer()
# sound = AudioSegment.from_wav("./A12_T610_HR1U_CHCH10_NETWORK-CONTROLLER-L_ddd-hh-mm-ss_DDD-HH-MM-SS-0.wav")
sound = AudioSegment.from_wav("foo.wav")
chunks = silence.split_on_silence(sound, min_silence_len=500, silence_thresh=sound.dBFS-0, keep_silence=500)

for ind, chunk in enumerate(chunks):
    chunk.export("chunk-{}.wav".format(ind))

    with sr.AudioFile("chunk-{}.wav".format(ind)) as data:
        try:
            text = r.recognize_google(r.record(data))
            print(text)
        except sr.UnknownValueError as e:
            pass
