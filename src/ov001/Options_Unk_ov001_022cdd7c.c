#include <nitro/types.h>

typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; void *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void Options_Unk_ov001_022cb96c(void *p);
void Options_State_Unk_ov001_022cccbc(void);

#pragma thumb on
void Options_Unk_ov001_022cdd7c(void) {
    Options_Unk_ov001_022cb96c(gTaskCurrent->p);
    gTaskCurrent->fn = Options_State_Unk_ov001_022cccbc;
}
#pragma thumb reset
