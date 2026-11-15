#include <nitro/types.h>

typedef struct UnkStruct_022eb394 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x43c - 0x6c];
    void (*func2)(void);
} UnkStruct_022eb394;

void func_ov000_022ee380(UnkStruct_022eb394 *s);
void func_ov000_022ea798(void);
void func_ov000_022eb364(void);

void func_ov000_022eb394(UnkStruct_022eb394 *s) {
    func_ov000_022ee380(s);
    s->func2 = func_ov000_022ea798;
    s->func = func_ov000_022eb364;
}
