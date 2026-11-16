#include <nitro/types.h>

typedef struct { u8 unk0[0xc]; u32 unkC; u8 unk10[8]; u16 unk18; u16 unk1A; u8 unk1C[4]; } UnkHdr_020525d8;
typedef struct { u8 unk0[0x20]; u32 unk20; u8 unk24[4]; u32 unk28; } UnkFile_020525d8;
void FS_ReadFile(void *f, void *dst, u32 size);
void FS_SeekFile(void *f, u32 off, s32 whence);
void NaviMsg_ReadFaceFrameMap(UnkFile_020525d8 *f, void *dst) { UnkHdr_020525d8 h; FS_ReadFile(f, &h, 0x20); FS_SeekFile(f, h.unkC, 0); FS_ReadFile(f, dst, h.unk18 * h.unk1A * 2 + 4); f->unk28 = f->unk20; }
