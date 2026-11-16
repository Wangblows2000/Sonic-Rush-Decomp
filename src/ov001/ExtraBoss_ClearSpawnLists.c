#include <nitro/types.h>

typedef struct UnkStruct_02303c54_n { u8 pad[0x28]; struct UnkStruct_02303c54_n *next; } UnkStruct_02303c54_n;
typedef struct { u8 pad[0x98]; UnkStruct_02303c54_n *a; u8 pad2[4]; UnkStruct_02303c54_n *d; u8 pad3[4]; UnkStruct_02303c54_n *b; u8 pad4[4]; UnkStruct_02303c54_n *c; } UnkStruct_0231f548;
extern UnkStruct_0231f548 *data_ov001_0231f548;
void ExtraBoss_Unk_ov001_02303394(void *p);
void ExtraBoss_Unk_ov001_02303244(void *p);
void ExtraBoss_Unk_ov001_0230319c(void *p);
void ExtraBoss_Unk_ov001_023032ec(void *p);
void ExtraBoss_StackPush(void *p);

void ExtraBoss_ClearSpawnLists(void) {
    UnkStruct_02303c54_n *n;
    for (n = data_ov001_0231f548->a; n != NULL; n = n->next) {
        ExtraBoss_Unk_ov001_02303394(n);
        ExtraBoss_StackPush(n);
    }
    for (n = data_ov001_0231f548->b; n != NULL; n = n->next) {
        ExtraBoss_Unk_ov001_02303244(n);
        ExtraBoss_StackPush(n);
    }
    for (n = data_ov001_0231f548->c; n != NULL; n = n->next) {
        ExtraBoss_Unk_ov001_0230319c(n);
        ExtraBoss_StackPush(n);
    }
    for (n = data_ov001_0231f548->d; n != NULL; n = n->next) {
        ExtraBoss_Unk_ov001_023032ec(n);
        ExtraBoss_StackPush(n);
    }
}
