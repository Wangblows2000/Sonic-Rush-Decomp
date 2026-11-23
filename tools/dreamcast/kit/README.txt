Sonic Rush DC kit — native Dreamcast port, built from YOUR ROM
==============================================================

This kit contains the Dreamcast program and the conversion tools only. It has
no Sonic Rush data. The disc is made on your PC from your own ROM dump.

Windows 10/11
  1. Install WSL once: open PowerShell as administrator, run  wsl --install
     and restart. Open "Ubuntu" from the Start menu and create a user.
  2. In Ubuntu, unzip the kit somewhere, e.g.
         cd ~ && unzip /mnt/c/Users/<you>/Downloads/srdc_kit.zip && cd srdc_kit
  3. Make the disc from your ROM (path as seen from Ubuntu):
         sh make_disc.sh /mnt/c/Users/<you>/Downloads/sonic_rush.nds
     It asks for your Ubuntu password once to install packages, then converts
     everything (a few minutes) and writes  out/srdc.cdi
  4. Copy it to Windows:  cp out/srdc.cdi /mnt/c/Users/<you>/Desktop/
     Run it in Flycast. It has been tested in Flycast only, not on a real
     Dreamcast.

Supported ROM: Sonic Rush (Europe), SHA-1 52e5024efcf7617a820fa23789b37e5168c3c4fc.

Controls: analog/D-pad move, A jump, B trick, X boost, right/left trigger =
DS R/L (tricks, air actions), Start pause. Pick Sonic or Blaze on the player
select screen. Special stages show the Dreamcast button to press for each
Trick Spring step.

State of the port: see docs/dreamcast/KNOWN-ISSUES.md in the kit.
Third-party tools: thirdparty/ (img4dc/cdi4dc: GPL, source included;
wav2adpcm from KallistiOS: see its header), bin/dsd (ds-decomp, licence in
thirdparty/ds-decomp-LICENSE).
