// flags: -O1,p -proc arm946e -enum int -char signed -gccext,on -fp soft -interworking -sym on -gccinc -nolink -msgstyle gcc -w off -lang c99 -Cpp_exceptions off
#include <nitro/types.h>

extern u32 func_02009158(u32 a);
extern BOOL func_02009520(u32 a);
extern void func_02008fe8(void);

u32 func_020091ec(u32 a)
{
    u32 r;
    u32 block;
    if ((r = func_02009158(a)) != 0) {
        return r;
    }
    block = 0;
    while (func_02009520(block) != 0) {
    }
    if ((r = func_02009158(a)) != 0) {
        return r;
    }
    func_02008fe8();
    if ((r = func_02009158(a)) != 0) {
        return r;
    }
    block = 1;
    do {
        func_02009520(block);
    } while ((r = func_02009158(a)) == 0);
    return r;
}
