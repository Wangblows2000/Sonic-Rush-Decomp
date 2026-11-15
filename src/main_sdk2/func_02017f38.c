#include <nitro/types.h>

typedef struct {
    void *unk0;
    void *unk4;
    u8 pad8[4];
    u8 state;
} UnkStruct_02080e74;

extern UnkStruct_02080e74 data_02080e74;

BOOL func_02017ec0(void);
void func_02017e74(void);
s32 func_02017cf4(u32 a, u32 b, u32 c);
void func_02017cc4(void);
s32 func_02017a18(u32 a, u32 b, u32 c);

s32 func_02017f38(u32 a, u32 b, u32 c)
{
    for (;;) {
        switch (data_02080e74.state) {
        case 0:
        case 1:
            {
                s32 r = func_02017ec0();
                if (r == 0) {
                    return r;
                }
            }
            break;
        case 2:
            func_02017e74();
            break;
        case 3:
            return func_02017cf4(a, b, c);
        case 4:
            func_02017cc4();
            break;
        case 5:
            return func_02017a18(a, b, c);
        case 6:
            break;
        }
    }
}
