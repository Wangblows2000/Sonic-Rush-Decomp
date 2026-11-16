#include <ov000_b/stageobj.h>

void func_ov000_0231dbc4(UnkStruct_EffObj *w);
void func_ov000_0231e06c(UnkStruct_EffObj *w);
void func_ov000_0231d904(UnkStruct_EffObj *w);
void func_ov000_0231db90(UnkStruct_EffObj *w);

void func_ov000_0231de44(UnkStruct_EffObj *w) {
    func_ov000_0231dbc4(w);
    if (w->unkC == 0) {
        if ((w->unk58 & 1) && !(w->unk58 & 0x2000)) {
            func_ov000_0231e06c(w);
            return;
        }
    } else {
        w->unkC--;
    }
    func_ov000_0231d904(w);
    func_ov000_0231db90(w);
}
