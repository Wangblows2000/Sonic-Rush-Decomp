/*
 * VMU backend for the save module (KOS only). The DS backup image
 * (SAVE_IMAGE_SIZE bytes) is the payload of one VMU file, packaged with
 * vmu_pkg (description + the DS banner icon converted by the menus
 * converter). First memory card found on the maple bus (A1, A2, B1, ...).
 *
 * Size: 128-byte header + 512-byte icon + 8012-byte image = 8652 bytes, 17 blocks.
 */
#ifdef _arch_dreamcast
#include <kos.h>
#include <dc/maple.h>
#include <dc/vmufs.h>
#include <dc/vmu_pkg.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"

#define SAVE_VMU_FILE "SONICRUSH"

static uint16_t s_icon_pal[16];
static uint8_t s_icon[512];
static int s_have_icon;

void save_set_icon(const uint16_t pal[16], const uint8_t data[512]) {
    memcpy(s_icon_pal, pal, sizeof(s_icon_pal));
    memcpy(s_icon, data, sizeof(s_icon));
    s_have_icon = 1;
}

static maple_device_t *vmu_dev(void) {
    return maple_enum_type(0, MAPLE_FUNC_MEMCARD);
}

static int vmu_present(void) { return vmu_dev() != NULL; }

static int vmu_read(uint8_t *img, int size) {
    maple_device_t *dev = vmu_dev();
    void *buf = NULL;
    int n = 0;
    vmu_pkg_t pkg;
    if (!dev)
        return -1;
    if (vmufs_read(dev, SAVE_VMU_FILE, &buf, &n) < 0 || !buf)
        return 1;   /* no file yet: erased backup */
    memset(&pkg, 0, sizeof(pkg));
    if (vmu_pkg_parse((uint8_t *)buf, (size_t)n, &pkg) < 0 || pkg.data_len != size) {
        free(buf);
        return 1;   /* not ours / damaged: treated as erased, the game reformats */
    }
    memcpy(img, pkg.data, (size_t)size);
    free(buf);
    return 0;
}

static int vmu_write(const uint8_t *img, int size) {
    maple_device_t *dev = vmu_dev();
    vmu_pkg_t pkg;
    uint8_t *out = NULL;
    int out_size = 0, r;
    if (!dev)
        return -1;
    memset(&pkg, 0, sizeof(pkg));
    strcpy(pkg.desc_short, "SONIC RUSH");
    strcpy(pkg.desc_long, "Sonic Rush DC save data");
    strcpy(pkg.app_id, "SRDC");
    pkg.icon_cnt = s_have_icon ? 1 : 0;
    pkg.icon_anim_speed = 0;
    pkg.eyecatch_type = VMUPKG_EC_NONE;
    pkg.data_len = size;
    memcpy(pkg.icon_pal, s_icon_pal, sizeof(pkg.icon_pal));
    pkg.icon_data = s_icon;
    pkg.data = img;
    if (vmu_pkg_build(&pkg, &out, &out_size) < 0 || !out)
        return -1;
    r = vmufs_write(dev, SAVE_VMU_FILE, out, out_size, VMUFS_OVERWRITE);
    free(out);
    return r < 0 ? -1 : 0;
}

/* call once before save_load() */
void save_vmu_install(void) {
    g_save_backend.present = vmu_present;
    g_save_backend.read = vmu_read;
    g_save_backend.write = vmu_write;
}
#endif
