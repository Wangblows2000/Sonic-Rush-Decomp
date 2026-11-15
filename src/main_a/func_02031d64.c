#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkCmd_02031d64;
typedef struct { u8 unk0[0x1C]; u32 unk1C; } UnkStruct_02031d64;
typedef void (*UnkCallback_02031d64)(UnkCmd_02031d64 *, UnkStruct_02031d64 *, void *);

BOOL func_02031d64(UnkCmd_02031d64 *cmd, UnkStruct_02031d64 *s, UnkCallback_02031d64 cb, void *arg) {
    if (cb != NULL) cb(cmd, s, arg);
    s->unk1C += cmd->unk2;
    return TRUE;
}
