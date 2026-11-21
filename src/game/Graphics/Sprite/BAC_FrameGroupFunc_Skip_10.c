#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkCmd_02031cdc;
typedef struct { u8 unk0[0x1C]; u32 unk1C; } UnkStruct_02031cdc;

BOOL BAC_FrameGroupFunc_Skip_10(UnkCmd_02031cdc *cmd, UnkStruct_02031cdc *s) {
    s->unk1C += cmd->unk2;
    return TRUE;
}
