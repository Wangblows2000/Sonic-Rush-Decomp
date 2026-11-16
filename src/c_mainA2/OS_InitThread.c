#include <nitro/types.h>

extern void SDK_SYS_STACKSIZE(void);
extern void SDK_IRQ_STACKSIZE(void);
#define OSi_SYS_STACKSIZE ((s32)SDK_SYS_STACKSIZE)
#define OSi_IRQ_STACKSIZE ((s32)SDK_IRQ_STACKSIZE)
extern void SDK_SECTION_ARENA_DTCM_START(void);

extern char data_027e0000[];
#define HW_DTCM ((u32)data_027e0000)
#define HW_DTCM_SVC_STACK (HW_DTCM + 0x3f80)

#define OSi_LAUNCHER_STACK_LO_DEFAULT SDK_SECTION_ARENA_DTCM_START
#define OSi_LAUNCHER_STACK_HI_MAX (HW_DTCM_SVC_STACK - OSi_IRQ_STACKSIZE)
#define OSi_LAUNCHER_STACK_BOTTOM (HW_DTCM_SVC_STACK - OSi_IRQ_STACKSIZE)

typedef struct UnkThread_02005e64 {
    u8 pad0[0x64];
    u32 state;
    struct UnkThread_02005e64 *next;
    u32 id;
    u32 priority;
    void *profiler;
    u8 pad78[0x84 - 0x78];
    u32 stackTop;
    u32 stackBottom;
    u32 stackWarningOffset;
    u16 joinQueue;
} UnkThread_02005e64;

typedef struct {
    u16 isNeedRescheduling;
    u16 irqDepth;
    u16 unk4;
    u16 pad6;
    UnkThread_02005e64 *current;
    UnkThread_02005e64 *list;
    void *switchCallback;
    UnkThread_02005e64 *entry[16];
} UnkThreadInfo_02005e64;

extern BOOL data_0207d088;
extern UnkThreadInfo_02005e64 data_0207d090;
extern UnkThread_02005e64 **data_0207d084;
extern UnkThread_02005e64 data_0207d178;
extern UnkThread_02005e64 data_0207d0e4;
extern u32 data_0207d2d4[];

void *OS_SetSwitchThreadCallback(void *cb);
void OSi_IdleThreadProc(void *arg);
void OS_CreateThread(UnkThread_02005e64 *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio);

void OS_InitThread(void)
{
    void *stackLo;
    int i;

    if (data_0207d088)
        return;

    data_0207d088 = TRUE;

    for (i = 0; i < 16; i++)
        data_0207d090.entry[i] = NULL;

    data_0207d084 = &data_0207d090.current;

    data_0207d178.priority = 0x10;
    data_0207d178.id = 0;
    data_0207d178.state = 1;
    data_0207d178.next = NULL;
    data_0207d178.profiler = NULL;
    data_0207d090.entry[0] = &data_0207d178;
    data_0207d090.list = &data_0207d178;
    data_0207d090.current = &data_0207d178;

    stackLo = (OSi_SYS_STACKSIZE == 0) ? (void *)((u32)OSi_LAUNCHER_STACK_LO_DEFAULT - OSi_SYS_STACKSIZE) : (void *)((u32)OSi_LAUNCHER_STACK_HI_MAX - OSi_SYS_STACKSIZE);

    data_0207d178.stackBottom = (u32)OSi_LAUNCHER_STACK_BOTTOM;
    data_0207d178.stackTop = (u32)stackLo;
    data_0207d178.stackWarningOffset = 0;

    *(u32 *)(data_0207d178.stackBottom - sizeof(u32)) = 0xfddb597dUL;
    *(u32 *)data_0207d178.stackTop = 0x7bf9dd5bUL;

    data_0207d178.joinQueue = 0;

    data_0207d090.irqDepth = 0x10;
    data_0207d090.isNeedRescheduling = 0;
    data_0207d090.unk4 = 0;

    *(UnkThreadInfo_02005e64 **)0x027fffa0 = &data_0207d090;

    (void)OS_SetSwitchThreadCallback(NULL);

    OS_CreateThread(&data_0207d0e4, OSi_IdleThreadProc, NULL, data_0207d2d4, 0xc8, 0x1f);
    data_0207d0e4.priority = 0x20;
    data_0207d0e4.state = 1;
}
