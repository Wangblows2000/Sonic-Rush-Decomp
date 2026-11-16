#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; u16 unk4; u16 unk6; u16 unk8; } UnkCmd_02032288;
typedef struct { u8 unk0[0xA]; u16 unkA; u16 unkC; u8 unkE[0xE]; u32 unk1C; u8 unk20[8]; u16 unk28; } UnkStruct_02032288;

BOOL BAC_FrameGroupFunc_EndFrame(UnkCmd_02032288 *cmd, UnkStruct_02032288 *s) {
    s->unkC = cmd->unk4;
    s->unkA = cmd->unk6;
    s->unk28 = cmd->unk8 << 4;
    s->unk1C += cmd->unk2;
    return FALSE;
}
