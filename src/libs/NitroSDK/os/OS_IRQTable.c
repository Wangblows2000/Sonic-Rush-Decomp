#include <nitro/types.h>

/* NitroSDK os_irqTable.c: the ARM9 IRQ handler table in DTCM (0x027e0004).
 * One entry per IE bit: VBlank, HBlank, VCount, Timer0-3, Serial, DMA0-3,
 * Keypad, Cartridge, (unused), (unused), IPC sync, IPC send/recv FIFO,
 * card data, card IREQ, geometry FIFO. Only timers and DMAs have internal
 * handlers; every other slot starts as OS_IrqDummy. */
typedef void (*OSIrqFunction)(void);

extern void OS_IrqDummy(void);
extern void OSi_IrqTimer0(void);
extern void OSi_IrqTimer1(void);
extern void OSi_IrqTimer2(void);
extern void OSi_IrqTimer3(void);
extern void OSi_IrqDma0(void);
extern void OSi_IrqDma1(void);
extern void OSi_IrqDma2(void);
extern void OSi_IrqDma3(void);

OSIrqFunction OS_IRQTable[22] = {
    OS_IrqDummy, OS_IrqDummy, OS_IrqDummy,
    OSi_IrqTimer0, OSi_IrqTimer1, OSi_IrqTimer2, OSi_IrqTimer3,
    OS_IrqDummy,
    OSi_IrqDma0, OSi_IrqDma1, OSi_IrqDma2, OSi_IrqDma3,
    OS_IrqDummy, OS_IrqDummy, OS_IrqDummy, OS_IrqDummy, OS_IrqDummy,
    OS_IrqDummy, OS_IrqDummy, OS_IrqDummy, OS_IrqDummy, OS_IrqDummy,
};
