#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkCmd_02031da4;
typedef struct { u8 unk0[0x1C]; u32 unk1C; } UnkStruct_02031da4;
typedef void (*UnkCallback_02031da4)(UnkCmd_02031da4 *, UnkStruct_02031da4 *, void *);

BOOL func_02031da4(UnkCmd_02031da4 *cmd, UnkStruct_02031da4 *s, UnkCallback_02031da4 cb, void *arg) {
    if (cb != NULL) cb(cmd, s, arg);
    s->unk1C += cmd->unk2;
    return TRUE;
}
