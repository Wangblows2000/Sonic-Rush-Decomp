#include <nitro/types.h>

s32 ObjShift_SetFromRef();
s32 Math_LerpFx32();
void Sound_PlaySeqArcEx(void *a, s32 b, s32 c);
void Sound_SetPositional(void *a, void *b, void *c);
void EnemyFlapperGroup_Action_Unk_ov000_022ffef8(u8 *p0);

extern void *gPlayers[];

void EnemyFlapperGroup_State_Unk_ov000_022fff44(u8 *p0) {
    u32 v = *(u32 *)((u8 *)p0 + 0x8);
    s32 t0 = (v & 0xffff) << 8;
    s32 t1 = ((v >> 16) & 0xffff) << 8;

    if (*(s16 *)((u8 *)p0 + 0x30) == 0) {
        if (*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x2) != 0x12) {
            s32 t2 = ObjShift_SetFromRef(*(s16 *)((u8 *)p0 + 0x2e), 0x1000, 0, 4, 0x280, 0x80);
            *(u16 *)((u8 *)p0 + 0x2e) = t2;
        } else {
            s32 t3 = ObjShift_SetFromRef(*(s16 *)((u8 *)p0 + 0x2e), 0x1000, 0, 2, 0x600, 0x100);
            *(u16 *)((u8 *)p0 + 0x2e) = t3;
        }
        *(u32 *)((u8 *)p0 + 0x1c) = *(u32 *)((u8 *)p0 + 0x10);
        *(u32 *)((u8 *)p0 + 0x20) = *(u32 *)((u8 *)p0 + 0x14);
        *(u32 *)((u8 *)p0 + 0x10) = Math_LerpFx32(t0, *(u32 *)((u8 *)p0 + 0x1fc), *(u16 *)((u8 *)p0 + 0x2e));
        *(u32 *)((u8 *)p0 + 0x14) = Math_LerpFx32(t1, *(u32 *)((u8 *)p0 + 0x200), *(u16 *)((u8 *)p0 + 0x2e));
    } else {
        *(s16 *)((u8 *)p0 + 0x30) -= 1;
        if (*(s16 *)((u8 *)p0 + 0x30) == 0) {
            Sound_PlaySeqArcEx(*(void **)((u8 *)p0 + 0x1dc), 0, 0x91);
            Sound_SetPositional(*(void **)((u8 *)p0 + 0x1dc), p0, gPlayers[0]);
        }
    }
    if (*(u32 *)((u8 *)p0 + 0x10) != t0)
        return;
    if (*(u32 *)((u8 *)p0 + 0x14) != t1)
        return;
    if (!(*(u32 *)((u8 *)p0 + 0x1f4) & 1)) {
        *(u16 *)((u8 *)p0 + 0x62) |= 0x80;
        *(u32 *)((u8 *)p0 + 0x1f4) |= 1;
        return;
    }
    if (*(s16 *)((u8 *)p0 + 0x34) != 0) {
        *(s16 *)((u8 *)p0 + 0x34) -= 1;
        return;
    }
    EnemyFlapperGroup_Action_Unk_ov000_022ffef8(p0);
}
