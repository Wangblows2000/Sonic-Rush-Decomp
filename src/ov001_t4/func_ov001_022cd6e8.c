#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
} UnkInfo_022cd6e8;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0xc];
    s32 unk2c;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x3ac - 0x38];
    UnkInfo_022cd6e8 *info;
} UnkWork_022cd6e8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022cd6e8 *work;
} UnkTask_022cd6e8;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022cd6e8;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022cd6e8;

extern UnkTask_022cd6e8 *data_022b4574;
extern UnkPad_022cd6e8 data_022503e0;
extern UnkTouch_022cd6e8 data_022b65b4;

void func_ov001_022cc17c(UnkWork_022cd6e8 *w);
void func_ov001_022cbf30(UnkWork_022cd6e8 *w);
void func_ov001_022cdd28(void);
void func_ov001_022cdca4(void);
BOOL func_ov001_022c8ba8(u16 x, u16 y);
void func_02052104(s32 a);

#pragma thumb on
void func_ov001_022cd6e8(void) {
    UnkWork_022cd6e8 *w;
    s32 has;
    u16 x;
    u16 y;
    s32 bx;
    s32 by;

    w = data_022b4574->work;
    has = w->info->unk14;
    func_ov001_022cc17c(w);
    func_ov001_022cbf30(w);
    w->unk14++;
    w->unk18++;
    if (w->unk1c >= 0) {
        w->unk1c++;
        if (w->unk1c > 10) {
            func_ov001_022cdd28();
        }
    }
    if (w->unk1c >= 0) {
        return;
    }
    if (w->unk34 != -1) {
        switch (w->unk2c) {
        case 0:
            switch (w->unk30) {
            case 0:
                if (w->unk18 >= 30) {
                    func_ov001_022cdca4();
                    w->unk34 = -1;
                }
                return;
            case 1:
                w->unk2c = 1;
                w->unk30 = 0;
                w->unk34 = -1;
                return;
            }
            return;
        case 1:
            switch (w->unk30) {
            case 0:
                if (w->unk18 >= 30) {
                    func_ov001_022cdca4();
                    w->unk34 = -1;
                }
                return;
            case 1:
                if (w->unk18 >= 30) {
                    func_ov001_022cdca4();
                    w->unk34 = -1;
                }
                return;
            }
            return;
        }
        return;
    }
    switch (w->unk2c) {
    case 0:
        if (data_022503e0.trg & 0x40) {
            if (w->unk30 != 0) {
                w->unk30 = 0;
                w->unk14 = 0;
                func_02052104(4);
            }
        } else if (data_022503e0.trg & 0x80) {
            if (w->unk30 != 1) {
                w->unk30 = 1;
                w->unk14 = 0;
                func_02052104(4);
            }
        }
        break;
    case 1:
        if (data_022503e0.trg & 0x20) {
            if (w->unk30 != 0) {
                w->unk30 = 0;
                w->unk14 = 0;
                func_02052104(4);
            }
        } else if (data_022503e0.trg & 0x10) {
            if (w->unk30 != 1) {
                w->unk30 = 1;
                w->unk14 = 0;
                func_02052104(4);
            }
        }
        break;
    }
    if (has == 0) {
        w->unk30 = 0;
    }
    if (data_022503e0.trg & 1) {
        w->unk34 = w->unk30;
        w->unk18 = 0;
        func_02052104(0);
        return;
    }
    if (data_022503e0.trg & 2) {
        switch (w->unk2c) {
        case 0:
            func_02052104(1);
            func_ov001_022cdd28();
            return;
        case 1:
            w->unk2c = 0;
            w->unk30 = 1;
            func_02052104(1);
            return;
        }
        return;
    }
    if (!(data_022b65b4.flags & 0x40)) {
        return;
    }
    x = data_022b65b4.x;
    y = data_022b65b4.y;
    if (x < 0x100 && y < 0xc0 && func_ov001_022c8ba8(x, y)) {
        func_02052104(1);
        w->unk1c = 0;
    }
    switch (w->unk2c) {
    case 0:
        if (has) {
            bx = 0x38;
            by = 0x40;
        } else {
            bx = 0x38;
            by = 0x58;
        }
        if (x >= bx && x < bx + 0x90 && y >= by && y < by + 0x16) {
            w->unk30 = 0;
            w->unk34 = w->unk30;
            w->unk18 = 0;
            func_02052104(0);
            return;
        }
        if (has && x >= 0x38 && x < 0xc8 && y >= 0x58 && y < 0x6e) {
            w->unk2c = 1;
            w->unk30 = 0;
            w->unk34 = -1;
            func_02052104(0);
            return;
        }
        break;
    case 1:
        if (x >= 0x38 && x < 0x80 && y >= 0x70 && y < 0x86) {
            w->unk30 = 0;
            w->unk34 = w->unk30;
            w->unk18 = 0;
            func_02052104(0);
            return;
        }
        if (x >= 0x80 && x < 0xc8 && y >= 0x70 && y < 0x86) {
            w->unk30 = 1;
            w->unk34 = w->unk30;
            w->unk18 = 0;
            func_02052104(0);
            return;
        }
        if (has) {
            bx = 0x38;
            by = 0x40;
        } else {
            bx = 0x38;
            by = 0x58;
        }
        if (x >= bx && x < bx + 0x90 && y >= by && y < by + 0x16) {
            w->unk2c = 0;
            w->unk30 = 0;
            w->unk18 = 0;
            func_02052104(0);
            return;
        }
        if (has && x >= 0x38 && x < 0xc8 && y >= 0x58 && y < 0x6e) {
            w->unk2c = 0;
            w->unk30 = 1;
            w->unk18 = 0;
            func_02052104(0);
            return;
        }
        break;
    }
}
#pragma thumb reset
