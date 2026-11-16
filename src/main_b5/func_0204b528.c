#include <nitro/types.h>

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

typedef struct {
    u8 pad0[0x1c];
    u8 stage;
    u8 act;
} UnkStruct_022c4560;

extern void *data_0208fe80;
extern u8 *data_022c4488;
extern UnkStruct_022c4560 data_022c4560;
extern void (*data_02074184[][3])(void);

void func_ov000_0233bc44(void);
void func_ov000_02335d0c(void);
void func_0204b2ec(void);
void func_02054378(void);
void func_020451c8(void);

static inline void Free_0204b528(u8 *p) {
    if ((u8 *)func_02006abc(0) != p) {
        u32 size;
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    func_020349b8(data_0208fe80, p);
}

void func_0204b528(void) {
    data_02074184[data_022c4560.stage][data_022c4560.act]();
    func_ov000_0233bc44();
    func_ov000_02335d0c();
    if (data_022c4488 != NULL) {
        Free_0204b528(data_022c4488);
        data_022c4488 = NULL;
    }
    func_0204b2ec();
    if (data_022c4560.stage < 9 && data_022c4560.act < 2)
        return;
    func_02054378();
    func_020451c8();
}
