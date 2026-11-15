#include <nitro/types.h>

typedef struct { u8 cat; u8 pad; u16 type; } UnkAnmHeader_02021f60;
typedef struct {
    fx32 frame;
    fx32 ratio;
    UnkAnmHeader_02021f60 *resAnm;
    u32 unkC;
    void *next;
    void *resTex;
    u8 priority;
} UnkAnmObj_02021f60;
typedef void (*UnkInitFn_02021f60)(UnkAnmObj_02021f60 *obj, UnkAnmHeader_02021f60 *res, void *mdl);
typedef struct { u8 cat; u8 pad; u16 type; UnkInitFn_02021f60 fn; } UnkEntry_02021f60;
typedef struct { UnkInitFn_02021f60 fn; u32 next; } UnkFnEntry_02021f60;

extern u32 data_02077ac4;
extern UnkEntry_02021f60 data_02077ae8[];
extern UnkFnEntry_02021f60 data_02077aec[];

void func_02021f60(UnkAnmObj_02021f60 *obj, UnkAnmHeader_02021f60 *res, void *mdl, void *tex) {
    u32 i;
    obj->frame = 0;
    obj->resAnm = res;
    obj->next = NULL;
    obj->priority = 0x7F;
    obj->ratio = 0x1000;
    obj->resTex = tex;
    for (i = 0; i < data_02077ac4; i++) {
        if (res->cat == data_02077ae8[i].cat && res->type == data_02077ae8[i].type) {
            data_02077aec[i].fn(obj, res, mdl);
            return;
        }
    }
}
