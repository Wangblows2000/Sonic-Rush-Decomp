#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *data_022b4574;
void func_0204bf54(void);
void func_0204b3b8(void);
void func_0204b414(void);
void func_02033754(void);
void func_02035cb4(void);
void func_02035368(void);
void func_0204bbe4(void);
void func_0204bc4c(void) { func_0204bf54(); func_0204b3b8(); func_0204b414(); func_02033754(); func_02035cb4(); func_02035368(); data_022b4574->fn = func_0204bbe4; }
