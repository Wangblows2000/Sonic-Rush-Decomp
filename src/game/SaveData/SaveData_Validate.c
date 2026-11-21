#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u32 unk4[21];
    u16 unk58;
    u8 unk5a;
    u8 pad5b;
    u32 unk5c[21];
} UnkSave_022c468c;

typedef struct {
    u8 pad0[0x16];
    u16 unk16[105];
    u8 pade8[4];
    u16 unkec[20];
} UnkSave_022c473c;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1c;
    u8 unk1d;
    u8 unk1e;
    u8 unk1f;
} UnkEntry_022c4850;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    UnkEntry_022c4850 e[10];
} UnkSave_022c4850;

extern u32 gSaveData;
extern UnkSave_022c468c data_022c468c;
extern UnkSave_022c473c data_022c473c;
extern UnkSave_022c4850 data_022c4850;

BOOL SaveData_VerifyChecksum(void *p);
u32 SaveData_CalcChecksum(void *p);

BOOL SaveData_Validate(void) {
    BOOL ok;
    u32 i;
    UnkSave_022c468c *s;
    UnkSave_022c473c *t;
    UnkSave_022c4850 *u;
    u32 *p32;
    u16 *p16;
    UnkEntry_022c4850 *e;

    ok = TRUE;
    if (!SaveData_VerifyChecksum(&gSaveData)) {
        gSaveData = SaveData_CalcChecksum(&gSaveData);
        ok = FALSE;
    }
    s = &data_022c468c;
    if (s->unk2 > 99) {
        s->unk2 = 99;
        ok = FALSE;
    }
    if (s->unk5a > 99) {
        s->unk5a = 99;
        ok = FALSE;
    }
    if (s->unk0 > 26) {
        s->unk0 = 26;
        ok = FALSE;
    }
    if (s->unk58 > 28) {
        s->unk58 = 28;
        ok = FALSE;
    }
    if (s->unk3 & 0x80) {
        s->unk3 &= ~0x80;
        ok = FALSE;
    }
    p32 = s->unk4;
    for (i = 0; i < 21; i++) {
        if (p32[i] > 999999) {
            p32[i] = 999999;
            ok = FALSE;
        }
    }
    p32 = s->unk5c;
    for (i = 0; i < 21; i++) {
        if (p32[i] > 999999) {
            p32[i] = 999999;
            ok = FALSE;
        }
    }
    t = &data_022c473c;
    p16 = (u16 *)((u32)t + 0x16);
    for (i = 0; i < 105; i++) {
        if (p16[i] != 0xffff && p16[i] > 36000) {
            p16[i] = 36000;
            ok = FALSE;
        }
    }
    p16 = (u16 *)((u32)t + 0xec);
    for (i = 0; i < 20; i++) {
        if (p16[i] != 0xffff && p16[i] > 36000) {
            p16[i] = 36000;
            ok = FALSE;
        }
    }
    u = &data_022c4850;
    if (u->unk0 > 99) {
        u->unk0 = 99;
        ok = FALSE;
    }
    if (u->unk1 > 99) {
        u->unk1 = 99;
        ok = FALSE;
    }
    if (u->unk2 > 99) {
        u->unk2 = 99;
        ok = FALSE;
    }
    e = u->e;
    for (i = 0; i < 10; i++, e++) {
        if (e->unk1c > 10) {
            e->unk1c = 10;
            ok = FALSE;
        }
        if (e->unk1d > 99) {
            e->unk1d = 99;
            ok = FALSE;
        }
        if (e->unk1e > 99) {
            e->unk1e = 99;
            ok = FALSE;
        }
        if (e->unk1f > 99) {
            e->unk1f = 99;
            ok = FALSE;
        }
    }
    gSaveData = SaveData_CalcChecksum(&gSaveData);
    return ok;
}
