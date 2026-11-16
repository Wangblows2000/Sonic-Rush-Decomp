#include <nitro/types.h>

typedef struct {
    void *unk0;
    u8 pad4[0x30];
    s32 unk34;
    u8 pad38[0x10];
} UnkSpr_022e9308;

typedef struct {
    u8 pad0[0x54];
    UnkSpr_022e9308 spr[4];
    u8 pad174[0x184 - 0x174];
    u8 *bufs[3];
} UnkStruct_022e9308;

extern void *gMainHeap;

void ObjVram_Free(void *p, s32 q);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void *OS_GetArenaLo(s32 i);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

#pragma thumb on
void ZoneMapPause_ReleaseAssets(UnkStruct_022e9308 *p) {
    s32 i;
    u8 *buf;
    s32 size;

    for (i = 0; i < 4; i++) {
        if (p->spr[i].unk34) {
            ObjVram_Free(p->spr[i].unk0, p->spr[i].unk34);
        }
        {
            vu16 zero = 0;
            MIi_CpuClear16(zero, &p->spr[i], sizeof(UnkSpr_022e9308));
        }
    }
    for (i = 0; i < 3; i++) {
        buf = p->bufs[i];
        if (buf != NULL) {
            if (OS_GetArenaLo(0) != buf) {
                buf -= 0x20;
                size = *(s32 *)buf;
                if (*(u32 *)(buf + size - 4) == 0x2467531) {
                    vu32 fill = 0xcccccccc;
                    MIi_CpuClear32(fill, buf, size);
                    DC_StoreRange(buf, size);
                }
            }
            Heap_Free(gMainHeap, buf);
            p->bufs[i] = NULL;
        }
    }
}
#pragma thumb reset
