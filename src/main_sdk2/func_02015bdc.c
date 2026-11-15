#include <nitro/types.h>

typedef struct {
    u8 data[0x48];
} UnkFSFile;

typedef struct {
    u32 top;
    u32 size;
    void *dst;
    u32 result;
} UnkReq_02015bdc;

typedef struct {
    u8 pad0[0x10];
    u8 name[4];
    u32 unk14;
} UnkArc_02015bdc;

typedef struct {
    u8 pad0[0x10];
    UnkReq_02015bdc *req;
    UnkArc_02015bdc *arc;
} UnkArg_02015bdc;

void func_0200be70(UnkFSFile *f);
void *func_0200b828(void *name, u32 len);
BOOL func_0200bbec(UnkFSFile *f, void *arc, u32 top, u32 bottom, s32 id);
s32 func_0200b99c(UnkFSFile *f, void *dst, u32 size);
void func_0200bad8(UnkFSFile *f);
void OS_Terminate(void);

void func_02015bdc(UnkArg_02015bdc *arg)
{
    UnkFSFile file;
    UnkArc_02015bdc *arc = arg->arc;
    UnkReq_02015bdc *req = arg->req;
    u32 top;
    func_0200be70(&file);
    top = req->top;
    if (func_0200bbec(&file, func_0200b828(arc->name, arc->unk14), top, top + req->size, -1)) {
        if (req->size == func_0200b99c(&file, req->dst, req->size)) {
            req->result = 2;
        }
        func_0200bad8(&file);
    }
    if (req->result != 2) {
        OS_Terminate();
    }
}
