#include <nitro/types.h>

typedef struct {
    u8 language;
    u8 favoriteColor;
    u8 month;
    u8 day;
    u16 nickName[10];
    u16 nickNameLength;
    u16 comment[26];
    u16 commentLength;
} UnkOwnerInfo_0203bb94;

typedef struct {
    u16 a, b, c, d, e, f, g;
} UnkStruct_022b89e8;

typedef struct {
    u8 color : 4;
    u8 flag : 4;
    u8 nameLength;
    u16 name[10];
    UnkStruct_022b89e8 sub;
} UnkStruct_0203bb94;

extern UnkStruct_022b89e8 data_022b89e8;
extern void *gMainHeap;
typedef struct { void *work; } UnkStruct_022b89e0;
extern UnkStruct_022b89e0 data_022b89e0;
void OS_GetOwnerInfo(UnkOwnerInfo_0203bb94 *info);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
u32 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 *OS_GetArenaLo(u32 a);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
s32 func_02018c64(void *work, UnkStruct_0203bb94 *param, u32 a, u32 b, u32 c);
void OS_Terminate(void);
void MB_SetParentCommParam(u32 a, u32 b);
void MB_CommSetParentStateCallback(void (*cb)(void));
void func_0203b0ac(void);
void func_0203b09c(u32 a);

void func_0203bb94(u32 a, u32 b) {
    UnkStruct_0203bb94 param;
    UnkOwnerInfo_0203bb94 info;
    u32 *work;
    UnkStruct_022b89e8 *sub;

    OS_GetOwnerInfo(&info);
    param.color = info.favoriteColor;
    param.nameLength = info.nickNameLength;
    MI_CpuCopy8(info.nickName, param.name, info.nickNameLength * 2);
    param.flag = 0;
    sub = &param.sub;
    sub->a = 0;
    sub->b = 0;
    sub->c = 0;
    sub->d = 0;
    sub->e = 0;
    sub->f = 0;
    sub->g = 0;
    data_022b89e8 = *sub;
    work = Heap_Alloc(gMainHeap, 0xc040, -32);
    if (OS_GetArenaLo(0) != work) {
        vu32 v = 0xcccccccc;
        MIi_CpuClear32(v, work, 0xc040);
        work[0] = 0xc040;
        work[0x300f] = 0x2467531;
        DC_StoreRange(work, 0xc040);
        work += 8;
    }
    data_022b89e0.work = work;
    if (func_02018c64(data_022b89e0.work, &param, a, b, 2) != 0)
        OS_Terminate();
    MB_SetParentCommParam(0x100, 0xf);
    MB_CommSetParentStateCallback(func_0203b0ac);
    func_0203b09c(1);
}
