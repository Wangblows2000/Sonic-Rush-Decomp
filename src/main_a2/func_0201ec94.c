#include <nitro/types.h>

void func_02005c48(void *q);
void func_02006698(void *m);
void func_02006628(void *m);
void *func_0201f988(void *list);
void func_0201ed04(void *x);
void func_0201f904(void *x);

void func_0201ec94(u8 *p) {
    void *queue;
    void *mutex;
    void *list;
    void *x;
    mutex = p + 0x498;
    list = p + 0x4AC;
    queue = p + 0x494;
    for (;;) {
        func_02005c48(queue);
        for (;;) {
            func_02006698(mutex);
            x = func_0201f988(list);
            if (x == NULL) {
                func_02006628(mutex);
                break;
            }
            func_0201ed04(x);
            func_0201f904(x);
            func_02006628(mutex);
        }
    }
}
