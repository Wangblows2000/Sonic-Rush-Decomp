#include <nitro/types.h>

typedef struct { u8 unk0[0xa]; u16 unkA; } UnkStruct_0206d8bc;
extern u16 gObjRectPrevAtkFlag;
extern u16 gObjRectPrevDefFlag;
extern u8 gObjRectCalledNoHit;
void ObjRect_FuncNoHit(UnkStruct_0206d8bc *a, UnkStruct_0206d8bc *b) { a->unkA = gObjRectPrevAtkFlag; b->unkA = gObjRectPrevDefFlag; gObjRectCalledNoHit = 1; }
