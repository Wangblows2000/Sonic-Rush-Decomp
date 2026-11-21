#include <nitro/types.h>

extern void *gMainHeap;
extern void *data_ov000_0236ac28;
extern u8 data_ov000_0236ac38[];
extern u8 data_ov000_0236ac40[];
extern u8 data_ov000_0236ac48[];
extern u8 data_ov000_0236ac50[];
extern u8 data_ov000_0236ac58[];
extern u8 data_ov000_02368dbc[];
extern u8 data_ov000_02368dc8[];
extern u8 data_ov000_02368dd4[];
extern u8 data_ov000_02368de0[];
extern u8 data_ov000_02368dec[];

void ObjData_LoadFromArchiveCopy(void *obj, void *def, void *res);
void ObjData_SetupModelResource(void *obj, s32 a);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

static inline void UnkFree_0234dfb8(void **pp) {
    void *p = *pp;
    u32 size;
    if (p != NULL) {
        if (OS_GetArenaLo(0) != p) {
            p = (u8 *)p - 0x20;
            size = *(u32 *)p;
            if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
                vu32 fill = 0xcccccccc;
                MIi_CpuClear32(fill, p, size);
                DC_StoreRange(p, size);
            }
        }
        Heap_Free(gMainHeap, p);
    }
    *pp = NULL;
}

void Player_LoadAssets(void) {
    ObjData_LoadFromArchiveCopy(data_ov000_0236ac50, data_ov000_02368dbc, data_ov000_0236ac28);
    ObjData_LoadFromArchiveCopy(data_ov000_0236ac58, data_ov000_02368dc8, data_ov000_0236ac28);
    ObjData_SetupModelResource(data_ov000_0236ac50, 0);
    ObjData_SetupModelResource(data_ov000_0236ac58, 0);
    ObjData_LoadFromArchiveCopy(data_ov000_0236ac38, data_ov000_02368dd4, data_ov000_0236ac28);
    ObjData_LoadFromArchiveCopy(data_ov000_0236ac40, data_ov000_02368de0, data_ov000_0236ac28);
    ObjData_LoadFromArchiveCopy(data_ov000_0236ac48, data_ov000_02368dec, data_ov000_0236ac28);
    UnkFree_0234dfb8(&data_ov000_0236ac28);
}
