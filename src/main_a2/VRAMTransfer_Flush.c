#include <nitro/types.h>

typedef struct UnkJob_0203383c {
    struct UnkJob_0203383c *next;
    u32 type;
    u32 dstType;
    u32 dst;
    u32 src;
    u32 size;
} UnkJob_0203383c;

extern UnkJob_0203383c *gVRAMTransferHead;
extern UnkJob_0203383c *gVRAMTransferTail;
extern u32 data_0208a754[];

u32 func_020030e4(void);
void func_020034cc(u32 state);
void Mem_Copy(u32 src, u32 dst, u32 size);
void func_02007da4(u32 ch, u32 src, u32 dst, u32 size);
void func_02007d2c(u32 ch, u32 src, u32 dst, u32 size);
u32 VRAMTransfer_GetCompressionType(u32 src);
u32 Compress_GetData(u32 src);
u32 VRAMTransfer_GetDataSize(u32 src);
void func_02008648(u32 src, u32 dst);
void func_02008720(u32 src, u32 dst);
void func_020087f8(u32 src, u32 dst);
void DC_StoreRange(u32 addr, u32 size);
void TransferJob_Free(UnkJob_0203383c *job);

void VRAMTransfer_Flush(void) {
    u32 saved;
    u32 flags = 0;
    while (gVRAMTransferHead != NULL) {
        UnkJob_0203383c *job = gVRAMTransferHead;
        u32 dst = 0;
        switch (job->dstType) {
        case 0:
            dst = job->dst;
            break;
        case 1: {
            u32 a = job->dst;
            u32 page = a >> 17;
            dst = a + data_0208a754[page] - (page << 17);
            if (!(flags & 2)) {
                saved = func_020030e4();
                flags |= 2;
            }
            break;
        }
        }
        switch (job->type) {
        case 0: {
            u32 src = job->src;
            u32 size = job->size;
            if (src >= 0x2000000 && src < 0x2400000 && dst >= 0x2000000 && dst < 0x2400000) {
                Mem_Copy(src, dst, size);
            } else {
                u32 n;
                if (((src | dst) & 3) == 0 && (n = size & ~3) != 0) {
                    func_02007da4(3, src, dst, n);
                    src += n;
                    dst += n;
                    size &= 3;
                }
                if (size != 0) {
                    func_02007d2c(3, src, dst, size);
                }
            }
            break;
        }
        case 1: {
            u32 src = job->src;
            switch (VRAMTransfer_GetCompressionType(src)) {
            case 0x10:
                func_02008648(src, dst);
                break;
            case 0x20:
                func_02008720(src, dst);
                break;
            case 0x30:
                func_020087f8(src, dst);
                break;
            default:
                Mem_Copy(Compress_GetData(src), dst, VRAMTransfer_GetDataSize(src));
                break;
            }
            DC_StoreRange(dst, VRAMTransfer_GetDataSize(src));
            break;
        }
        }
        TransferJob_Free(gVRAMTransferHead);
        gVRAMTransferHead = gVRAMTransferHead->next;
        if (gVRAMTransferHead == NULL) {
            gVRAMTransferTail = NULL;
        }
    }
    if (flags & 2) {
        func_020034cc(saved);
    }
}
