# Dreamcast port — camera and presentation

Status: design only. Nothing here is implemented yet.

- World and physics stay in DS coordinates. The game already tracks one camera
  position (`Camera_*`, ov000) that both DS screens derive from; the top screen
  shows the area above the bottom screen. The DC compositor renders the
  combined 256x384 world strip through one continuous viewport that follows
  that camera, never switching between top/bottom states.
- Framing: keep DS player scale. 640x480 at 2.5x gives a 256x192 world window —
  exactly one DS screen — so the view shows about one screen's worth of world,
  centred on the player with smoothed vertical tracking instead of the DS
  screen split.
- Look-ahead and damping are applied in the compositor only (render
  transform), never to game state.
- Cutscenes: relevance from `TalkDemo` script state (speaker, action targets)
  rather than screen position.
- HUD: rebuilt in the 4:3 safe area (`UI/Hud` data, new layout).
