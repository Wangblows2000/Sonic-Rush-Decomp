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

extern unsigned char gSaveDataBusy[];
extern unsigned char gSystemFlags[];
int TitleDemo_Create();
int TitleDemo_InitDisplay();
int TitleDemo_LoadAssets();
int TitleDemo_InitSprites();
int TitleDemo_InitBackgrounds();
int TitleDemo_LoadArchives();
int TitleDemo_LoadSound();
int Touch_Enable();
int TitleDemo_Action_Scene1();
int TitleDemo_PlayMusic();

#pragma thumb on

void TitleDemo_Start(void)

{
  int iVar1;
  
  iVar1 = TitleDemo_Create();
  TitleDemo_InitDisplay();
  TitleDemo_LoadAssets(iVar1);
  TitleDemo_InitSprites(iVar1);
  TitleDemo_InitBackgrounds(iVar1);
  TitleDemo_LoadArchives(iVar1);
  TitleDemo_LoadSound(iVar1);
  Touch_Enable();
  TitleDemo_Action_Scene1(*(undefined4 *)(iVar1 + 0x48c));
  TitleDemo_PlayMusic(iVar1);
  *((int *)gSaveDataBusy) = 0;
  *((int *)gSystemFlags) = *((int *)gSystemFlags) & 0xffffffef;
  return;
}

#pragma thumb reset
