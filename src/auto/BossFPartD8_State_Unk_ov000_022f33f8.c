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

extern unsigned char data_ov000_0236a48c[];
int BossFPartD8_State_Unk_ov000_022f33d0();
int func_ov000_022f4ce4();
int Sound_PlayVoice();

void BossFPartD8_State_Unk_ov000_022f33f8(int param_1)

{
  int iVar1;
  
  func_ov000_022f4ce4(param_1,0x11,0x1000,0);
  iVar1 = ((int)data_ov000_0236a48c);
  *(undefined4 *)(((int)data_ov000_0236a48c) + 0x78) = 0x12c000;
  *(undefined2 *)(iVar1 + 0x80) = 0x40;
  *(undefined4 *)(iVar1 + 0x84) = 0xfffb0000;
  *(undefined2 *)(iVar1 + 0x8c) = 0x40;
  Sound_PlayVoice(0,0,0xd2);
  *(undefined4 *)(param_1 + 0x68) = ((int)BossFPartD8_State_Unk_ov000_022f33d0);
  return;
}
