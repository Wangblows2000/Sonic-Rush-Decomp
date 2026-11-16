#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u8 sub[0x58];
    s32 unk5c;
    u8 pad60[0x1994 - 0x60];
    u8 obj1994[4];
    s32 unk1998;
    u8 pad199c[0x10];
    s32 unk19ac;
    u8 pad19b0[0x1cc4 - 0x19b0];
    s32 unk1cc4;
} UnkMap_022e7504;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padc[0x68];
    s32 unk74;
    s32 unk78;
    u8 pad7c[8];
    s32 unk84;
    s32 unk88;
    s32 unk8c;
    u8 pad90[0x14];
    UnkMap_022e7504 *map;
} UnkWork_022e7504;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e7504 *work;
} UnkTask_022e7504;

typedef struct {
    s32 unk0;
} UnkCfg_022e7504;

extern UnkTask_022e7504 *gTaskCurrent;
extern UnkCfg_022e7504 gGameState;

void func_ov001_022e3840(void *p, s32 a);
void func_ov001_022e3844(void *p, s32 a);
void SysSound_Release(void);
BOOL func_020538a8(void);
void func_ov001_022e1aac(s32 a);
void func_ov001_022e1a30(s32 a);
void func_02060ea0(void);
void func_ov001_022e6dec(UnkWork_022e7504 *w);
BOOL TalkDemo_IsActive(void);
void func_ov001_022e5618(void *p);
BOOL func_ov001_022e2f00(s32 a);
BOOL func_ov001_022e2ee8(s32 a);
void func_ov001_022e3180(s32 a, s32 b);
void SysSound_Init(void);
void func_02052168(s32 a);
void func_ov001_022e6870(void);

#pragma thumb on
void func_ov001_022e7504(void) {
    UnkWork_022e7504 *w = gTaskCurrent->work;
    s32 d;
    BOOL ok;

    if (w->unk74 == 0) {
        func_ov001_022e3840(w->map->obj1994, 0x1000);
        func_ov001_022e3844(w->map->obj1994, 0);
    }
    if ((u32)w->unk74 < 0x10) {
        w->map->unk19ac = ((0x10 - w->unk74) * 0x20) / 0x10;
    }
    if (w->unk74 == 0xf) {
        w->map->unk1998 = 0;
        w->map->unk5c = 0;
        SysSound_Release();
    }
    if (w->unk88 == 0 && (u32)w->unk74 >= 0x10 && !func_020538a8()) {
        w->unk84 = 1;
    }
    if (w->unk84 == 1 && w->unk88 == 0) {
        w->unk78 = 0;
        switch (w->unk0) {
        case 0:
            func_ov001_022e1aac(w->unk4);
            break;
        case 1:
            func_ov001_022e1a30(w->unk8);
            break;
        }
        func_02060ea0();
        w->unk88 = 1;
    }
    func_ov001_022e6dec(w);
    w->unk74++;
    if (w->unk88 == 1 && !TalkDemo_IsActive()) {
        if (w->unk78 == 0) {
            w->unk78 = w->unk74;
            w->unk8c = 1;
            func_ov001_022e5618(w->map->sub);
            ok = TRUE;
            w->map->unk5c = 1;
            w->map->unk1998 = 1;
            switch (w->unk0) {
            case 0:
                ok = func_ov001_022e2f00(w->unk4);
                break;
            case 1:
                ok = func_ov001_022e2ee8(w->unk8);
                break;
            }
            if (ok) {
                func_ov001_022e3180(w->map->unk1cc4, 1);
            }
        }
        d = w->unk74 - w->unk78;
        w->map->unk19ac = (d * 0x20) / 0x10;
        if (w->map->unk19ac > 0x1f) {
            w->map->unk19ac = 0x1f;
        }
        if (d >= 0xf) {
            BOOL alt;
            w->unk74 = 0;
            w->unk78 = 0;
            w->map->unk19ac = 0x1f;
            SysSound_Init();
            alt = FALSE;
            if (gGameState.unk0 == 0) {
                if (w->unk4 >= 0x1f && w->unk4 <= 0x21) {
                    alt = TRUE;
                }
            } else if (w->unk8 >= 0x1e && w->unk8 <= 0x20) {
                alt = TRUE;
            }
            if (alt) {
                func_02052168(5);
            } else {
                func_02052168(4);
            }
            func_ov001_022e6870();
        }
    }
}
#pragma thumb reset
