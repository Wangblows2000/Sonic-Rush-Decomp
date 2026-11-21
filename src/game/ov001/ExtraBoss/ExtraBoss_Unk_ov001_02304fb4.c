#include <nitro/types.h>

typedef struct UnkStruct_02304fb4_w {
    u8 pad0[0x40];
    s32 unk40;
} UnkStruct_02304fb4_w;

typedef struct UnkStruct_02304fb4_res {
    u8 pad0[0x10];
    u32 palOffset;
} UnkStruct_02304fb4_res;

typedef struct UnkStruct_02304fb4_d {
    u8 pad0[0x584];
    s16 unk584;
    s16 unk586;
    s32 unk588;
    s32 unk58C;
    s32 unk590;
    u8 pad594[0x598 - 0x594];
    void *unk598;
    s32 unk59C;
    s32 unk5A0;
    void *unk5A4;
    s32 unk5A8;
    s16 unk5AC[0xc0];
    s16 unk72C[0xc0];
    s16 unk8AC[8][0x20];
    u8 padAAC[0xb2c - 0xaac];
    u16 unkB2C;
    u8 padB2E[2];
    s32 unkB30;
    s32 unkB34;
    s32 unkB38;
    s32 unkB3C;
    s32 unkB40;
    s32 unkB44;
    s32 unkB48;
} UnkStruct_02304fb4_d;

typedef struct UnkStruct_02304fb4_dma {
    u32 reg;
    void *src0;
    void *src1;
    s32 unkC;
    s32 unk10;
} UnkStruct_02304fb4_dma;

extern UnkStruct_02304fb4_res *data_ov001_0231f550[];
extern UnkStruct_02304fb4_d *data_ov001_0231f54c;
extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;
extern u8 data_ov001_02316f44[];
extern u8 data_ov001_02316f94[];
extern s32 data_ov001_02316ee0[];
extern s32 data_ov001_02316f1c[];
extern s32 data_ov001_02316f08[];
extern s16 data_0206e470[];
extern u32 gRandSeed;
extern UnkStruct_02304fb4_dma data_02087cdc[];
extern u32 gSystemFlags;

UnkStruct_02304fb4_w *ExtraBoss_GetWork(void);
void PaletteTransfer_QueueLoad(void *pal, s32 a, u32 dst);
void PaletteTransfer_Flush(void);
static inline void PaletteLoadNow(void *pal, s32 a, u32 dst)
{
    u32 head = gPaletteTransferHead;
    u32 tail = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueLoad(pal, a, dst);
    PaletteTransfer_Flush();
    gPaletteTransferHead = head;
    gPaletteTransferTail = tail;
}
void PaletteTransfer_QueueLoad(void *pal, s32 a, u32 dst);
void PaletteTransfer_Flush(void);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void ExtraBoss_State_ScrollDone(void);
static inline BOOL IsLate(void) { return ExtraBoss_GetWork()->unk40 > 1 ? TRUE : FALSE; }
void ExtraBoss_State_ScrollSpawn(void);

void ExtraBoss_Unk_ov001_02304fb4(void)
{
    s32 stage = ExtraBoss_GetWork()->unk40;
    s32 i;
    s32 j;
    s16 *dst;

    if (stage < 4) {
        PaletteLoadNow((u8 *)data_ov001_0231f550[0] + data_ov001_0231f550[0]->palOffset, 0, 0x5000000);
        data_ov001_0231f54c->unk5A4 = data_ov001_02316f44;
        data_ov001_0231f54c->unk5A8 = 0x14;
    } else {
        PaletteLoadNow((u8 *)data_ov001_0231f550[11] + data_ov001_0231f550[11]->palOffset, 0, 0x5000000);
        data_ov001_0231f54c->unk5A4 = data_ov001_02316f94;
        data_ov001_0231f54c->unk5A8 = 0x14;
    }

    data_ov001_0231f54c->unk588 = data_ov001_02316ee0[ExtraBoss_GetWork()->unk40];
    data_ov001_0231f54c->unk58C = 0x60000;
    data_ov001_0231f54c->unk584 = 0;
    data_ov001_0231f54c->unk586 = 0x20;
    data_ov001_0231f54c->unk590 = data_ov001_02316f1c[stage];
    if (IsLate())
        data_ov001_0231f54c->unk598 = ExtraBoss_State_ScrollDone;
    else
        data_ov001_0231f54c->unk598 = ExtraBoss_State_ScrollSpawn;

    data_ov001_0231f54c->unkB3C = 0;
    data_ov001_0231f54c->unkB40 = 0;
    data_ov001_0231f54c->unkB44 = 0;
    data_ov001_0231f54c->unkB48 = data_ov001_02316f08[stage];
    data_ov001_0231f54c->unk59C = 0;
    data_ov001_0231f54c->unk5A0 = 0;
    data_ov001_0231f54c->unkB2C = 0;
    data_ov001_0231f54c->unkB34 = 0;
    data_ov001_0231f54c->unkB38 = 0;
    data_ov001_0231f54c->unkB30 = 0xff;

    dst = data_ov001_0231f54c->unk8AC[0];
    for (i = 0; i < 8; i++) {
        s32 amp;
        u32 angle;
        gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
        amp = ((u16)(gRandSeed >> 16) & 0xfff) + 0x1000;
        angle = (u16)((i & 1) << 15);
        for (j = 0; j < 0x20; j++, dst++) {
            s32 s = data_0206e470[((s32)((u16)angle & 0xffff) >> 4) * 2];
            *dst = (s32)(((s64)s * amp + 0x800) >> 12) >> 9;
            angle += 0x400;
        }
    }
    MIi_CpuCopy16(data_ov001_0231f54c->unk8AC[1], data_ov001_0231f54c->unk5AC, 0x180);
    MIi_CpuCopy16(data_ov001_0231f54c->unk8AC[1], data_ov001_0231f54c->unk72C, 0x180);
    {
        void *b = data_ov001_0231f54c->unk72C;
        void *a = data_ov001_0231f54c->unk5AC;
        UnkStruct_02304fb4_dma *h = &data_02087cdc[1];
        h->reg = 0x4000014;
        h->src0 = a;
        h->src1 = b;
        h->unkC = 2;
        h->unk10 = 0;
        gSystemFlags |= 0x800;
    }
}
