#include <nitro/types.h>

typedef void (*FSOverlayInitFunc)(void);

typedef struct {
    u32 id;
    u8 *ram_address;
    u32 ram_size;
    u32 bss_size;
    FSOverlayInitFunc *sinit_init;
    FSOverlayInitFunc *sinit_init_end;
    u32 file_id;
    u8 compressed[3];
    u8 flag;
} FSOverlayInfo_StartOverlay;

typedef struct {
    u8 digest[20];
} FSOverlayDigest_StartOverlay;

extern FSOverlayDigest_StartOverlay data_0207cfc8[];
/* The ROM pool holds two words for 0x0207cfc8 (digest table end and start,
   two linker symbols at one address); the second is written as data_0207cfc4 + 4. */
extern u8 data_0207cfc4[];

u32 FSi_GetOverlayBinarySize(FSOverlayInfo_StartOverlay *p);
BOOL FSi_CompareDigest(void *digest, void *data, u32 size);
void MI_CpuFill8(void *dst, u8 val, u32 size);
void OS_Terminate(void);
void MIi_UncompressBackward(void *end);
void DC_FlushRange(void *p, u32 size);

void FS_StartOverlay(FSOverlayInfo_StartOverlay *p)
{
    u32 size = FSi_GetOverlayBinarySize(p);

    if (*(u16 *)0x027ffc40 == 2) {
        BOOL ret = FALSE;
        if (p->flag & 2) {
            if (p->id < (u32)((FSOverlayDigest_StartOverlay *)(data_0207cfc4 + 4) - data_0207cfc8))
                ret = FSi_CompareDigest(&data_0207cfc8[p->id], p->ram_address, size);
        }
        if (!ret) {
            MI_CpuFill8(p->ram_address, 0, size);
            OS_Terminate();
            return;
        }
    }
    if (p->flag & 1)
        MIi_UncompressBackward(p->ram_address + size);
    DC_FlushRange(p->ram_address, p->ram_size);
    {
        FSOverlayInitFunc *q = p->sinit_init;
        FSOverlayInitFunc *e = p->sinit_init_end;
        for (; q < e; q++) {
            if (*q)
                (**q)();
        }
    }
}
