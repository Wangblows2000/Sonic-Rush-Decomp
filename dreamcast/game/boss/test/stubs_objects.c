/* Host-test only: weak stand-ins for object-module functions that are declared
 * but not yet implemented in dreamcast/game/objects (work in progress by the
 * objects owner).  The real definitions override these when they exist. */
#include <stdint.h>
struct rlvl_header_t_;
__attribute__((weak)) void spawn_decorations_in_block(uint16_t x, uint16_t y, const void *rc, const void *o, uint32_t f) {
    (void)x; (void)y; (void)rc; (void)o; (void)f;
}
__attribute__((weak)) void dec_preload(const void *h) { (void)h; }
__attribute__((weak)) int dec_init_level(const uint8_t *level_data) { (void)level_data; return 0; }
/* referenced by obj_enemy.c while its owner adds it */
__attribute__((weak)) void *EffShadow_Create(void *o) { (void)o; return 0; }
