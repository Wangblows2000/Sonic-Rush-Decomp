#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkCmd_02031de4;
typedef struct { u8 unk0[0x1C]; u32 unk1C; } UnkStruct_02031de4;

BOOL BAC_FrameGroupFunc_Skip_5(UnkCmd_02031de4 *cmd, UnkStruct_02031de4 *s) {
    s->unk1C += cmd->unk2;
    return TRUE;
}
