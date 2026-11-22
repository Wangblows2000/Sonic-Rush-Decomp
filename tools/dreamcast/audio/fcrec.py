#!/usr/bin/env python3
"""fcrec.py - record the audio output of a Dreamcast disc running in Flycast, and
analyse it (Sonic Rush DC port).

Flycast has no "record output to file" option, but its ALSA backend opens any
ALSA PCM name, and alsa-lib's predefined `file` PCM writes everything to a file
(slave = null, so no sound card is needed and nothing blocks):

  HOME=/root DISPLAY=:99 LIBGL_ALWAYS_SOFTWARE=1 \\
  /home/claude/dcwork/flycast/build/flycast \\
      -config config:Debug.SerialConsoleEnabled=yes \\
      -config audio:backend=alsa \\
      -config "alsa:device=file:/tmp/rec.raw,raw" \\
      disc.cdi > serial.log 2>&1

/tmp/rec.raw = signed 16-bit little-endian stereo at 44100 Hz (Flycast's AICA
output rate), in emulated time. With the null slave the emulator is not
throttled by audio, so it runs faster than real time (about 5x here): 30 s of
wall time gave 151 s of audio. Serial output (printf) goes to serial.log.

Usage:
  fcrec.py record DISC.cdi SECONDS OUT.raw [--log serial.log] [--display :93]
           [--hold Right] [--press s,x --every 2]                (Xvfb is started if needed;
           input = xdotool key events to this Flycast window; --press keys are sent every
           --every s of EMULATED time (from the recorded file size). Flycast keyboard:
           arrows = d-pad, x = A (jump), c = B, s = X (boost), d = Y, Return = Start)
  fcrec.py wav IN.raw OUT.wav                                   (adds a WAV header)
  fcrec.py onsets IN.raw [--thr 50]                             (10 ms RMS blocks, onset times in ms)
  fcrec.py compare IN.raw ONSET_MS REF.wav                      (envelope + spectrogram match of one
                                                                 sound against a reference render)
"""
import argparse
import os
import subprocess
import sys
import time
import wave

import numpy as np

FLYCAST = os.environ.get("FLYCAST", "/home/claude/dcwork/flycast/build/flycast")
RATE = 44100


def load_raw(p):
    return np.fromfile(p, np.int16).astype(np.float64).reshape(-1, 2)


