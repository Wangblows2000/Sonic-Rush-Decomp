#include <nitro/types.h>
#include <main_a2/strm.h>

typedef struct { u8 unk0[0x30]; } UnkNode_020203bc;

extern u32 data_02083f44;
extern u8 data_02083f50[];
extern UnkNode_020203bc data_02083f70[];
extern u8 data_02083f5c[];
extern u8 *data_02083f4c;
extern u8 data_02084100[];
extern UnkStrm data_020847b8[];
extern u8 data_02084300[];

BOOL func_020202e0(void *heap);
void NNS_FndInitList(void *list, u16 offset);
void func_0202076c(void *list, void *obj);
void func_02006724(void *m);
void func_0200be70(void *file);
void func_0201c748(UnkStrm *s);
void func_0201fa50(u8 *p, u32 prio);

void func_020203bc(u32 prio, void *heap) {
    s32 i;
    UnkStrm *s;
    UnkNode_020203bc *n;
    if (data_02083f44 != 0) {
        func_020202e0(heap);
        return;
    }
    data_02083f44 = 1;
    NNS_FndInitList(data_02083f50, 0);
    n = data_02083f70;
    for (i = 0; i < 8; i++, n++) {
        func_0202076c(data_02083f50, n);
    }
    func_02006724(data_02083f5c);
    data_02083f4c = data_02084100;
    s = data_020847b8;
    for (i = 0; i < 4; i++, s++) {
        s->unk10C_0 = 0;
        func_0200be70(s->file);
        func_0201c748(s);
        s->unk144 = i;
        s->unk120 = 0;
        s->unk128 = 0;
        s->unk12C = 0;
        s->unk11C = 0;
    }
    func_020202e0(heap);
    func_0201fa50(data_02084300, prio);
}
