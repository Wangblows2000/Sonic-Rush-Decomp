/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
extern u8 gGameState[];
extern u8 gPad[];
void StageTitleCard_State_ToStartStage();
s32 G3X_SetClearColor();
s32 StageTitleCardStrip_Draw();
s32 StageTitleCardPartE0_Draw();
s32 StageTitleCardPart180_Draw();
s32 StageTitleCardPart260_Draw();
s32 StageTitleCardPart324_Draw();
s32 StageTitleCard_DrawGlyphText();
s32 StageTitleCardPart420_Draw();
s32 StageTitleCardPart20_Draw();
s32 StageTitleCardBg_Draw();
s32 Task_Destroy();

void StageTitleCard_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x488);
    if ((*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x488) & 0x20000000) == 0) {
        if ((t1 & 0x40000000) == 0) {
            if (*(u32 *)((u8 *)gGameState + 0x10) != 1) {
                s32 t2 = (*(u16 *)((u8 *)gPad + 0x4) & 3);
                if (t2 != 0) {
                    *(u32 *)((u8 *)t0 + 0x474) = (u32)StageTitleCard_State_ToStartStage;
                    *(u32 *)((u8 *)t0 + 0x488) = (*(u32 *)((u8 *)t0 + 0x488) | 0x40000000);
                }
            }
        }
    }
    ((s32 (*)())*(u32 *)((u8 *)t0 + 0x478))(t0, *(u32 *)((u8 *)t0 + 0x478));
    ((s32 (*)())*(u32 *)((u8 *)t0 + 0x474))(t0, *(u32 *)((u8 *)t0 + 0x474));
    if ((*(u32 *)((u8 *)t0 + 0x488) & 0x80000000) == 0) {
        G3X_SetClearColor(0x7fff, *(u16 *)((u8 *)(t0 + 0x400) + 0xd8), 0x7fff, 0, 0);
        if ((*(u32 *)((u8 *)t0 + 0x488) & 4) != 0) {
            ((s32 (*)())*(u32 *)((u8 *)t0 + 0x88))(t0, (t0 + 0x88), *(u32 *)((u8 *)t0 + 0x88));
        }
        if ((*(u32 *)((u8 *)t0 + 0x488) & 0x20) != 0) {
            ((s32 (*)())*(u32 *)((u8 *)t0 + 0xe0))(t0, (t0 + 0xe0), *(u32 *)((u8 *)t0 + 0xe0));
        }
        if ((*(u32 *)((u8 *)t0 + 0x488) & 0x80) != 0) {
            ((s32 (*)())*(u32 *)((u8 *)t0 + 0x180))(t0, (t0 + 0x180), *(u32 *)((u8 *)t0 + 0x180));
            ((s32 (*)())*(u32 *)((u8 *)t0 + 0x1f0))(t0, (t0 + 0x1f0), *(u32 *)((u8 *)t0 + 0x1f0));
        }
        if ((*(u32 *)((u8 *)t0 + 0x488) & 0x200) != 0) {
            ((s32 (*)())*(u32 *)((u8 *)t0 + 0x260))(t0, (t0 + 0x260), *(u32 *)((u8 *)t0 + 0x260));
        }
        if ((*(u32 *)((u8 *)t0 + 0x488) & 0x800) != 0) {
            ((s32 (*)())*(u32 *)((u8 *)t0 + 0x324))(t0, (t0 + 0x324), *(u32 *)((u8 *)t0 + 0x324));
        }
        if ((*(u32 *)((u8 *)t0 + 0x488) & 0x2000) != 0) {
            ((s32 (*)())*(u32 *)((u8 *)t0 + 0x3e8))(t0, (t0 + 0x3e8), *(u32 *)((u8 *)t0 + 0x3e8));
        }
        if ((*(u32 *)((u8 *)t0 + 0x488) & 0x8000) != 0) {
            ((s32 (*)())*(u32 *)((u8 *)t0 + 0x404))(t0, (t0 + 0x404), *(u32 *)((u8 *)t0 + 0x404));
        }
        if ((*(u32 *)((u8 *)t0 + 0x488) & 0x20000) != 0) {
            ((s32 (*)())*(u32 *)((u8 *)t0 + 0x420))(t0, (t0 + 0x420), *(u32 *)((u8 *)t0 + 0x420));
        }
        if ((*(u32 *)((u8 *)t0 + 0x488) & 0x80000) != 0) {
            ((s32 (*)())*(u32 *)t0)(t0, t0, *(u32 *)t0);
        }
        ((s32 (*)())*(u32 *)((u8 *)t0 + 0x20))(t0, (t0 + 0x20), *(u32 *)((u8 *)t0 + 0x20));
        if ((*(u32 *)((u8 *)t0 + 0x488) & 1) != 0) {
            ((s32 (*)())*(u32 *)((u8 *)t0 + 0x78))(t0, (t0 + 0x78), *(u32 *)((u8 *)t0 + 0x78));
        }
        if ((*(u32 *)((u8 *)t0 + 0x48c) & 4) != 0) {
            StageTitleCardStrip_Draw(t0);
        }
        if ((*(u32 *)((u8 *)t0 + 0x48c) & 8) != 0) {
            StageTitleCardPartE0_Draw(t0);
        }
        if ((*(u32 *)((u8 *)t0 + 0x48c) & 0x10) != 0) {
            StageTitleCardPart180_Draw(t0);
        }
        if ((*(u32 *)((u8 *)t0 + 0x48c) & 0x20) != 0) {
            StageTitleCardPart260_Draw(t0);
        }
        if ((*(u32 *)((u8 *)t0 + 0x48c) & 0x40) != 0) {
            StageTitleCardPart324_Draw(t0);
        }
        if ((*(u32 *)((u8 *)t0 + 0x48c) & 0x80) != 0) {
            StageTitleCard_DrawGlyphText(t0, (t0 + 0x3e8));
        }
        if ((*(u32 *)((u8 *)t0 + 0x48c) & 0x100) != 0) {
            StageTitleCard_DrawGlyphText(t0, (t0 + 0x404));
        }
        if ((*(u32 *)((u8 *)t0 + 0x48c) & 0x200) != 0) {
            StageTitleCardPart420_Draw(t0);
        }
        if ((*(u32 *)((u8 *)t0 + 0x48c) & 2) != 0) {
            StageTitleCardPart20_Draw(t0);
        }
        if ((*(u32 *)((u8 *)t0 + 0x48c) & 1) == 0) {
            return;
        }
        StageTitleCardBg_Draw(t0);
        return;
    }
    Task_Destroy(*(u32 *)gTaskCurrent);
}
