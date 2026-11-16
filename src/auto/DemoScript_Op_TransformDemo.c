/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTransformDemoId[];
s32 TransformDemo_Start();
void DemoScript_State_WaitTransformDemo();

void DemoScript_Op_TransformDemo(u8 *p0) {
    s32 t0 = *(u8 *)((u8 *)(*(u32 *)((u8 *)p0 + 0x14) + (*(s8 *)((u8 *)p0 + 0x18) << 2)) + 0x1);
    *(u16 *)gTransformDemoId = t0;
    TransformDemo_Start(gTransformDemoId, t0);
    *(u32 *)p0 = (u32)DemoScript_State_WaitTransformDemo;
}

#pragma thumb reset
