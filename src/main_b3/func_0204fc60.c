#include <nitro/types.h>

typedef struct { u8 pad0[0x18]; u16 unk18; u16 unk1A; } UnkRes_0204fc60;
typedef struct {
    u8 pad0[0x28];
    s32 unk28;
    u32 unk2C;
    u8 pad30[0x14];
    s32 unk44;
    u8 pad48[0x318];
    u8 bg[0x210];           /* 0x360 */
    UnkRes_0204fc60 *res[5];/* 0x570 */
} UnkWork_0204fc60;
typedef struct { u8 pad0[8]; void (*fn)(void); u8 padC[4]; UnkWork_0204fc60 *work; } UnkTask_0204fc60;
extern UnkTask_0204fc60 *gTaskCurrent;

void Background_Init(void *buf, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void Background_Draw(void *buf);
void func_0204fc04(void);

#pragma thumb on
void func_0204fc60(void) {
    UnkWork_0204fc60 *w = gTaskCurrent->work;
    UnkRes_0204fc60 *res;

    switch (w->unk44) {
    case 0:
        res = w->res[0];
        break;
    case 1:
        res = w->res[1];
        break;
    case 2:
        res = w->res[2];
        break;
    case 3:
        res = w->res[3];
        break;
    case 4:
        res = w->res[4];
        break;
    default:
        res = w->res[0];
        break;
    }
    Background_Init(w->bg, res, 0, w->unk28, (u8)w->unk2C, res->unk18, res->unk1A);
    Background_Draw(w->bg);
    gTaskCurrent->fn = func_0204fc04;
}
#pragma thumb reset
