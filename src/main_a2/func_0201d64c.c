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

BOOL FS_SeekFile(void *f, u32 pos, u32 whence);
s32 FS_ReadFile(void *f, void *buf, u32 size);
void *NNS_SndHeapAlloc(void *heap, u32 size, void *cb, void *arg, u32 c);
void func_0201d0d0(void);
void func_0201d0c4(void);
void func_0201d0b8(void);

BOOL func_0201d64c(UnkStruct_0201d64c *arc, void *heap, BOOL loadSymb) {
    u32 n;
    if (!FS_SeekFile(arc->file, 0, 0)) return FALSE;
    if (FS_ReadFile(arc->file, arc, 0x30) != 0x30) return FALSE;
    if (heap != NULL) {
        arc->info = NNS_SndHeapAlloc(heap, arc->infoSize, func_0201d0d0, arc, 0);
        if (arc->info == NULL) return FALSE;
        if (!FS_SeekFile(arc->file, arc->infoOff, 0)) return FALSE;
        n = FS_ReadFile(arc->file, arc->info, arc->infoSize);
        if (n != arc->infoSize) return FALSE;
        arc->fat = NNS_SndHeapAlloc(heap, arc->fatSize, func_0201d0c4, arc, 0);
        if (arc->fat == NULL) return FALSE;
        if (!FS_SeekFile(arc->file, arc->fatOff, 0)) return FALSE;
        n = FS_ReadFile(arc->file, arc->fat, arc->fatSize);
        if (n != arc->fatSize) return FALSE;
        if (loadSymb && arc->symbSize != 0) {
            arc->symb = NNS_SndHeapAlloc(heap, arc->symbSize, func_0201d0b8, arc, 0);
            if (arc->symb == NULL) return FALSE;
            if (!FS_SeekFile(arc->file, arc->symbOff, 0)) return FALSE;
            n = FS_ReadFile(arc->file, arc->symb, arc->symbSize);
        if (n != arc->symbSize) return FALSE;
        }
    }
    return TRUE;
}
