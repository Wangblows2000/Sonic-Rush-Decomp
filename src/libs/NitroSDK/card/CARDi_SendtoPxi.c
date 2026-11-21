/* ghidra_tier.py: mechanically fixed Ghidra draft, verified by match.py. */
typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined3;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned char bool;
typedef unsigned short ushort;
typedef unsigned short word;
typedef unsigned int uint;
typedef unsigned int dword;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;
typedef long long longlong;
typedef int code();
#define true 1
#define false 0

int PXI_SendWordByFifo();
int WaitByLoop();

void CARDi_SendtoPxi(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = PXI_SendWordByFifo(0xe,param_1,0);
  if (iVar1 == 0) {
    return;
  }
  do {
    WaitByLoop(param_2);
    iVar1 = PXI_SendWordByFifo(0xe,param_1,0);
  } while (iVar1 != 0);
  return;
}
