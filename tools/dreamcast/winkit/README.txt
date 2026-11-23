SONIC RUSH DC - DISC MAKER  (Windows 10 and 11)
================================================

This turns YOUR OWN copy of Sonic Rush into a Sega Dreamcast disc.

What you need
-------------
* Your Sonic Rush ROM file: the EUROPEAN version, a file ending in .nds.
  (This kit does not contain the game. It only works with your own copy.)
* About 4 GB of free space on your computer.
* About 5 minutes.


How to make the disc
--------------------
1. UNZIP FIRST.
   Right-click the zip file and choose "Extract All...", then "Extract".
   Tip: put the folder on your Desktop. (Not in "Program Files".)

2. Open the unzipped folder and double-click  SonicRushDC.exe
   If a blue box says "Windows protected your PC":
   click "More info", then click "Run anyway".

3. Click "Choose ROM..." and pick your Sonic Rush .nds file.
   (Or drag the .nds file onto the window.
    Or copy the .nds file into this folder before starting: it is picked
    automatically.)

4. Click the big button "Make my Dreamcast disc!" and wait.
   The bar fills up as it works. Do not close the window.

5. "All done!" - your disc is in the folder  "Your Dreamcast disc":
       SonicRushDC.cdi


How to play it
--------------
* On your computer: open SonicRushDC.cdi in the Flycast emulator
  (File > Open, or drag it onto Flycast).
* On a real Dreamcast: burn SonicRushDC.cdi to a CD-R with a program that
  can burn Dreamcast CDI images. (The port has been tested in Flycast only,
  not on a real Dreamcast yet.)

Controls: stick or D-pad = move, A = jump, B = trick, X = boost,
right / left trigger = the DS R / L buttons, START = pause.
In special stages the screen shows which Dreamcast button to press.


Something went wrong?
---------------------
* "This file is not a Sonic Rush ROM": pick the right .nds file.
* "...the American / Japanese version": you need the European (EUR) one.
* "cannot write into its own folder": move the whole folder to your Desktop.
* "Not enough free disk space": free up about 4 GB.
* Anything else: tick "Show details" in the window to see what happened.
  Making the disc again is safe; it reuses the work already done.


Please keep the disc for yourself
---------------------------------
SonicRushDC.cdi contains the game, made from your copy. Do not share it.

Inside the "app" folder are the tools that do the work (Python with numpy and
Pillow, dsd, cdi4dc, wav2adpcm and the Sonic Rush DC converters). Their licences
are in app\LICENSES.
