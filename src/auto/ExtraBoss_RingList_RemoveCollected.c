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

extern unsigned char gExtraBossRingWork[];

void ExtraBoss_RingList_RemoveCollected(int param_1)

{
  if (*(int *)(param_1 + 0x24) == 0) {
    *(undefined4 *)(*((int *)gExtraBossRingWork) + 0xa0) = *(undefined4 *)(param_1 + 0x28);
  }
  else {
    *(undefined4 *)(*(int *)(param_1 + 0x24) + 0x28) = *(undefined4 *)(param_1 + 0x28);
  }
  if (*(int *)(param_1 + 0x28) == 0) {
    *(undefined4 *)(*((int *)gExtraBossRingWork) + 0xa4) = *(undefined4 *)(param_1 + 0x24);
  }
  else {
    *(undefined4 *)(*(int *)(param_1 + 0x28) + 0x24) = *(undefined4 *)(param_1 + 0x24);
  }
  return;
}
