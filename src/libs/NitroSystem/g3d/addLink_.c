#include <nitro/types.h>

typedef struct UnkAnmObj_02021e3c UnkAnmObj_02021e3c;
struct UnkAnmObj_02021e3c {
    u8 unk0[0x10];
    UnkAnmObj_02021e3c *next;
    u8 unk14[4];
    u8 priority;
};

void addLink_(UnkAnmObj_02021e3c **list, UnkAnmObj_02021e3c *obj) {
    UnkAnmObj_02021e3c *head = *list;
    UnkAnmObj_02021e3c *cur;
    UnkAnmObj_02021e3c *p;
    if (head == NULL) {
        *list = obj;
        return;
    }
    cur = head->next;
    if (cur == NULL) {
        if (head->priority > obj->priority) {
            p = obj;
            while (p->next != NULL) p = p->next;
            p->next = head;
            *list = obj;
        } else {
            head->next = obj;
        }
        return;
    }
    while (cur != NULL) {
        if (cur->priority >= obj->priority) {
            p = obj;
            while (p->next != NULL) p = p->next;
            head->next = obj;
            p->next = cur;
            return;
        }
        head = cur;
        cur = cur->next;
    }
    head->next = obj;
}
