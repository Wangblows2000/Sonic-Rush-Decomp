#include <nitro/types.h>

typedef struct { u32 type; u32 pad; u16 a; u8 pad2[6]; u32 b; u8 rest[0x9c - 0x14]; } UnkStruct_02015264;
void WBT_QueueCommand(UnkStruct_02015264 *p);
void WBT_RequestSync(u16 a, u32 b) { UnkStruct_02015264 s; s.a = a; s.type = 2; s.b = b; WBT_QueueCommand(&s); }
