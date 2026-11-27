# Dreamcast port — testing and latest results

All results are from Flycast (built from source, HLE BIOS, run headless under
Xvfb with software GL). Nothing has been verified on real Dreamcast hardware.
Emulated frame times are guest times from the frame profiler, not host speed.

## How the port is tested

1. **Host tests** (BUILD.md §3): the translated modules run on the PC against
   the converted data, with checks taken from the original code's behaviour.
2. **Flycast runs** of test discs (BUILD.md §4). Input is injected with
   `xdotool` (default Flycast keyboard: arrows = pad, X = A, C = B, S = X,
   D = Y, Return = Start); results come from the serial console
   (`Debug.SerialConsoleEnabled=yes`) and screenshots.
3. **Autopilot story runs**: `SRDC_AUTOMAP=s|b|x` plus `SRDC_AUTOCLEAR`
   play whole stories through the real menus, zone map, cutscenes and saves.
   In boss acts the boss starts attacking at "GO!", so a long autoclear delay
   loses lives; use `SRDC_AUTOCLEAR=200` for full-story runs.
4. **Frame pacing**: `/cd/debug/frameprof.txt` reports frames per vblank and
   the worst frame per second and per scene.

### Flycast save states

A cold boot of a test disc reaches the act in about 30 s; loading a state
takes about 1 s. The Flycast build used has states but no rewind and no Lua.

- State files: `<SavestatePath>/srdc.state` (slot 0) or `srdc_<n>.state`.
  Give each tester its own `HOME` or
  `-config config:Dreamcast.SavestatePath=<dir>`.
- Load on boot: `-config config:Dreamcast.SavestateSlot=<n> -config
  config:Dreamcast.AutoLoadState=yes`.
- Hotkeys are bound in `$HOME/.config/flycast/mappings/SDL_Keyboard.cfg`,
  section `[digital]`: `btn_quick_save` (F5, scancode 62), `btn_jump_state`
  (F8, 65), `btn_next_slot` / `btn_prev_slot`; keep the default pad bindings
  in the same file.
- A state holds all of RAM, code included: it is valid only for the disc
  build that saved it.

## Latest results (2026-11-23)

| Area | Result |
| --- | --- |
| Boot and front end | SEGA logo → title intro → title → menus → player select → zone map (character model, YES/NO box, talks) → stage; options pages and slides; records; sound test |
| Sonic story | full run, new save: every act, boss, cutscene, map talk, ending, staff roll and unlock notice; no crash; no model load after a stage's first frame |
| Blaze story | full run (39 min of play): zones in Blaze's order, talks, cutscenes, ending, staff roll; progress kept after a reboot |
| Extra story | full run (20 min): script, Extra boss, ExtraAct, ending, staff roll 2; extra-cleared flag kept after a reboot; START pause in the Extra boss (CONTINUE / GO BACK) |
| Act death restart | a death in an act remakes its objects and rings from the level's lists, as the DS StageRestart does (`objects_stage_restart`). Before, the kept objects ran on the old stage clock after the timer reset: in Altitude Limit act 2 (Flycast, after a death at the cloud-lift pit past the second marker) the periodic platform spawners (0xaf) made no platform for as long as the act had run before the death, and destroyed enemies and taken rings stayed gone. Now the lifts, enemies and rings come back: after the same death the recorded line was followed past the pit to x 27000 with no new desync (the old build died again there), and the goal ring ends the act |
| Bosses | every boss (1–7, final, Extra) cleared as Sonic and as Blaze; uncapped Sonic fights showed every attack phase; mid-fight talks freeze the fight; a death restarts the fight at full boss health, with a new player at the start and the arena's rings and objects made again (StageRestart_Create: an idle player in each of the 7 story boss arenas died, restarted 2-3 times and reached the game over in Flycast; before, a crush could leave the player in the floor at every respawn) |
| Special stages | entry by boosting at the special ring, clear and fail paths, emerald saved; controller scheme (Trick Spring buttons, A attack, START prompt) |
| Water Palace | above / below the water line with the tinted palettes, bubbles, drown countdown and jingle, music restart after the respawn (Sonic and Blaze) |
| Time attack, game over, continue | reached and completed in Flycast |
| Frame rate | every act 59.86–60 fps; bosses 598 frames in 598 vblanks; special stage 59.93 fps; no 3-vblank goal frame after the act-clear prefetch |
| Memory | VRAM and sound RAM flat over a 95-minute session; heap at stage starts unchanged from loop 2 to loop 3 of the story |
| Kit | clean unpack + `make_disc.sh` with the owner's ROM: disc built in 2 min 46 s, boots to a playable first act |

Open items and accepted differences: KNOWN-ISSUES.md.
