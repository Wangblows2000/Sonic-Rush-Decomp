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

extern unsigned char data_022c14a0[];
int OS_IsThreadTerminated();
int OS_WakeupThreadDirect();
int OS_JoinThread();

void WFSi_EndTaskThread(void)

{
  int iVar1;
  int iVar2;
  
  iVar2 = ((int)data_022c14a0);
  iVar1 = OS_IsThreadTerminated(((int)data_022c14a0) + 0x400);
  if (iVar1 != 0) {
    return;
  }
  iVar2 = iVar2 + 0x400;
  do {
    OS_WakeupThreadDirect(iVar2);
    OS_JoinThread(iVar2);
    iVar1 = OS_IsThreadTerminated(iVar2);
  } while (iVar1 == 0);
  return;
}
