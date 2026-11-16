#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
} UnkWork_023354ac;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0xc8];
    u32 unkC8;
} UnkStruct_0236aad8;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_0236aad8 gCamera;

void func_ov000_023352c8(UnkWork_023354ac *w);
void func_ov000_023350bc(UnkWork_023354ac *w, s32 idx, u32 flags);
void func_ov000_02334eb4(UnkWork_023354ac *w, s32 idx, u32 flags);
void func_ov000_023353c8(void);
void func_ov000_0233545c(void);
void func_ov000_023353dc(void);

void func_ov000_023354ac(UnkWork_023354ac *w) {
    if (!(gGameState.unk1C < 9 && gGameState.unk1D < 2)) {
        func_ov000_023352c8(w);
        w->fn = func_ov000_023353c8;
        return;
    }
    if (gCamera.unkC8 & 1) {
        func_ov000_02334eb4(w, 0, 1);
        w->fn = func_ov000_0233545c;
        return;
    }
    func_ov000_023350bc(w, 0, 1);
    func_ov000_023350bc(w, 1, 1);
    w->fn = func_ov000_023353dc;
}
