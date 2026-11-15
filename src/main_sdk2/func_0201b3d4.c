#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    void *arg;
    void *next;
} UnkPMCallback;

extern u32 data_0208104c;
extern UnkPMCallback data_02081050;
extern UnkPMCallback data_0208105c;
extern s8 data_02081040;
extern u32 data_02081044;

void func_02008efc(void);
void func_0201b1a0(void *arg);
void func_0201b194(void *arg);
void func_0200de7c(UnkPMCallback *cb);
void func_0200de64(UnkPMCallback *cb);
void func_0201b47c(void);
void func_0201cee4(void);
void func_0201bc84(void);

static inline void func_0201b3d4_SetCb(UnkPMCallback *cb, void (*func)(void *), void *arg)
{
    cb->func = func;
    cb->arg = arg;
}

void func_0201b3d4(void)
{
    if (data_0208104c != 0) {
        return;
    }
    data_0208104c = 1;
    func_02008efc();
    func_0201b3d4_SetCb(&data_02081050, func_0201b1a0, NULL);
    func_0201b3d4_SetCb(&data_0208105c, func_0201b194, NULL);
    func_0200de7c(&data_02081050);
    func_0200de64(&data_0208105c);
    func_0201b47c();
    func_0201cee4();
    func_0201bc84();
    data_02081040 = -1;
    data_02081044 = 1;
}
