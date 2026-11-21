#include <nitro/types.h>

typedef struct UnkObj_0205caa0 { void (*fn)(void *, struct UnkObj_0205caa0 *); } UnkObj_0205caa0;
void StageTitleCardPlayer_State_BeginZoom(void *a, UnkObj_0205caa0 *o);
void StageTitleCardPlayer_State_ToZoom(void *a, UnkObj_0205caa0 *o) { o->fn = StageTitleCardPlayer_State_BeginZoom; o->fn(a, o); }
