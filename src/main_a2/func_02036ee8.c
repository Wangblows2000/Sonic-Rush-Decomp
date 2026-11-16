#include <nitro/types.h>
#include "main_a2/task.h"

extern UnkTask *data_022b457c;
extern UnkTask *data_022b4580;
extern UnkTask *data_022b4584;
extern UnkTask *data_022b4588;

void func_02036da8(UnkTask *t);

void func_02036ee8(void) {
    UnkTask *t;
    UnkTask *next;
    t = data_022b457c->next;
    while (t != data_022b4580) {
        next = t->next;
        if (t->flags & 1) {
            func_02036da8(t);
        }
        t = next;
    }
    t = data_022b4584->next;
    while (t != data_022b4588) {
        next = t->next;
        if (t->flags & 1) {
            func_02036da8(t);
        }
        t = next;
    }
}
