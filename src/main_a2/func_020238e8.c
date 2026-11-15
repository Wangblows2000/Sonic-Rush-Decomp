#include <nitro/types.h>
#include <main_a2/g3drs.h>

typedef struct { fx32 m[4][4]; } UnkMtx44_020238e8;
typedef struct { fx32 x, y, z; } UnkVec_020238e8;
typedef struct { fx32 m[4][3]; } UnkMtx43_020238e8;
typedef struct { u8 unk0[0xFC]; u32 flag; } UnkGlb_020238e8;

extern UnkVec_020238e8 data_02077c48;
extern UnkVec_020238e8 data_02077c54;
extern UnkMtx43_020238e8 data_02077c24;
extern u32 data_02077c1c[];
extern u32 data_02077c18[];
extern UnkGlb_020238e8 data_02084d38;
extern u8 data_02084d84[];

void func_02025268(u32 op, const void *args, u32 n);
void func_02025454(void);
BOOL func_020040e4(UnkMtx44_020238e8 *m);
void *func_02022104(void);
void *func_020220c0(void);
void *func_020221ac(void);
void func_020014b4(const void *src, UnkMtx44_020238e8 *dst);
void func_02001d18(const UnkMtx44_020238e8 *a, const UnkMtx44_020238e8 *b, UnkMtx44_020238e8 *dst);
fx32 VEC_Mag(const void *v);
void func_020024b4(const void *src, void *dst);
void MIi_CpuSend32(const void *src, volatile void *dst, u32 size);

#define REG_GXFIFO (*(vu32 *)0x04000400)

void func_020238e8(UnkRS *rs, u32 opt) {
    int t;
    u32 len;
    UnkVec_020238e8 *trans;
    UnkVec_020238e8 *scale;
    UnkMtx43_020238e8 *mtx;
    BOOL skip;
    len = 2;
    trans = &data_02077c48;
    scale = &data_02077c54;
    mtx = &data_02077c24;
    if (rs->flag & 0x200) {
        if (opt == 0x40 || opt == 0x60) len++;
        if (opt == 0x20 || opt == 0x60) len++;
        rs->c += len;
        return;
    }
    if (opt == 0x40 || opt == 0x60) {
        len++;
        if (!(rs->flag & 0x100)) {
            u32 arg;
            if (opt == 0x40) {
                arg = rs->c[2];
            } else {
                arg = rs->c[3];
            }
            func_02025268(0x14, &arg, 1);
        }
    }
    skip = UnkRS_CbA(rs, 8, &t);
    if (!(rs->flag & 0x100) && !skip) {
        UnkMtx44_020238e8 m;
        func_02025454();
        REG_GXFIFO = 0x151110;
        REG_GXFIFO = 0;
        REG_GXFIFO = 0;
        while (func_020040e4(&m)) {
        }
        if (data_02084d38.flag & 1) {
            UnkMtx44_020238e8 tmp;
            func_020014b4(func_02022104(), &tmp);
            func_02001d18(&m, &tmp, &m);
        } else if (data_02084d38.flag & 2) {
            UnkMtx44_020238e8 tmp;
            func_020014b4(data_02084d84, &tmp);
            func_02001d18(&m, &tmp, &m);
        }
        trans->x = m.m[3][0];
        trans->y = m.m[3][1];
        trans->z = m.m[3][2];
        scale->x = VEC_Mag(m.m[0]);
        scale->y = VEC_Mag(m.m[1]);
        scale->z = VEC_Mag(m.m[2]);
        if (m.m[1][1] != 0 || m.m[1][2] != 0) {
            func_020024b4(m.m[1], mtx->m[1]);
            mtx->m[2][1] = -mtx->m[1][2];
            mtx->m[2][2] = mtx->m[1][1];
        } else {
            func_020024b4(m.m[2], mtx->m[2]);
            mtx->m[1][2] = -mtx->m[2][1];
            mtx->m[1][1] = mtx->m[2][2];
        }
        if (data_02084d38.flag & 1) {
            REG_GXFIFO = 0x171012;
            MIi_CpuSend32(data_02077c1c, &REG_GXFIFO, 8);
            MIi_CpuSend32(func_020220c0(), &REG_GXFIFO, 0x30);
            REG_GXFIFO = 0x1B19;
            MIi_CpuSend32(&data_02077c24, &REG_GXFIFO, 0x3C);
        } else if (data_02084d38.flag & 2) {
            REG_GXFIFO = 0x171012;
            MIi_CpuSend32(data_02077c1c, &REG_GXFIFO, 8);
            MIi_CpuSend32(func_020221ac(), &REG_GXFIFO, 0x30);
            REG_GXFIFO = 0x1B19;
            MIi_CpuSend32(&data_02077c24, &REG_GXFIFO, 0x3C);
        } else {
            MIi_CpuSend32(data_02077c18, &REG_GXFIFO, 0x48);
        }
    }
    if (t == 3) {
        rs->flag &= ~0x40;
        rs->cb[8](rs);
        skip = rs->flag & 0x40;
    } else {
        skip = FALSE;
    }
    if (opt == 0x20 || opt == 0x60) {
        len++;
        if (!skip && !(rs->flag & 0x100)) {
            u32 arg2 = rs->c[2];
            func_02025268(0x13, &arg2, 1);
        }
    }
    rs->c += len;
}
