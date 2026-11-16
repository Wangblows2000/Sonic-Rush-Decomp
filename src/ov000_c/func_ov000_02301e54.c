#include <ov000_c/stageobj.h>

extern s8 data_ov000_023643d4[];
extern s8 data_ov000_023643d8[];
extern s8 data_ov000_023643dc[];


void func_ov000_02301e54(UnkStruct_StageObj *w) {
    if (w->spr0.unk8 != 0) {
        s32 t = w->unkC;
        if (t != 0) {
            switch (w->unk8) {
            default:
                w->unk1D6 = data_ov000_023643d4[(10 - t) / 5];
                w->unk2A = (w->unk1D6 << 8) / 5;
                break;
            case 1:
                w->unk1D6 = data_ov000_023643d8[(15 - t) / 5];
                w->unk2A = (w->unk1D6 << 8) / 5;
                break;
            case 2:
                w->unk1D6 = data_ov000_023643dc[20 - t];
                w->unk2A = (w->unk1D6 << 8) / 5;
                break;
            }
            w->unkC--;
        } else {
            w->unk1D6 = 0;
            w->unk2A = 0;
        }
        if (w->unk5C & 8)
            func_02069818(w, 0);
    } else {
        w->unk1D6 = 0;
        w->unk2A = 0;
    }
}
