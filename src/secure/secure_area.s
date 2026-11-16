@ ARM9 secure area 0x02000000-0x02000800 (one translation unit).
@ The BIOS SWI wrapper stubs are written out below at their exact offsets (9 of
@ them start on halfword boundaries, which per-function TUs under the LCF's
@ ALIGNALL(4) cannot reproduce). The bytes in between are the secure-area
@ header/filler; they are NOT committed: .incbin pulls them at build time from
@ the user's own extracted binary (extract/eur/arm9/arm9.bin), exactly like the
@ dsd gap objects. Assembled with GNU as (configure.py rule `gas`), which also
@ emits the $t/$d mapping symbols objdiff needs.
	.syntax unified
	.cpu arm946e-s
	.text

	.incbin "extract/eur/arm9/arm9.bin", 0x0, 0x8c
	.thumb
	.global WaitByLoop
	.type WaitByLoop, %function
WaitByLoop:
	svc 3
	bx lr
	.size WaitByLoop, .-WaitByLoop
	.incbin "extract/eur/arm9/arm9.bin", 0x90, 0x50
	.thumb
	.global IsDebugger
	.type IsDebugger, %function
IsDebugger:
	svc 15
	bx lr
	.size IsDebugger, .-IsDebugger
	.incbin "extract/eur/arm9/arm9.bin", 0xe4, 0x72
	.thumb
	.global CpuFastSet
	.type CpuFastSet, %function
CpuFastSet:
	svc 12
	bx lr
	.size CpuFastSet, .-CpuFastSet
	.incbin "extract/eur/arm9/arm9.bin", 0x15a, 0x64
	.thumb
	.global LZ77UnCompReadNormalWrite8bit
	.type LZ77UnCompReadNormalWrite8bit, %function
LZ77UnCompReadNormalWrite8bit:
	svc 17
	bx lr
	.size LZ77UnCompReadNormalWrite8bit, .-LZ77UnCompReadNormalWrite8bit
	.incbin "extract/eur/arm9/arm9.bin", 0x1c2, 0x7e
	.thumb
	.global VBlankIntrWait
	.type VBlankIntrWait, %function
VBlankIntrWait:
	movs r2, #0
	svc 5
	bx lr
	.size VBlankIntrWait, .-VBlankIntrWait
	.incbin "extract/eur/arm9/arm9.bin", 0x246, 0x54
	.thumb
	.global CpuSet
	.type CpuSet, %function
CpuSet:
	svc 11
	bx lr
	.size CpuSet, .-CpuSet
	.incbin "extract/eur/arm9/arm9.bin", 0x29e, 0x66
	.thumb
	.global BitUnPack
	.type BitUnPack, %function
BitUnPack:
	svc 16
	bx lr
	.size BitUnPack, .-BitUnPack
	.incbin "extract/eur/arm9/arm9.bin", 0x308, 0x52
	.thumb
	.global HuffUnCompReadByCallback
	.type HuffUnCompReadByCallback, %function
HuffUnCompReadByCallback:
	svc 19
	bx lr
	.size HuffUnCompReadByCallback, .-HuffUnCompReadByCallback
	.incbin "extract/eur/arm9/arm9.bin", 0x35e, 0x74
	.thumb
	.global Mod
	.type Mod, %function
Mod:
	svc 9
	adds r0, r1, #0
	bx lr
	.size Mod, .-Mod
	.incbin "extract/eur/arm9/arm9.bin", 0x3d8, 0x66
	.thumb
	.global Halt
	.type Halt, %function
Halt:
	svc 6
	bx lr
	.size Halt, .-Halt
	.incbin "extract/eur/arm9/arm9.bin", 0x442, 0x62
	.thumb
	.global IntrWait
	.type IntrWait, %function
IntrWait:
	svc 4
	bx lr
	.size IntrWait, .-IntrWait
	.incbin "extract/eur/arm9/arm9.bin", 0x4a8, 0x6a
	.thumb
	.global Sqrt
	.type Sqrt, %function
Sqrt:
	svc 13
	bx lr
	.size Sqrt, .-Sqrt
	.incbin "extract/eur/arm9/arm9.bin", 0x516, 0x58
	.thumb
	.global SoftReset
	.type SoftReset, %function
SoftReset:
	svc 0
	bx lr
	.size SoftReset, .-SoftReset
	.incbin "extract/eur/arm9/arm9.bin", 0x572, 0x6a
	.thumb
	.global LZ77UnCompReadByCallbackWrite16bit
	.type LZ77UnCompReadByCallbackWrite16bit, %function
LZ77UnCompReadByCallbackWrite16bit:
	svc 18
	bx lr
	.size LZ77UnCompReadByCallbackWrite16bit, .-LZ77UnCompReadByCallbackWrite16bit
	.incbin "extract/eur/arm9/arm9.bin", 0x5e0, 0x64
	.thumb
	.global Div
	.type Div, %function
Div:
	svc 9
	bx lr
	.size Div, .-Div
	.incbin "extract/eur/arm9/arm9.bin", 0x648, 0x64
	.thumb
	.global RLUnCompReadNormalWrite8bit
	.type RLUnCompReadNormalWrite8bit, %function
RLUnCompReadNormalWrite8bit:
	svc 20
	bx lr
	.size RLUnCompReadNormalWrite8bit, .-RLUnCompReadNormalWrite8bit
	.incbin "extract/eur/arm9/arm9.bin", 0x6b0, 0x5a
	.thumb
	.global RLUnCompReadByCallbackWrite16bit
	.type RLUnCompReadByCallbackWrite16bit, %function
RLUnCompReadByCallbackWrite16bit:
	svc 21
	bx lr
	.size RLUnCompReadByCallbackWrite16bit, .-RLUnCompReadByCallbackWrite16bit
	.incbin "extract/eur/arm9/arm9.bin", 0x70e, 0x76
	.thumb
	.global GetCRC16
	.type GetCRC16, %function
GetCRC16:
	svc 14
	bx lr
	.size GetCRC16, .-GetCRC16
	.incbin "extract/eur/arm9/arm9.bin", 0x788, 0x78
