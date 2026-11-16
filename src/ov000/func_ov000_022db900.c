#include <nitro/types.h>

typedef struct UnkStruct_022db900 {
    u8 pad[0xbc0];
    s32 unkBC0;
} UnkStruct_022db900;

s32 func_ov000_022db934(void);
void *_fflt(void *p);
void *_fmul(void *p, s32 a);
s32 _ffix(void *p);

void func_ov000_022db900(UnkStruct_022db900 *s, void *p) {
    s32 v = func_ov000_022db934();
    s->unkBC0 = _ffix(_fmul(_fflt(p), v));
}
