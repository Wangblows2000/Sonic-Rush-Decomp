/* Windows build of the Vs CPU route tool (vs_route.exe): definitions for the weak references of
 * model/model.c (engine/render.c's view and user clip). PE/COFF static links do not resolve undefined
 * weak symbols to 0 as ELF does; the route simulation never draws, so these only stand in for the
 * default full-screen view (render_reset_view: 640 x 480 at 2.5x, no clip). Linked after
 * route/snap_end.c: outside the simulation snapshot. */
float g_view_x = 0.0f, g_view_y = 0.0f, g_view_w = 640.0f, g_view_h = 480.0f, g_view_scale = 2.5f;
int g_render_clip = 0;
