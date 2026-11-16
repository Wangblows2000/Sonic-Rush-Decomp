#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u8 unk8;
    u8 pad9;
    u16 unkA;
} UnkEnt_0233deec;

typedef struct {
    u8 pad0[8];
    s32 unk8[2];
    u8 buf[2][0x200];
    UnkEnt_0233deec ent[24];
} UnkStruct_0233deec;

typedef BOOL (*UnkFunc_0233deec)(UnkStruct_0233deec *w, u8 i);

extern UnkFunc_0233deec data_ov000_0236880c[];
void func_ov000_0233c8d4(UnkEnt_0233deec *e, void *buf);
void func_ov000_0233c878(void *buf);
void func_ov000_0233c840(void *buf);

void func_ov000_0233deec(UnkStruct_0233deec *w) {
    u8 i;
    u8 j;
    u8 dirty[2];
    u8 *d;
    UnkEnt_0233deec *e;

    for (i = 0; i < 2; i++) {
        if (data_ov000_0236880c[w->unk8[i]](w, i))
            w->unk8[i] = 0;
    }
    d = (u8 *)dirty;
    ((u8 *)d)[0] = 0;
    ((u8 *)d)[1] = 0;
    for (j = 0; j < 24; j++) {
        e = &w->ent[j];
        e->unkA--;
        if (e->unkA == 0) {
            func_ov000_0233c8d4(e, w->buf[e->unk8]);
            d[e->unk8] = 1;
        }
    }
    if (dirty[0])
        func_ov000_0233c878(w->buf[0]);
    if (dirty[1])
        func_ov000_0233c840(w->buf[1]);
}
