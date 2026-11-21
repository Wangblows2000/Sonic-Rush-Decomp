#include <nitro/types.h>
#include <NitroSystem/strm.h>

typedef struct { u8 unk0[0x30]; } UnkNode_020203bc;

extern u32 data_02083f44;
extern u8 data_02083f50[];
extern UnkNode_020203bc data_02083f70[];
extern u8 data_02083f5c[];
extern u8 *data_02083f4c;
extern u8 data_02084100[];
extern UnkStrm data_020847b8[];
extern u8 data_02084300[];

BOOL NNS_SndArcStrmSetupPlayer(void *heap);
void NNS_FndInitList(void *list, u16 offset);
void NNS_FndAppendListObject(void *list, void *obj);
void OS_InitMutex(void *m);
void FS_InitFile(void *file);
void NNS_SndStrmInit(UnkStrm *s);
void CreateThread(u8 *p, u32 prio);

void NNS_SndArcStrmInit(u32 prio, void *heap) {
    s32 i;
    UnkStrm *s;
    UnkNode_020203bc *n;
    if (data_02083f44 != 0) {
        NNS_SndArcStrmSetupPlayer(heap);
        return;
    }
    data_02083f44 = 1;
    NNS_FndInitList(data_02083f50, 0);
    n = data_02083f70;
    for (i = 0; i < 8; i++, n++) {
        NNS_FndAppendListObject(data_02083f50, n);
    }
    OS_InitMutex(data_02083f5c);
    data_02083f4c = data_02084100;
    s = data_020847b8;
    for (i = 0; i < 4; i++, s++) {
        s->unk10C_0 = 0;
        FS_InitFile(s->file);
        NNS_SndStrmInit(s);
        s->unk144 = i;
        s->unk120 = 0;
        s->unk128 = 0;
        s->unk12C = 0;
        s->unk11C = 0;
    }
    NNS_SndArcStrmSetupPlayer(heap);
    CreateThread(data_02084300, prio);
}
