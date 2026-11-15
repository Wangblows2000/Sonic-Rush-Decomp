#include <nitro/types.h>

typedef struct { u8 unk0[0xC]; u32 unkC; } UnkHeader_02032234;
typedef struct { u16 unk0; u16 unk2; u32 unk4; } UnkCmd_02032234;
typedef struct { u8 unk0[0x10]; UnkHeader_02032234 *header; u8 unk14[8]; u32 unk1C; u8 unk20[4]; u32 unk24; u8 unk28[4]; u32 unk2C; } UnkStruct_02032234;

BOOL func_02032234(UnkCmd_02032234 *cmd, UnkStruct_02032234 *s) {
    s->unk24 = cmd->unk4;
    if (*(u16 *)(s->unk24 + ((u8 *)s->header + s->header->unkC) + 2) & 1) {
        s->unk2C |= 0x80000000;
    } else {
        s->unk2C &= ~0x80000000;
    }
    s->unk1C += cmd->unk2;
    return TRUE;
}
