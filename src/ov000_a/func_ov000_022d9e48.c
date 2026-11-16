#include <nitro/types.h>

typedef struct UnkStruct_022d9e48 {
    u8 pad[0x21c];
    u16 unk21C;
    u16 unk21E;
} UnkStruct_022d9e48;

typedef struct UnkStruct_022d9e48_7 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022d9e48_7;

typedef struct UnkStruct_022d9e48_4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022d9e48_4;

extern UnkStruct_022d9e48_7 data_ov000_02369ac0;
extern UnkStruct_022d9e48 *data_ov000_02369b14;
extern UnkStruct_022d9e48_4 data_022c4560;
extern u32 data_0208fe70;
extern u8 data_ov000_02363178[][8];
extern u8 data_ov000_02363198[][8];
extern u8 data_ov000_023631b8[][8];
extern u8 data_ov000_02363098[];
extern u8 data_ov000_02363090[];
extern u8 data_ov000_02363088[];
extern u8 data_ov000_023630f0[][8];
extern u8 data_ov000_02363120[][8];
extern u8 data_ov000_023630d0[][8];
extern u8 data_ov000_02363100[][8];
extern u8 data_ov000_02363110[][8];

void func_ov000_022dabec(u8 a);
void func_ov000_022dab6c(u8 a);
void func_ov000_022dabac(u8 a);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

static inline s32 GetIdx(void) {
    BOOL b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return data_022c4560.unk14;
}

void func_ov000_022d9e48(void) {
    UnkStruct_022d9e48 *c = data_ov000_02369b14;
    u16 r;
    if (data_ov000_02369ac0.unk8 >= 6) {
        switch (data_ov000_02363178[c->unk21E][c->unk21C]) {
        case 0:
            func_ov000_022dabec(data_ov000_02363098[Rand() & 7]);
            break;
        case 1:
            r = Rand();
            func_ov000_022dab6c(data_ov000_023630f0[GetIdx()][r & 3]);
            break;
        }
    } else if (data_ov000_02369ac0.unk8 >= 3) {
        switch (data_ov000_02363198[c->unk21E][c->unk21C]) {
        case 0:
            func_ov000_022dabec(data_ov000_02363090[Rand() & 7]);
            break;
        case 1:
            r = Rand();
            func_ov000_022dab6c(data_ov000_02363120[GetIdx()][r & 7]);
            break;
        case 2:
            r = Rand();
            func_ov000_022dabac(data_ov000_023630d0[GetIdx()][r & 7]);
            break;
        }
    } else {
        switch (data_ov000_023631b8[c->unk21E][c->unk21C]) {
        case 0:
            func_ov000_022dabec(data_ov000_02363088[Rand() & 7]);
            break;
        case 1:
            r = Rand();
            func_ov000_022dab6c(data_ov000_02363100[GetIdx()][r & 7]);
            break;
        case 2:
            r = Rand();
            func_ov000_022dabac(data_ov000_02363110[GetIdx()][r & 7]);
            break;
        }
    }
    c->unk21C++;
    if (c->unk21C >= 8) {
        c->unk21C = 0;
        c->unk21E = Rand() & 3;
    }
}
