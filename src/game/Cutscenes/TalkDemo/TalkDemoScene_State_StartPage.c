/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TalkDemo_GetScriptPage();
void TalkDemoScene_State_RockPage();
s32 TalkDemo_ResetBg2Affine();
void TalkDemoScene_State_LoadImage10x10();
s32 TalkDemoScene_QueuePageSound();

void TalkDemoScene_State_StartPage(u8 *p0) {
    s32 t0 = TalkDemo_GetScriptPage(*(u32 *)((u8 *)p0 + 0x68), ((u32)(*(u32 *)((u8 *)p0 + 0x74) << 0x18) >> 0x18));
    *(u32 *)((u8 *)p0 + 0x6c) = t0;
    s32 t1 = *(u32 *)((u8 *)p0 + 0x6c);
    *(u16 *)((u8 *)p0 + 0x7e) = 0;
    s32 t2 = *(u32 *)((u8 *)t1 + 0x30);
    *(u16 *)((u8 *)p0 + 0x80) = t2;
    s32 t3 = *(u32 *)((u8 *)t1 + 0x4);
    if (t3 == 2) {
        *(u32 *)p0 = (u32)TalkDemoScene_State_RockPage;
    } else {
        s32 v4 = t2;
        if (t3 == 4) {
            s32 t5 = (*(u32 *)((u8 *)p0 + 0x4) | 1);
            *(u32 *)((u8 *)p0 + 0x4) = t5;
            v4 = t5;
        }
        s32 t6 = *(u32 *)((u8 *)t1 + 0x4);
        s32 v7 = t6;
        s32 v8 = v4;
        if (t6 == 5) {
            s32 t9 = (*(u32 *)((u8 *)p0 + 0x4) | 0x10);
            *(u32 *)((u8 *)p0 + 0x4) = t9;
            v7 = 0x10;
            v8 = t9;
        }
        if (*(u32 *)((u8 *)t1 + 0x14) != 1) {
            TalkDemo_ResetBg2Affine(0, v7, v8);
        }
        *(u32 *)((u8 *)p0 + 0xc) = 0;
        *(u32 *)p0 = (u32)TalkDemoScene_State_LoadImage10x10;
    }
    TalkDemoScene_QueuePageSound(p0);
    *(u32 *)((u8 *)p0 + 0x10) = 1;
}

#pragma thumb reset
