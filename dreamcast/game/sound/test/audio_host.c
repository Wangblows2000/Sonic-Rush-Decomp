/*
 * Host stand-in for engine/audio.c (music streams / RAM jingles) and the
 * player / objects globals the sound module reads, for the host tests.
 * Records every call in a log and keeps the state the tests check.
 */
#include "audio_host.h"
#include "../../player/dcp_player.h"
#include "../../objects/objects.h"
#include <stdio.h>
#include <string.h>

DcpWorld g_dcp_world;
ObjStage g_obj_stage;
AudioHost g_ah;

static void logf_(const char *s) {
    size_t n = strlen(g_ah.log);
    if (n + strlen(s) + 2 < sizeof g_ah.log) { strcat(g_ah.log, s); strcat(g_ah.log, "\n"); }
}
void audio_host_reset_log(void) { g_ah.log[0] = 0; }

int audio_music_play_layers(const char *path, uint32_t ls, uint32_t le, int vol0, int vol1) {
    char b[160];
    snprintf(b, sizeof b, "music %s %u-%u %d/%d", path, ls, le, vol0, vol1);
    logf_(b);
    g_ah.gen[0]++;
    snprintf(g_ah.music, sizeof g_ah.music, "%s", path);
    g_ah.mvol[0] = vol0; g_ah.mvol[1] = vol1; g_ah.mframes[0] = g_ah.mframes[1] = 0;
    g_ah.music_on = 1;
    return 0;
}
int audio_music_play(const char *path, uint32_t ls, uint32_t le) { return audio_music_play_layers(path, ls, le, 127, 0); }
void audio_music_layer_volume(int layer, int vol, int frames) {
    char b[64];
    snprintf(b, sizeof b, "layer %d -> %d / %d", layer, vol, frames);
    logf_(b);
    if (layer >= 0 && layer < 2) { g_ah.mvol[layer] = vol; g_ah.mframes[layer] = frames; }
}
void audio_music_fade_stop(int frames) {
    char b[64];
    snprintf(b, sizeof b, "music stop %d", frames);
    logf_(b);
    g_ah.gen[0]++;
    g_ah.music_on = 0; g_ah.music_stop_frames = frames;
}
int audio_music_active(void) { return g_ah.music_on; }
void audio_music_stop(void) {
    logf_("music stop all");
    g_ah.gen[0]++; g_ah.gen[1]++; g_ah.gen[2]++;
    g_ah.music_on = 0; g_ah.j[0].on = g_ah.j[1].on = 0;
}
int audio_jingle_preload(const char *path) {
    char b[160];
    snprintf(b, sizeof b, "preload %s", path);
    logf_(b);
    g_ah.preloads++;
    return 0;
}
void audio_jingle_flush(void) { logf_("jingle flush"); g_ah.gen[1]++; g_ah.gen[2]++; g_ah.j[0].on = g_ah.j[1].on = 0; }
int audio_jingle_play(int slot, const char *path, uint32_t ls, uint32_t le, int vol) {
    char b[160];
    snprintf(b, sizeof b, "jingle %d %s %u-%u %d", slot, path, ls, le, vol);
    logf_(b);
    g_ah.gen[1 + slot]++;
    snprintf(g_ah.j[slot].path, sizeof g_ah.j[slot].path, "%s", path);
    g_ah.j[slot].on = 1; g_ah.j[slot].paused = 0; g_ah.j[slot].vol = vol; g_ah.j[slot].stop_frames = -1;
    return 0;
}
void audio_jingle_stop(int slot, int frames) {
    char b[64];
    snprintf(b, sizeof b, "jingle %d stop %d", slot, frames);
    logf_(b);
    g_ah.gen[1 + slot]++;
    g_ah.j[slot].on = 0; g_ah.j[slot].stop_frames = frames;
}
void audio_jingle_pause(int slot, int on) {
    if (g_ah.j[slot].on && g_ah.j[slot].paused != on) {
        char b[64];
        snprintf(b, sizeof b, "jingle %d pause %d", slot, on);
        logf_(b);
        g_ah.j[slot].paused = on;
    }
}
void audio_jingle_volume(int slot, int vol, int frames) { (void)frames; g_ah.j[slot].vol = vol; }
int audio_jingle_active(int slot) { return g_ah.j[slot].on; }
uint32_t audio_slot_gen(int slot) { return slot >= 0 && slot < 3 ? g_ah.gen[slot] : 0; }