def cmd_record(a):
    env = dict(os.environ, HOME=os.environ.get("FC_HOME", "/root"), DISPLAY=a.display, LIBGL_ALWAYS_SOFTWARE="1")
    if not os.path.exists("/tmp/.X11-unix/X" + a.display.lstrip(":")):
        subprocess.Popen(["Xvfb", a.display, "-screen", "0", "1024x768x24"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(1)
    out = os.path.abspath(a.out)
    if os.path.exists(out):
        os.unlink(out)
    with open(a.log, "w") as log:
        p = subprocess.Popen([FLYCAST, "-config", "config:Debug.SerialConsoleEnabled=yes", "-config", "audio:backend=alsa",
                              "-config", f"alsa:device=file:{out},raw", a.disc], stdout=log, stderr=subprocess.STDOUT, env=env)
        win, held, nxt, t0 = None, False, a.every, time.time()
        while time.time() - t0 < a.seconds:
            if (a.hold or a.press) and not win:
                r = subprocess.run(["xdotool", "search", "--pid", str(p.pid), "--name", "flycast"],
                                   capture_output=True, text=True, env=env)
                win = (r.stdout.split() or [None])[-1]
            emu = os.path.getsize(out) / 4 / RATE if os.path.exists(out) else 0.0
            if win and a.hold and not held and emu > 1:
                subprocess.run(["xdotool", "keydown", "--window", win, a.hold], env=env); held = True
            if win and a.press and emu >= nxt:
                for k in a.press.split(","):
                    subprocess.run(["xdotool", "keydown", "--window", win, k], env=env); time.sleep(0.03)
                    subprocess.run(["xdotool", "keyup", "--window", win, k], env=env)
                nxt += a.every
            time.sleep(0.02)
        p.terminate()
        try:
            p.wait(3)
        except subprocess.TimeoutExpired:
            p.kill()
    n = os.path.getsize(out) // 4 if os.path.exists(out) else 0
    print(f"{out}: {n} frames = {n / RATE:.1f} s of emulated audio; serial log {a.log}")


def cmd_wav(a):
    x = np.fromfile(a.inp, np.int16)
    with wave.open(a.out, "wb") as w:
        w.setnchannels(2); w.setsampwidth(2); w.setframerate(RATE); w.writeframes(x.tobytes())


def env10(x, block=441):
    n = len(x) // block
    return np.sqrt((x[:n * block].reshape(n, block) ** 2).mean(1))


def cmd_onsets(a):
    x = load_raw(a.inp).mean(1)
    e = env10(x)
    on, prev = [], False
    for i, v in enumerate(e):
        if v > a.thr and not prev:
            on.append(i * 10)
        prev = v > a.thr
    print(f"{len(x) / RATE:.1f} s, peak {np.abs(x).max():.0f}; onsets (ms): {on}")


def cmd_compare(a):
    from scipy.signal import resample_poly, stft
    x = load_raw(a.inp).mean(1)
    with wave.open(a.ref) as w:
        r = np.frombuffer(w.readframes(w.getnframes()), np.int16).astype(np.float64).reshape(-1, w.getnchannels()).mean(1)
        rr = w.getframerate()
    if rr != RATE:
        g = np.gcd(RATE, rr)
        r = resample_poly(r, RATE // g, rr // g)
    s0 = int((a.onset - 200) * RATE / 1000)
    seg = x[s0:s0 + len(r) + RATE]
    er, es = env10(r), env10(seg)
    best = max(range(0, 60), key=lambda L: np.corrcoef(es[L:L + len(er)], er)[0, 1] if L + len(er) <= len(es) else -1)
    y = seg[best * 441:best * 441 + len(r)]

    def spec(v):
        return np.log10(np.abs(stft(v, RATE, nperseg=1024, noverlap=1024 - 441)[2]) + 1e-3)
    Sr, Sy = spec(r), spec(y)
    n = min(Sr.shape[1], Sy.shape[1])
    m = Sr[:, :n] > Sr.max() - 3
    sc = np.corrcoef(Sr[:, :n][m], Sy[:, :n][m])[0, 1]
    ec = np.corrcoef(es[best:best + len(er)], er)[0, 1]
    db = lambda v: np.round(20 * np.log10(v / er.max() + 1e-9)).astype(int)  # noqa: E731
    print(f"offset {best * 10 - 200:+d} ms  envelope corr {ec:.3f}  spectrogram corr {sc:.3f}  "
          f"peak level rec/ref {20 * np.log10(es.max() / er.max()):+.1f} dB")
    print("ref dB/50ms", db(env10(r, 2205))[:30])
    print("rec dB/50ms", db(env10(y, 2205))[:30])


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest="cmd", required=True)
    p = sp.add_parser("record"); p.add_argument("disc"); p.add_argument("seconds", type=float); p.add_argument("out")
    p.add_argument("--log", default="/tmp/fcrec_serial.log"); p.add_argument("--display", default=":99")
    p.add_argument("--hold", default=""); p.add_argument("--press", default=""); p.add_argument("--every", type=float, default=2.0)
    p.set_defaults(f=cmd_record)
    p = sp.add_parser("wav"); p.add_argument("inp"); p.add_argument("out"); p.set_defaults(f=cmd_wav)
    p = sp.add_parser("onsets"); p.add_argument("inp"); p.add_argument("--thr", type=float, default=50); p.set_defaults(f=cmd_onsets)
    p = sp.add_parser("compare"); p.add_argument("inp"); p.add_argument("onset", type=float); p.add_argument("ref")
    p.set_defaults(f=cmd_compare)
    a = ap.parse_args()
    a.f(a)
    return 0


if __name__ == "__main__":
    sys.exit(main())
