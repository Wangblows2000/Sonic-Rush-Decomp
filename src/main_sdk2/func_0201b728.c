#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3d];
    u8 prio;
} UnkSndPlayer_0201b728;

extern u8 data_0208344c[];
extern u8 data_02083458[];

void *NNS_FndGetNextListObject(void *list, void *obj);
void func_0202061c(void *list, void *obj);
void func_0201b7a4(UnkSndPlayer_0201b728 *p);
void func_0201b7e0(UnkSndPlayer_0201b728 *p);

UnkSndPlayer_0201b728 *func_0201b728(s32 prio)
{
    UnkSndPlayer_0201b728 *p = NNS_FndGetNextListObject(data_0208344c, NULL);
    if (p == NULL) {
        p = NNS_FndGetNextListObject(data_02083458, NULL);
        if (prio < p->prio) {
            return NULL;
        }
        func_0201b7a4(p);
    }
    func_0202061c(data_0208344c, p);
    p->prio = prio;
    func_0201b7e0(p);
    return p;
}
