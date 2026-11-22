#!/usr/bin/env python3
"""Write an original placeholder sample (two-note chime, 22050 Hz mono PCM16)."""
import math, struct, sys, wave
rate = 22050
out = []
for f, dur in ((880.0, 0.18), (1318.5, 0.32)):
    n = int(rate * dur)
    for i in range(n):
        env = math.exp(-4.0 * i / n)
        out.append(int(12000 * env * math.sin(2 * math.pi * f * i / rate)))
with wave.open(sys.argv[1], "wb") as w:
    w.setnchannels(1); w.setsampwidth(2); w.setframerate(rate)
    w.writeframes(b"".join(struct.pack("<h", s) for s in out))
