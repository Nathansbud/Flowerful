from mutagen.id3 import ID3
import librosa
import matplotlib.pyplot as plt


path = '/Users/zackamiton/Torrents/SoulSeek Downloads/track.mp3'
print(ID3(path).get('TIT2'))

y, sr = librosa.load(path, duration=10)
onset_env = librosa.onset.onset_strength(y=y, sr=sr)
tempo = librosa.beat.tempo(onset_envelope=onset_env, sr=sr)
print(tempo)
