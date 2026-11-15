#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; } UnkCmd_02031dfc;
typedef struct { u8 unk0[0xA]; u16 unkA; u16 unkC; u16 unkE; u8 unk10[0xC]; u32 unk1C; } UnkStruct_02031dfc;

BOOL func_02031dfc(UnkCmd_02031dfc *cmd, UnkStruct_02031dfc *s) {
    s->unkA = 0;
    s->unkE = 0;
    s->unk1C = cmd->unk4;
    return TRUE;
}
