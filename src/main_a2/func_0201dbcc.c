#include <nitro/types.h>

typedef struct { u32 index; u32 offset; } SNDInstPos;
typedef struct { u8 type; u8 padding; u16 wave[2]; u8 rest[0xA]; } SNDInstData;

SNDInstPos SND_GetFirstInstDataPos(const void *bank);
BOOL func_02009be8(const void *bank, SNDInstData *inst, SNDInstPos *pos);
BOOL func_0201dc9c(void *arc, u32 waveIdx, u32 a, u32 b);

BOOL func_0201dbcc(void *arc, const void *bank, u32 waveArc, u32 a, u32 b) {
    SNDInstData inst;
    SNDInstPos pos = SND_GetFirstInstDataPos(bank);
    if (bank == NULL) return FALSE;
    while (func_02009be8(bank, &inst, &pos)) {
        if (inst.type == 1 && waveArc == inst.wave[1]) {
            if (!func_0201dc9c(arc, inst.wave[0], a, b)) return FALSE;
        }
    }
    return TRUE;
}
