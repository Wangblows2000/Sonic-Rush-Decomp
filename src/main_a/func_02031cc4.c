#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkCmd_02031cc4;
typedef struct { u8 unk0[0x1C]; u32 unk1C; } UnkStruct_02031cc4;

BOOL func_02031cc4(UnkCmd_02031cc4 *cmd, UnkStruct_02031cc4 *s) {
    s->unk1C += cmd->unk2;
    return TRUE;
}
