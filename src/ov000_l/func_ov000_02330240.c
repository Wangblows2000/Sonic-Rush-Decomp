#include <ov000_b/stageobj.h>

void func_020693f0(s16 *x, s16 *y, u8 a);
void func_ov000_02348350(void);

void func_ov000_02330240(UnkStruct_EffObj *w) {
    s16 x;
    s16 y;

    if (w->unk88 == NULL)
        return;
    x = w->unk2E;
    y = w->unk30;
    func_020693f0(&x, &y, ((UnkStruct_EffObj *)w->unk88)->unk5 + ((UnkStruct_EffObj *)w->unk88)->unk1);
    w->unk10 = ((UnkStruct_EffObj *)w->unk88)->unk10 + (x << 8);
    w->unk14 = ((UnkStruct_EffObj *)w->unk88)->unk14 + (y << 8);
    if (((UnkStruct_EffObj *)w->unk88)->unk68 != func_ov000_02348350)
        w->unk54 |= 4;
}
