#include <nitro/types.h>

typedef struct {
    u16 held;
} UnkPad_022503e0;

typedef struct {
    u8 pad0[0x80];
    u32 swapMode;   /* 0x80 */
    u32 swapDepth;  /* 0x84 */
} UnkStruct_0208f754;

extern u8 data_027e0000[];
extern UnkPad_022503e0 data_022503e0;
extern u32 data_02087cb8;
extern u32 data_02087cc4;
extern u32 data_02087cc0;
extern UnkStruct_0208f754 data_0208f754;

void func_02030598(void);
void func_0204edc0(void);
BOOL func_020105d0(void);
void func_0202ff74(void);
void func_02010590(void);
void func_02035134(UnkPad_022503e0 *pad, u16 keys);
void func_02037c60(void);
void func_02034d34(void);
void func_02030538(u32 arg);
void func_0202fd94(void);
void func_0202ff80(void);
void func_020302d8(void);
void func_02036fb8(void);
void func_02034114(void);
void func_02036a0c(u32 screen);
void VBlankIntrWait(void);
void func_02034ec0(void);
void func_0203005c(void);

#define REG_KEYINPUT   (*(vu16 *)0x04000130)
#define HW_BUTTON_XY   (*(vu16 *)0x027fffa8)
#define REG_POWCNT     (*(vu16 *)0x04000304)
#define REG_SWAP_BUFFERS (*(vu32 *)0x04000540)
#define HW_INTR_CHECK  (*(vu32 *)((u32)data_027e0000 + 0x3ff8))

static inline u32 GetPower(void)
{
    return REG_POWCNT & 0x20e;
}

void main(void)
{
    func_02030598();
    func_0204edc0();
    for (;;) {
        if (func_020105d0()) {
            func_0202ff74();
            func_02010590();
        }
        if (!(data_02087cb8 & 0x40)) {
            u16 keys = (u16)(((REG_KEYINPUT | HW_BUTTON_XY) ^ 0x2fff) & 0x2fff);
            func_02035134(&data_022503e0, keys);
        }
        func_02037c60();
        func_02034d34();
        if (!(data_02087cb8 & 0x10) && (data_022503e0.held & 0x30c) == 0x30c) {
            func_02030538(data_02087cc0);
        }
        func_0202fd94();
        func_0202ff80();
        func_020302d8();
        func_02036fb8();
        func_02034114();
        func_02036a0c(0);
        func_02036a0c(1);
        if ((GetPower() & 0xc) && !(data_02087cb8 & 0x8000000)) {
            REG_SWAP_BUFFERS = (data_0208f754.swapDepth << 1) | data_0208f754.swapMode;
        }
        data_02087cc4 = data_02087cb8;
        HW_INTR_CHECK &= ~1;
        VBlankIntrWait();
        data_02087cb8 = data_02087cc4;
        if (data_02087cb8 & 0x100) {
            func_02034ec0();
        }
        func_0203005c();
        data_02087cb8 &= ~0x80000000;
        if (data_02087cb8 & 8) {
            data_02087cb8 |= 0x80000000;
        }
    }
}
