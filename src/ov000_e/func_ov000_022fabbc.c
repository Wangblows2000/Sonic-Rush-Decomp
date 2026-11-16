#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2;
} UnkStruct_Ev;

typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2;
    u16 unk4;
} UnkStruct_Ent;

typedef struct {
    u8 pad[0x326];
    u8 unk326;
} UnkStruct_Player;

typedef struct {
    u8 pad[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_Player *data_ov000_0236ac60;

void func_0206c95c(void);
UnkStruct_Ev *func_0206c938(s32 a, s32 b);
u8 *func_ov000_023355d0(void);
void func_0204b7ec(s32 a, s32 b, s32 c);
void func_ov000_02342f14(UnkStruct_Player *p);
void func_ov000_02342ef4(UnkStruct_Player *p);
void func_ov000_02342e9c(UnkStruct_Player *p);
void func_ov000_02342e28(UnkStruct_Player *p);

void func_ov000_022fabbc(void) {
    UnkStruct_Ev *o;
    u16 i;
    UnkStruct_Ent *e;
    u8 *base;
    u16 off;

    if (!(data_022c4560.unk18 & 0x20))
        return;
    func_0206c95c();
    for (i = 0; i < 2; i++) {
        if (i == data_ov000_0236ac60->unk326)
            continue;
        for (;;) {
            o = func_0206c938(2, i);
            if (o == NULL)
                break;
            base = func_ov000_023355d0();
            off = o->unk2;
            e = (UnkStruct_Ent *)((u32)off + (u32)base);
            e->unk4 |= 0x2000;
            if (*(u8 *)((u32)off + (u32)base) == 0xff && e->unk2 != 0x30 && e->unk2 != 0x31)
                continue;
            switch (o->unk0) {
            case 7:
                func_0204b7ec(0, 0, 0xdf);
                func_ov000_02342f14(data_ov000_0236ac60);
                break;
            case 8:
                func_0204b7ec(0, 0, 0xdf);
                func_ov000_02342ef4(data_ov000_0236ac60);
                break;
            case 9:
                func_0204b7ec(0, 0, 0xdf);
                func_ov000_02342e9c(data_ov000_0236ac60);
                break;
            case 10:
                func_0204b7ec(0, 0, 0xdf);
                func_ov000_02342e28(data_ov000_0236ac60);
                break;
            case 2:
                e->unk0 = 0xff;
                break;
            }
        }
    }
}
