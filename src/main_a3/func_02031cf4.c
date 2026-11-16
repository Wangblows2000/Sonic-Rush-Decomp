#include <nitro/types.h>

typedef struct {
    u32 pos;
    u16 type;
    u16 pad;
} UnkTblEntry_02031cf4;

typedef struct {
    u32 count;
    UnkTblEntry_02031cf4 entries[1];
} UnkTbl_02031cf4;

typedef struct {
    u32 unk0;
    u32 tableOfs;
} UnkRes_02031cf4;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 idx;
} UnkCmd_02031cf4;

typedef struct {
    u8 unk0[0xa];
    u16 unkA;
    u16 unkC;
    u16 unkE;
    UnkRes_02031cf4 *res;
    u8 unk14[8];
    u32 pos;
} UnkCtx_02031cf4;

static inline UnkTbl_02031cf4 *GetTbl_02031cf4(UnkRes_02031cf4 *r)
{
    return (UnkTbl_02031cf4 *)((u32)r + r->tableOfs);
}

BOOL func_02031cf4(UnkCmd_02031cf4 *cmd, UnkCtx_02031cf4 *c)
{
    c->unkA = 0;
    c->unkE = 0;
    c->pos = GetTbl_02031cf4(c->res)->entries[cmd->idx].pos;
    return TRUE;
}
