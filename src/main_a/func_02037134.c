#include <nitro/types.h>

typedef struct UnkNode_02037134 UnkNode_02037134;
struct UnkNode_02037134 { u32 unk0; UnkNode_02037134 *next; };
extern UnkNode_02037134 *data_022b457c;
extern UnkNode_02037134 *data_022b4580;
extern UnkNode_02037134 *data_022b4584;
extern UnkNode_02037134 *data_022b4588;
void func_020371c8(UnkNode_02037134 *n);

void func_02037134(void) {
    UnkNode_02037134 *n;
    for (n = data_022b457c->next; n != data_022b4580; n = n->next) {
        func_020371c8(n);
    }
    for (n = data_022b4584->next; n != data_022b4588; n = n->next) {
        func_020371c8(n);
    }
}
