#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x58 - 2];
    u32 unk58;
    u8 pad5C[0x32C - 0x5C];
    u32 unk32C;
    u8 pad330[0x3E8 - 0x330];
    void *unk3E8;
} UnkStruct_0231636c_P;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[6];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x34 - 0x10];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    s16 unk62;
    u8 pad64[0x9C - 0x64];
    u16 unk9C;
    u8 pad9E[0x1F4 - 0x9E];
    u32 unk1F4;
    u8 pad1F8[0x204 - 0x1F8];
    UnkStruct_0231636c_P *unk204;
} UnkStruct_0231636c;

void StageObj_SetAnimation(void *w, s32 a);
s16 ObjSpd_DownSet(s16 a, s32 b);
s16 ObjSpd_UpSet(s16 a, s32 b, s32 c);

void GmkSpecial_State_Main(UnkStruct_0231636c *w) {
    UnkStruct_0231636c_P *p = w->unk204;
    s32 v;
    s32 a;

    if (p != NULL) {
        w->unk1F4 &= ~2;
        w->unk5C &= ~0x10;
        if (p->unk32C & 0x80) {
            if (w->unk9C == 2) {
                StageObj_SetAnimation(w, 0);
                w->unk5C |= 4;
            }
        } else if (w->unk9C == 0) {
            StageObj_SetAnimation(w, 2);
            w->unk5C |= 4;
        }
        w->unk1F4 &= ~1;
        if (p->unk3E8 != w || !(p->unk58 & 0x2000)) {
            w->unk204 = NULL;
            w->unkC = w->unk1 << 8;
            w->unk1F4 |= 2;
            return;
        }
        w->unk1 = w->unk204->unk1;
    } else {
        if (w->unk54 & 2) {
            w->unk8++;
            if (w->unk8 > 0x10) {
                w->unk8 = 0;
                w->unk54 &= ~2;
            }
        }
        v = w->unkC;
        if ((v < 0 ? -v : v) < 0x100 && (w->unk34 < 0 ? -w->unk34 : w->unk34) < 0x80) {
            {
                w->unkC = 0;
                w->unk34 = 0;
                if (!(w->unk1F4 & 1))
                    w->unk62 = 0x20;
                w->unk1F4 |= 1;
                goto end;
            }
        }
        if (v < 0) {
            if (w->unk34 > 0)
                w->unk34 = ObjSpd_DownSet(w->unk34, 0x40);
            w->unk34 = ObjSpd_UpSet(w->unk34, -0x40, 0xf00);
        } else {
            if (w->unk34 < 0)
                w->unk34 = ObjSpd_DownSet(w->unk34, 0x40);
            w->unk34 = ObjSpd_UpSet(w->unk34, 0x40, 0xf00);
        }
        w->unkC -= w->unk34;
        w->unk1 = (w->unkC & 0xffff) >> 8;
    }
end:
    if (!(w->unk1F4 & 2))
        return;
    w->unk1F4 &= ~2;
    StageObj_SetAnimation(w, 0);
    w->unk5C |= 0x10;
    w->unk5C |= 4;
}
