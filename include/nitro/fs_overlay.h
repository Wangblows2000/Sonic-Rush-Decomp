#ifndef NITRO_FS_OVERLAY_H
#define NITRO_FS_OVERLAY_H

/* Overlay IDs as link-time symbols. dsd's LCF defines OVERLAY_<n>_ID; NitroSDK
 * code writes FS_OVERLAY_ID(name). Map names used in this project to them. */
extern u32 OVERLAY_0_ID[1];
extern u32 OVERLAY_1_ID[1];
#define FS_OVERLAY_SYM_ov000 OVERLAY_0_ID
#define FS_OVERLAY_SYM_ov001 OVERLAY_1_ID
#define FS_OVERLAY_SYM_(name) FS_OVERLAY_SYM_##name
#define FS_EXTERN_OVERLAY(name)
#define FS_OVERLAY_ID(name) ((u32)&FS_OVERLAY_SYM_(name))

#endif
