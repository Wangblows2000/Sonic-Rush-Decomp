#include <nitro/types.h>
#include "main_a2/task.h"

extern UnkTask *data_022b457c;
extern UnkTask *data_022b4580;
extern UnkTask *data_022b4584;
extern UnkTask *data_022b4588;

void func_020371c8(UnkTask *t);

void func_02037038(u16 lo, u16 hi) {
    UnkTask *t;
    if (lo <= 0xefff) {
        t = data_022b457c->next;
        while (t != data_022b4580 && t->id < lo) {
            t = t->next;
        }
        while (t != data_022b4580 && t->id <= hi) {
            func_020371c8(t);
            t = t->next;
        }
    }
    if (hi >= 0xf000) {
        t = data_022b4584->next;
        while (t != data_022b4588 && t->id < lo) {
            t = t->next;
        }
        while (t != data_022b4588 && t->id <= hi) {
            func_020371c8(t);
            t = t->next;
        }
    }
}
