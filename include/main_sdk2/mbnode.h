#ifndef MAIN_SDK2_MBNODE_H
#define MAIN_SDK2_MBNODE_H
#include <nitro/types.h>

typedef struct UnkMbNode {
    u32 id;                  /* 0x00 */
    u32 size;                /* 0x04 */
    u8 name[0x20];           /* 0x08 */
    struct UnkMbNode *next;  /* 0x28 */
    u8 *data;                /* 0x2c */
    u16 unk30;               /* 0x30 */
    u16 unk32;               /* 0x32 */
} UnkMbNode;

UnkMbNode *func_0201558c(void);
void func_0201557c(UnkMbNode *n);
#endif
