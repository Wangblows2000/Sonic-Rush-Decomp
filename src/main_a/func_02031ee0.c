#include <nitro/types.h>

extern u32 data_02250438;
extern u32 data_02250434;
void func_020356bc(u32 a, u32 b, u32 c);
void func_02035754(void);

void func_02031ee0(u32 a, u32 b, u32 c) {
    u32 head = data_02250438;
    u32 tail = data_02250434;
    data_02250438 = 0;
    data_02250434 = 0;
    func_020356bc(a, b, c);
    func_02035754();
    data_02250438 = head;
    data_02250434 = tail;
}
