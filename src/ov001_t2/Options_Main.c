#include <nitro/types.h>

typedef struct {
    s32 state;
    s32 sel;
} UnkWork_022ce29c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022ce29c *work;
} UnkTask_022ce29c;

extern UnkTask_022ce29c *gTaskCurrent;

void Task_DestroyByTag(u16 id);
void Options_ExitToTitleMenu(void);
void Options_ExitToRecords(void);
void Options_ExitToTimeAttackMenuMode2(void);
void Options_ExitToTimeAttackMenuMode3(void);
void Options_ExitToTitleScreen(void);

#pragma thumb on
void Options_Main(void) {
    UnkWork_022ce29c *w = gTaskCurrent->work;
    s32 sel;

    if (w->state == 10) {
        sel = w->sel;
        Task_DestroyByTag(0x1000);
        switch (sel) {
        case 0:
            Options_ExitToTitleMenu();
            break;
        case 1:
            Options_ExitToRecords();
            break;
        case 2:
            Options_ExitToTimeAttackMenuMode2();
            break;
        case 3:
            Options_ExitToTimeAttackMenuMode3();
            break;
        case 4:
            Options_ExitToTitleScreen();
            break;
        }
    }
}
#pragma thumb reset
