#include <nitro/types.h>

typedef struct {
    u8 unk0[0x10];
    u32 symbOff;
    u32 symbSize;
    u32 infoOff;
    u32 infoSize;
    u32 fatOff;
    u32 fatSize;
    u8 unk28[0xC];
    u8 file[0x4C];
    void *fat;
    void *symb;
    void *info;
} UnkStruct_0201d64c;

BOOL func_0200b930(void *f, u32 pos, u32 whence);
s32 func_0200b99c(void *f, void *buf, u32 size);
void *func_0201d9e4(void *heap, u32 size, void *cb, void *arg, u32 c);
void func_0201d0d0(void);
void func_0201d0c4(void);
void func_0201d0b8(void);

BOOL func_0201d64c(UnkStruct_0201d64c *arc, void *heap, BOOL loadSymb) {
    u32 n;
    if (!func_0200b930(arc->file, 0, 0)) return FALSE;
    if (func_0200b99c(arc->file, arc, 0x30) != 0x30) return FALSE;
    if (heap != NULL) {
        arc->info = func_0201d9e4(heap, arc->infoSize, func_0201d0d0, arc, 0);
        if (arc->info == NULL) return FALSE;
        if (!func_0200b930(arc->file, arc->infoOff, 0)) return FALSE;
        n = func_0200b99c(arc->file, arc->info, arc->infoSize);
        if (n != arc->infoSize) return FALSE;
        arc->fat = func_0201d9e4(heap, arc->fatSize, func_0201d0c4, arc, 0);
        if (arc->fat == NULL) return FALSE;
        if (!func_0200b930(arc->file, arc->fatOff, 0)) return FALSE;
        n = func_0200b99c(arc->file, arc->fat, arc->fatSize);
        if (n != arc->fatSize) return FALSE;
        if (loadSymb && arc->symbSize != 0) {
            arc->symb = func_0201d9e4(heap, arc->symbSize, func_0201d0b8, arc, 0);
            if (arc->symb == NULL) return FALSE;
            if (!func_0200b930(arc->file, arc->symbOff, 0)) return FALSE;
            n = func_0200b99c(arc->file, arc->symb, arc->symbSize);
        if (n != arc->symbSize) return FALSE;
        }
    }
    return TRUE;
}
