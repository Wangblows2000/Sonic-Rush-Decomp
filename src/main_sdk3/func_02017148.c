#include <nitro/types.h>

typedef struct {
    u8 pad0[0x6c];
    u32 unk6c;
} UnkMbFileHdr_02017148;

typedef struct {
    u8 unk0[0xe4];
    u8 unkE4[0x597 - 0xe4];     /* 0x0e4 */
    u8 unk597;                  /* 0x597 */
    u8 pad598;
    u8 unk599;                  /* 0x599 */
    u8 pad59a[0x5a4 - 0x59a];
    u8 unk5a4[0x5b8 - 0x5a4];   /* 0x5a4 */
    void *info;                 /* 0x5b8 */
    u8 *unk5bc;                 /* 0x5bc */
    u8 pad5c0[6];
    u16 unk5c6;                 /* 0x5c6 */
    u8 pad5c8[2];
    u8 unk5ca;                  /* 0x5ca */
    u8 pad5cb;
    UnkMbFileHdr_02017148 *unk5cc; /* 0x5cc */
    u8 *unk5d0;                 /* 0x5d0 */
} UnkEntry_02017148;

typedef struct {
    u8 pad0[0x1300];
    u8 unk1300[0x1524 - 0x1300]; /* 0x1300 */
    u8 unk1524;                  /* 0x1524 */
    u8 pad1525[0x1788 - 0x1525];
    UnkEntry_02017148 entry[16]; /* 0x1788 */
    u8 pad74c8[0x74e0 - 0x74c8];
    u8 unk74e0[0x7ce0 - 0x74e0]; /* 0x74e0 */
    u8 unk7ce0[4];               /* 0x7ce0 */
} UnkStruct_02017148;

extern UnkStruct_02017148 *data_02080f1c;
extern u8 data_02080e68;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
BOOL func_02018340(void);
void func_020170c4(void *dst, u8 *rom);
BOOL func_02016e30(void *a, void *b);
void func_020181dc(void *a, void *info, void *c);
void func_02018038(void *a);
BOOL func_0201a4a4(void);
void func_0201a490(void *p);
void func_0201a4c0(void *p, u32 size);

BOOL func_02017148(void *info, u8 *rom)
{
    u8 i;
    UnkEntry_02017148 *e;
    u8 k = 0xff;
    u32 enabled = OS_DisableInterrupts();

    if (!func_02018340()) {
        OS_RestoreInterrupts(enabled);
        return FALSE;
    }
    if (data_02080f1c->unk1524 + 1 > 16) {
        OS_RestoreInterrupts(enabled);
        return FALSE;
    }
    for (i = 0; i < 16; i++) {
        if (data_02080f1c->entry[i].info == info) {
            OS_RestoreInterrupts(enabled);
            return FALSE;
        }
        if (data_02080f1c->entry[i].unk5ca == 0) {
            k = i;
            break;
        }
    }
    if (i == 16) {
        OS_RestoreInterrupts(enabled);
        return FALSE;
    }
    data_02080f1c->entry[k].info = info;
    e = &data_02080f1c->entry[k];
    func_020170c4(e, rom);
    if (!func_02016e30(data_02080f1c->entry[k].unk5a4, e)) {
        OS_RestoreInterrupts(enabled);
        return FALSE;
    }
    func_020181dc(data_02080f1c->entry[k].unkE4, info, data_02080f1c->unk1300);
    data_02080f1c->entry[k].unk599 = k;
    func_02018038(data_02080f1c->entry[k].unkE4);
    data_02080f1c->entry[k].unk597 = data_02080e68++;
    data_02080f1c->entry[k].unk5c6 = 1;
    data_02080f1c->entry[k].unk5bc = rom;
    data_02080f1c->entry[k].unk5cc = (UnkMbFileHdr_02017148 *)(rom + 0x1e8);
    data_02080f1c->entry[k].unk5d0 = rom + 0x258;
    if (data_02080f1c->entry[k].unk5cc->unk6c != 0 && !func_0201a4a4()) {
        func_0201a490(data_02080f1c->unk7ce0);
        func_0201a4c0(data_02080f1c->unk74e0, 0x800);
    }
    data_02080f1c->entry[k].unk5ca = 1;
    data_02080f1c->unk1524++;
    OS_RestoreInterrupts(enabled);
    return TRUE;
}
