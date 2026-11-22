/* One-file pack of the small per-stage files (tools/dreamcast/mkpak.py, "SPAK" v1):
 * sprite frames / palettes / atlas pages, object hit boxes, UI backgrounds, special-stage
 * and Extra-boss sprites. Each stand-alone file costs an ISO 9660 directory walk on the
 * GD-ROM (~30 ms in Flycast); a file in the pack costs one seek + one read.
 * /cd/data.pak is opened on the first pak_load and stays open. */
#ifndef SRDC_PAK_H
#define SRDC_PAK_H
#include <stdint.h>

/* path "/cd/<key>": the file from the pack, malloc'ed (32-byte aligned), *size set.
 * Returns PAK_FOUND, PAK_MISSING (the pack holds the directory but not this file: do not
 * look on the disc), PAK_NONE (not a packed directory, or no pack: use the file system). */
enum { PAK_NONE = 0, PAK_FOUND = 1, PAK_MISSING = 2 };
int pak_load(const char *path, void **out, uint32_t *size);
/* A stage's own pack (tools/dreamcast/mkpak.py --stage-packs: what objects_init loads, in
 * load order) read whole in one request; pak_load serves from it until pak_stage_end.
 * Returns 0, or -1 without the file (pak_load then reads data.pak / the disc). */
int pak_stage_begin(const char *path);
void pak_stage_end(void);
/* statistics for the load profile: packed files served, their bytes, GD-ROM reads made */
void pak_stats(uint32_t *files, uint32_t *bytes, uint32_t *reads);
#endif
