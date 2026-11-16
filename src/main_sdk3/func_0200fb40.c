#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u32 src;     /* 0x0c */
    u32 dst;     /* 0x10 */
    u32 len;     /* 0x14 */
} UnkCardCmd_0200fb40;

typedef struct {
    UnkCardCmd_0200fb40 *cmd;   /* 0x00 */
    u8 pad4[0x14];
    vu32 src;                   /* 0x18 */
    vu32 dst;                   /* 0x1c */
    vu32 len;                   /* 0x20 */
    u8 pad24[0xe0 - 0x24];
    u8 backup_cache_page_buf[0x100];   /* 0xe0 */
} UnkCardCommon_0200fb40;

extern void MI_CpuCopy8(const void *src, void *dst, u32 size);
extern void DC_FlushRange(const void *p, u32 size);
extern void DC_WaitWriteBufferEmpty(void);
extern BOOL func_020103f0(UnkCardCommon_0200fb40 *p, u32 req, u32 retry);
extern void func_0200fcb8(UnkCardCommon_0200fb40 *p);

void func_0200fb40(UnkCardCommon_0200fb40 *p)
{
    u32 len;
    void *buf;
    buf = p->backup_cache_page_buf;
    do {
        len = p->len;
        if (len > 0x100) {
            len = 0x100;
        }
        MI_CpuCopy8((const void *)p->src, buf, len);
        DC_FlushRange(buf, len);
        DC_WaitWriteBufferEmpty();
        p->cmd->src = (u32)buf;
        p->cmd->dst = p->dst;
        p->cmd->len = len;
        if (!func_020103f0(p, 8, 10)) {
            break;
        }
        p->src += len;
        p->dst = p->dst + len;
        p->len = p->len - len;
    } while (p->len != 0);
    func_0200fcb8(p);
}
