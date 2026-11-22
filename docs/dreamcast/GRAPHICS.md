# Dreamcast port — graphics

Implemented (foundation): 640x480 PVR output via `pvr_init_defaults`, opaque and
translucent lists, flat and textured quads (ARGB4444 non-twiddled), alpha
blending, three-layer parallax test, texture upload with `pvr_mem_malloc`.

Not implemented: BG tile layers, OAM/BAC sprites, affine BGs, line scroll,
NSBMD meshes, fog, render-to-texture, VRAM budget manager.

Budget (planned): 8 MB VRAM − 2 framebuffers 640x480x16bpp (1.2 MB) − TA/OPB
buffers (~1–2 MB) ≈ 4.5–5 MB textures per scene. DS VRAM is 656 KB, so a scene's
converted textures fit if BG layers are kept paletted (4/8 bpp PVR formats).

3D content (bosses, special stage, title/demo models) comes from NSBMD in
`mod/`; polygon and vertex counts must be preserved (record counts per model in
the manifest when the converter exists).
