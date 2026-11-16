#include <nitro/types.h>

typedef struct UnkStruct_02318b08_3 {
    u8 pad[0x330];
    u32 unk330;
} UnkStruct_02318b08_3;

typedef struct UnkStruct_02318b08_2 {
    s16 unk0;
    u16 unk2;
    u8 pad[4];
    void *unk8;
} UnkStruct_02318b08_2;

typedef struct UnkStruct_02318b08 {
    u8 pad[0x10];
    UnkStruct_02318b08_2 *unk10;
} UnkStruct_02318b08;

extern UnkStruct_02318b08_3 *data_ov000_0236ac60;
extern u16 data_ov000_0236a2bc[];

void func_0201c070(void *p, s32 a);
void func_0201bf1c(void *p);
void Sound_FreeHandle(void *p);
void func_ov000_0233e5c8(s32 a, s32 b);

void func_ov000_02318b08(UnkStruct_02318b08 *s) {
    UnkStruct_02318b08_2 *p = s->unk10;
    func_0201c070(p->unk8, 0);
    if (p->unk8 != NULL)
        func_0201bf1c(p->unk8);
    if (p->unk8 != NULL)
        Sound_FreeHandle(p->unk8);
    if (p->unk2 != 0) {
        if (data_ov000_0236ac60->unk330 & 0x8000)
            func_ov000_0233e5c8(3, 8);
        data_ov000_0236ac60->unk330 &= ~0x4000;
    }
    data_ov000_0236a2bc[p->unk0] = 0;
}
