#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3d];
    u8 prio;
} UnkSndPlayer_0201b7e0;

extern u8 data_02083458[];

void *NNS_FndGetNextListObject(void *list, void *obj);
void func_02020688(void *list, void *before, void *obj);

void func_0201b7e0(UnkSndPlayer_0201b7e0 *p)
{
    UnkSndPlayer_0201b7e0 *it = NNS_FndGetNextListObject(data_02083458, NULL);
    while (it != NULL) {
        if (p->prio < it->prio) {
            break;
        }
        it = NNS_FndGetNextListObject(data_02083458, it);
    }
    func_02020688(data_02083458, it, p);
}
