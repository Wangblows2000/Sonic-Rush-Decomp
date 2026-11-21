#include <nitro/types.h>

typedef struct {
    u32 result;     /* 0x00 */
    u32 type;       /* 0x04 */
    u8 pad8[0x10];
    struct {
    u32 totalSize;  /* 0x18 */
    u32 sectSize;   /* 0x1c */
    u32 pageSize;   /* 0x20 */
    u32 addrWidth;  /* 0x24 */
    u32 programPage; /* 0x28 */
    u32 writePage;  /* 0x2c */
    u32 writePageTotal; /* 0x30 */
    u32 eraseChip;  /* 0x34 */
    u32 eraseChipTotal; /* 0x38 */
    u32 eraseSector; /* 0x3c */
    } spec;
} UnkCard_0200f6e0;

extern UnkCard_0200f6e0 *data_0207f2c0;

void MI_CpuFill8(void *dest, u8 data, u32 size);

void CARDi_IdentifyBackupCore(u32 type)
{
    {
        UnkCard_0200f6e0 *const p = data_0207f2c0;

        MI_CpuFill8(&p->spec, 0, sizeof(p->spec));
        p->type = type;
        if (type != 0) {
            const u32 size = (u32)(1 << (((s32)type >> 8) & 0xff));
            const int device = (type & 0xff);

            p->spec.totalSize = size;

            if (device == 1) {
                switch (size) {
                default:
                    goto invalid_type;
                case 0x200:
                    p->spec.pageSize = 0x10;
                    p->spec.addrWidth = 1;
                    p->spec.programPage = 0x4f;
                    break;
                case 0x2000:
                    p->spec.pageSize = 0x20;
                    p->spec.addrWidth = 2;
                    p->spec.programPage = 0x4f;
                    break;
                case 0x10000:
                    p->spec.pageSize = 0x80;
                    p->spec.addrWidth = 2;
                    p->spec.programPage = 0x9e;
                    break;
                }
            } else if (device == 2) {
                switch (size) {
                default:
                    goto invalid_type;
                case 0x40000:
                    p->spec.writePage = 0x18b;
                    p->spec.writePageTotal = 0x127f;
                    p->spec.eraseSector = 0x127f;
                    p->spec.eraseChipTotal = 0x13435;
                    p->spec.eraseChip = 0x13435;
                    break;
                case 0x80000:
                case 0x100000:
                    p->spec.writePage = 0x18b;
                    p->spec.writePageTotal = 0;
                    p->spec.eraseSector = 0x127f;
                    p->spec.eraseChipTotal = 0x13435;
                    break;
                }
                p->spec.sectSize = 0x10000;
                p->spec.pageSize = 0x100;
                p->spec.addrWidth = 3;
                p->spec.programPage = 0x4f;
            } else if (device == 3) {
                switch (size) {
                default:
                    goto invalid_type;
                case 0x2000:
                case 0x8000:
                    break;
                }
                p->spec.pageSize = size;
                p->spec.addrWidth = 2;
            } else {
            invalid_type:
                p->type = 0;
                p->spec.totalSize = 0;
                data_0207f2c0->result = 3;
                return;
            }
        }
    }
}
