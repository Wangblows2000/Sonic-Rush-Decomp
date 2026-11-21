#include <nitro/types.h>

typedef struct UnkStruct_022f9d20_o {
    u32 unk0;
    u8 pad4[0x34 - 4];
    u32 unk34;
    u8 pad38[0x48 - 0x38];
} UnkStruct_022f9d20_o;

typedef struct UnkStruct_022f9d20 {
    u8 pad0[0x18];
    UnkStruct_022f9d20_o objs[0x28];
    u8 padB58[0xc90 - 0xb58];
    u8 unkC90[0xe08 - 0xc90];
    void *tasks[7];
    u8 padE24[0xea0 - 0xe24];
    u8 unkEA0[4];
} UnkStruct_022f9d20;

typedef struct UnkStruct_022f9d20_b {
    u8 pad0[0x1c];
    s32 unk1C;
} UnkStruct_022f9d20_b;

extern UnkStruct_022f9d20_b gDisplayMain;
extern UnkStruct_022f9d20 *data_ov001_0231f084;
extern void *gMainHeap;

void NNS_SndStopSoundAll(void);
void Task_Destroy(void *task);
void ObjVram_Free(u32 a, u32 b);
void Model_Release(void *p);
void NNS_SndHandleReleaseSeq(void *p);
void ExtraAct_ReleaseModelRes(void);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

void ExtraAct_ReleaseAssets(void) {
    s32 i;
    UnkStruct_022f9d20_o *o;
    u8 *blk;

    NNS_SndStopSoundAll();
    gDisplayMain.unk1C = 0;
    for (i = 0; i < 7; i++) {
        if (data_ov001_0231f084->tasks[i] != NULL)
            Task_Destroy(data_ov001_0231f084->tasks[i]);
    }
    o = data_ov001_0231f084->objs;
    for (i = 0; i < 0x28; i++, o++)
        ObjVram_Free(o->unk0, o->unk34);
    Model_Release(data_ov001_0231f084->unkC90);
    Model_Release(&data_ov001_0231f084->objs[0x28]);
    NNS_SndHandleReleaseSeq(data_ov001_0231f084->unkEA0);
    ExtraAct_ReleaseModelRes();
    blk = (u8 *)data_ov001_0231f084;
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    Heap_Free(gMainHeap, blk);
    data_ov001_0231f084 = NULL;
}
