// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_0206e470;
extern void StageObj_GetMapProbeFlags(void);
extern void func_02066010(void);
extern void func_020660b0(void);
extern void MapCollision_ProbeDistance(void);
extern void func_02067670(void);
extern void ObjAngle_MoveToward8(void);

asm void StageObj_CollideMapVertical(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x6c
    mov r1, #0
    mov r9, r0
    strb r1, [sp, #0x67]
    ldr r1, [r9, #0x80]
    str r1, [sp, #0x18]
    bl StageObj_GetMapProbeFlags
    ldr r3, [r9, #0x10]
    ldr r2, [r9, #0x14]
    ldrb r1, [r9, #1]
    mov r10, r0
    mov r7, r3, asr #8
    strb r1, [sp, #0x65]
    ldrb r0, [sp, #0x65]
    mov r6, r2, asr #8
    strb r0, [sp, #0x64]
    ldr r0, [r9, #0x58]
    ands r1, r0, #0x10
    movne r1, #0
    strne r1, [sp, #0x14]
    bne _70
    ldrb r1, [r9, #1]
    add r1, r1, #0x20
    and r1, r1, #0xc0
    mov r1, r1, asr #6
    and r1, r1, #0xff
    str r1, [sp, #0x14]
_70:
    ldrb r1, [r9, #5]
    cmp r1, #0
    beq _a4
    add r1, r1, #0x20
    and r1, r1, #0xc0
    mov r1, r1, asr #6
    and r2, r1, #0xff
    ldr r1, [sp, #0x14]
    add r1, r1, r2
    and r1, r1, #0xff
    and r1, r1, #3
    and r1, r1, #0xff
    str r1, [sp, #0x14]
_a4:
    ldr r1, [sp, #0x14]
    cmp r1, #3
    addls pc, pc, r1, lsl #2
    b _c4
    b _c4
    b _118
    b _19c
    b _210
_c4:
    ldrsb r3, [r9, #0x66]
    ldrsb r1, [r9, #0x67]
    ldrsb r2, [r9, #0x64]
    add r3, r3, #2
    str r1, [sp, #0x20]
    sub r1, r2, #2
    mov r2, r3, lsl #0x18
    mov r3, r1, lsl #0x18
    ldr r1, [sp, #0x20]
    mov r11, r2, asr #0x18
    str r1, [sp, #0x24]
    mov r1, r3, asr #0x18
    str r1, [sp, #0x1c]
    ldrsh r1, [r9, #0x28]
    mov r8, #0
    mvn r4, #4
    str r1, [sp, #0xc]
    ldrsh r1, [r9, #0x2a]
    mov r5, #2
    str r1, [sp, #0x10]
    b _270
_118:
    ldrsb r1, [r9, #0x67]
    ldrsb r4, [r9, #0x64]
    ldrsb r2, [r9, #0x66]
    rsb r3, r1, #0
    mov r3, r3, lsl #0x18
    sub r5, r4, #2
    mov r11, r3, asr #0x18
    mov r3, r5, lsl #0x18
    add r4, r2, #2
    mov r3, r3, asr #0x18
    cmp r2, #0
    rsblt r2, r2, #0
    sub r1, r1, r2
    mov r1, r1, lsl #0x18
    mov r4, r4, lsl #0x18
    str r3, [sp, #0x20]
    mov r3, r4, asr #0x18
    str r3, [sp, #0x24]
    ldrsh r3, [r9, #0x28]
    mov r8, r1, asr #0x18
    ldrsh r2, [r9, #0x2a]
    rsb r1, r3, #0
    mov r1, r1, lsl #0x10
    mov r1, r1, asr #0x10
    str r1, [sp, #0x10]
    rsb r1, r2, #0
    mov r1, r1, lsl #0x10
    mov r1, r1, asr #0x10
    str r11, [sp, #0x1c]
    str r1, [sp, #0xc]
    mov r4, #0
    mov r5, #1
    b _270
_19c:
    ldrsb r1, [r9, #0x67]
    ldrsb r2, [r9, #0x64]
    ldrsh r3, [r9, #0x28]
    ldrsh r4, [r9, #0x2a]
    rsb r5, r1, #0
    ldrsb r8, [r9, #0x66]
    sub r2, r2, #2
    rsb r3, r3, #0
    add r1, r8, #2
    mov r1, r1, lsl #0x18
    rsb r4, r4, #0
    mov r5, r5, lsl #0x18
    mov r5, r5, asr #0x18
    mov r2, r2, lsl #0x18
    mov r11, r1, asr #0x18
    mov r1, r2, asr #0x18
    mov r3, r3, lsl #0x10
    str r1, [sp, #0x1c]
    mov r1, r3, asr #0x10
    mov r4, r4, lsl #0x10
    str r1, [sp, #0xc]
    mov r1, r4, asr #0x10
    str r5, [sp, #0x20]
    str r5, [sp, #0x24]
    str r1, [sp, #0x10]
    mov r8, #0
    mov r4, #5
    mov r5, #3
    b _270
_210:
    ldrsb r1, [r9, #0x64]
    ldrsb r2, [r9, #0x66]
    ldrsb r11, [r9, #0x67]
    sub r4, r1, #2
    add r3, r2, #2
    mov r2, r4, lsl #0x18
    cmp r1, #0
    rsblt r1, r1, #0
    mov r2, r2, asr #0x18
    sub r1, r11, r1
    mov r3, r3, lsl #0x18
    str r2, [sp, #0x20]
    mov r2, r3, asr #0x18
    mov r4, #0
    rsb r1, r1, #0
    str r2, [sp, #0x24]
    mov r2, r1, lsl #0x18
    ldrsh r1, [r9, #0x28]
    str r11, [sp, #0x1c]
    mov r8, r2, asr #0x18
    str r1, [sp, #0x10]
    ldrsh r1, [r9, #0x2a]
    mov r5, r4
    str r1, [sp, #0xc]
_270:
    ands r0, r0, #0x400000
    bne _324
    bic r0, r10, #0x80
    str r0, [sp, #0x48]
    ldr r2, [sp, #0x20]
    ldr r3, [sp, #0x48]
    add r1, r11, r8
    add r2, r2, r4
    str r5, [sp]
    mov r0, #0
    str r0, [sp, #4]
    str r0, [sp, #8]
    mov r0, r9
    add r1, r7, r1
    add r2, r6, r2
    and r3, r3, #0xff
    bl func_02066010
    ldr r2, [sp, #0x24]
    ldr r1, [sp, #0x1c]
    add r2, r2, r4
    mov r0, r0, lsl #0x18
    ldr r3, [sp, #0x48]
    mov r4, r0, asr #0x18
    add r1, r1, r8
    str r5, [sp]
    mov r0, #0
    str r0, [sp, #4]
    str r0, [sp, #8]
    add r1, r7, r1
    add r2, r6, r2
    and r3, r3, #0xff
    mov r0, r9
    bl func_02066010
    mov r0, r0, lsl #0x18
    mov r0, r0, asr #0x18
    cmp r4, r0
    movge r4, r0
    cmp r4, #0
    ldrge r0, [r9, #0x58]
    orrge r0, r0, #0x100000
    strge r0, [r9, #0x58]
    ldrlt r0, [r9, #0x58]
    biclt r0, r0, #0x100000
    strlt r0, [r9, #0x58]
    b _35c
_324:
    cmp r5, #3
    beq _340
    cmp r5, #2
    beq _340
    ldrh r0, [r9, #0x60]
    ands r0, r0, #8
    beq _350
_340:
    ldr r0, [r9, #0x58]
    orr r0, r0, #0x100000
    str r0, [r9, #0x58]
    b _35c
_350:
    ldr r0, [r9, #0x58]
    bic r0, r0, #0x100000
    str r0, [r9, #0x58]
_35c:
    ldr r0, [r9, #0x58]
    ldr r2, [sp, #0x20]
    ands r0, r0, #0x100000
    orreq r0, r10, #0x80
    andeq r10, r0, #0xff
    ldr r0, [sp, #0x18]
    add r1, sp, #0x67
    str r0, [r9, #0x80]
    add r0, sp, #0x64
    str r5, [sp]
    str r0, [sp, #4]
    add r0, r7, r11
    str r0, [sp, #0x50]
    str r1, [sp, #8]
    ldr r1, [sp, #0x50]
    mov r0, r9
    add r2, r6, r2
    mov r3, r10
    bl func_02066010
    mov r0, r0, lsl #0x18
    mov r8, r0, asr #0x18
    ldrb r1, [sp, #0x67]
    mov r0, r9
    bl func_020660b0
    ldr r0, [sp, #0x24]
    str r5, [sp]
    add r0, r6, r0
    str r0, [sp, #0x4c]
    ldr r0, [sp, #0x1c]
    add r1, sp, #0x65
    add r0, r7, r0
    str r0, [sp, #0x54]
    str r1, [sp, #4]
    add r0, sp, #0x67
    str r0, [sp, #8]
    ldr r1, [sp, #0x54]
    ldr r2, [sp, #0x4c]
    mov r0, r9
    mov r3, r10
    bl func_02066010
    mov r0, r0, lsl #0x18
    mov r0, r0, asr #0x18
    str r0, [sp, #0x58]
    ldrb r1, [sp, #0x67]
    mov r0, r9
    bl func_020660b0
    ldr r1, [r9, #0x58]
    ands r0, r1, #0x200000
    beq _554
    ands r0, r1, #0x200
    bne _554
    ldr r0, [sp, #0x1c]
    cmp r11, r0
    ble _4c8
    sub r0, r11, r0
    sub r0, r0, #1
    mov r4, #1
    str r0, [sp, #0x44]
    cmp r0, #1
    ble _554
    ldr r0, [sp, #0x20]
    add r0, r6, r0
    str r0, [sp, #0x3c]
    ldr r0, [sp, #0x1c]
    add r0, r7, r0
    str r0, [sp, #0x40]
    mov r0, #0
    str r0, [sp, #0x5c]
_46c:
    str r5, [sp]
    add r0, sp, #0x66
    str r0, [sp, #4]
    ldr r0, [sp, #0x5c]
    ldr r1, [sp, #0x40]
    str r0, [sp, #8]
    ldr r2, [sp, #0x3c]
    mov r0, r9
    add r1, r4, r1
    mov r3, r10
    bl func_02067670
    mov r0, r0, lsl #0x18
    mov r0, r0, asr #0x18
    cmp r0, r8
    movlt r8, r0
    ldrltb r0, [sp, #0x66]
    strltb r0, [sp, #0x64]
    add r0, r4, #1
    and r4, r0, #0xff
    ldr r0, [sp, #0x44]
    cmp r4, r0
    blt _46c
    b _554
_4c8:
    sub r0, r0, r11
    sub r0, r0, #1
    mov r4, #1
    str r0, [sp, #0x38]
    cmp r0, #1
    ble _554
    ldr r0, [sp, #0x20]
    add r0, r6, r0
    str r0, [sp, #0x30]
    add r0, r7, r11
    str r0, [sp, #0x34]
    mov r0, #0
    str r0, [sp, #0x60]
_4fc:
    str r5, [sp]
    add r0, sp, #0x66
    str r0, [sp, #4]
    ldr r0, [sp, #0x60]
    ldr r1, [sp, #0x34]
    str r0, [sp, #8]
    ldr r2, [sp, #0x30]
    mov r0, r9
    add r1, r4, r1
    mov r3, r10
    bl func_02067670
    mov r0, r0, lsl #0x18
    mov r0, r0, asr #0x18
    cmp r0, r8
    movlt r8, r0
    ldrb r1, [sp, #0x66]
    add r0, r4, #1
    and r4, r0, #0xff
    ldr r0, [sp, #0x38]
    strb r1, [sp, #0x64]
    cmp r4, r0
    blt _4fc
_554:
    ldr r0, [sp, #0x1c]
    cmp r11, r0
    bge _5a8
    cmp r0, #0
    rsblt r0, r0, #0
    cmp r11, #0
    rsblt r1, r11, #0
    movge r1, r11
    add r1, r1, r0
    ldr r0, [sp, #0x50]
    str r5, [sp]
    add r1, r0, r1, asr #1
    mov r0, #0
    str r0, [sp, #4]
    add r3, sp, #0x67
    str r3, [sp, #8]
    ldr r2, [sp, #0x4c]
    mov r0, r9
    mov r3, r10
    bl func_02066010
    b _5ec
_5a8:
    cmp r0, #0
    rsblt r0, r0, #0
    cmp r11, #0
    rsblt r1, r11, #0
    movge r1, r11
    add r1, r1, r0
    ldr r0, [sp, #0x54]
    str r5, [sp]
    add r1, r0, r1, asr #1
    mov r0, #0
    str r0, [sp, #4]
    add r3, sp, #0x67
    str r3, [sp, #8]
    ldr r2, [sp, #0x4c]
    mov r0, r9
    mov r3, r10
    bl func_02066010
_5ec:
    mov r0, r9
    ldrb r1, [sp, #0x67]
    bl func_020660b0
    ldrh r1, [r9, #0x5e]
    ands r0, r1, #8
    beq _668
    ldr r0, [r9, #0x58]
    ands r0, r0, #0x10
    beq _668
    ldr r0, [sp, #0x10]
    cmp r0, #0
    ble _668
    ands r0, r1, #1
    bne _668
    ldrb r1, [r9, #5]
    ldrb r0, [sp, #0x64]
    add r0, r0, r1
    and r0, r0, #0xff
    cmp r0, #0x40
    blo _644
    cmp r0, #0xc0
    bls _660
_644:
    ldrb r0, [sp, #0x65]
    add r0, r0, r1
    and r0, r0, #0xff
    cmp r0, #0x40
    blo _668
    cmp r0, #0xc0
    bhi _668
_660:
    mov r8, #0x18
    str r8, [sp, #0x58]
_668:
    ldr r0, [sp, #0x58]
    cmp r8, r0
    movlt r4, r8
    movge r4, r0
    cmp r4, #0
    beq _9e4
    cmp r4, #0
    bge _89c
    ldr r0, [r9, #0x58]
    ands r0, r0, #0x10
    beq _6a0
    ldr r0, [sp, #0x10]
    cmp r0, #0
    blt _6ac
_6a0:
    ldr r0, [r9, #0x58]
    orr r0, r0, #1
    str r0, [r9, #0x58]
_6ac:
    mvn r0, #0xd
    cmp r4, r0
    blt _a08
    ldr r1, [r9, #0x58]
    ands r0, r1, #1
    beq _a08
    mvn r0, #0
    cmp r4, r0
    bne _854
    ands r0, r1, #0x10000
    beq _854
    ldr r0, [sp, #0x58]
    ldr r1, [sp, #0x20]
    cmp r8, r0
    ldr r0, [sp, #0x24]
    ldrge r8, [sp, #0x58]
    str r0, [sp, #0x2c]
    ldr r0, [sp, #0x1c]
    cmp r5, #3
    str r0, [sp, #0x28]
    mov r0, r11
    addls pc, pc, r5, lsl #2
    b _7a4
    b _784
    b _73c
    b _718
    b _760
_718:
    ldr r2, [sp, #0x2c]
    add r1, r1, r8
    add r2, r2, r8
    mov r2, r2, lsl #0x18
    mov r1, r1, lsl #0x18
    mov r2, r2, asr #0x18
    mov r1, r1, asr #0x18
    str r2, [sp, #0x2c]
    b _7a4
_73c:
    ldr r2, [sp, #0x28]
    sub r0, r0, r8
    sub r2, r2, r8
    mov r2, r2, lsl #0x18
    mov r0, r0, lsl #0x18
    mov r2, r2, asr #0x18
    mov r0, r0, asr #0x18
    str r2, [sp, #0x28]
    b _7a4
_760:
    ldr r2, [sp, #0x2c]
    sub r1, r1, r8
    sub r2, r2, r8
    mov r2, r2, lsl #0x18
    mov r1, r1, lsl #0x18
    mov r2, r2, asr #0x18
    mov r1, r1, asr #0x18
    str r2, [sp, #0x2c]
    b _7a4
_784:
    ldr r2, [sp, #0x28]
    add r0, r0, r8
    add r2, r2, r8
    mov r2, r2, lsl #0x18
    mov r0, r0, lsl #0x18
    mov r2, r2, asr #0x18
    mov r0, r0, asr #0x18
    str r2, [sp, #0x28]
_7a4:
    mov r2, #0
    str r2, [sp]
    str r2, [sp, #4]
    add r0, r7, r0
    add r1, r6, r1
    mov r2, r10
    mov r3, r5
    bl MapCollision_ProbeDistance
    mov r0, r0, lsl #0x18
    ldr r1, [sp, #0x2c]
    mov r8, r0, asr #0x18
    ldr r0, [sp, #0x28]
    mov r2, #0
    str r2, [sp]
    str r2, [sp, #4]
    add r1, r6, r1
    add r0, r7, r0
    mov r2, r10
    mov r3, r5
    bl MapCollision_ProbeDistance
    mov r0, r0, lsl #0x18
    mov r0, r0, asr #0x18
    cmp r8, r0
    movge r8, r0
    cmp r8, #1
    beq _a08
    cmp r5, #3
    addls pc, pc, r5, lsl #2
    b _828
    b _840
    b _830
    b _828
    b _838
_828:
    add r6, r6, r4
    b _844
_830:
    sub r7, r7, r4
    b _844
_838:
    sub r6, r6, r4
    b _844
_840:
    add r7, r7, r4
_844:
    ldr r0, [r9, #0x58]
    bic r0, r0, #0x10000
    str r0, [r9, #0x58]
    b _a08
_854:
    cmp r5, #3
    addls pc, pc, r5, lsl #2
    b _870
    b _888
    b _878
    b _870
    b _880
_870:
    add r6, r6, r4
    b _88c
_878:
    sub r7, r7, r4
    b _88c
_880:
    sub r6, r6, r4
    b _88c
_888:
    add r7, r7, r4
_88c:
    ldr r0, [r9, #0x58]
    bic r0, r0, #0x10000
    str r0, [r9, #0x58]
    b _a08
_89c:
    cmp r4, #1
    ldreq r0, [r9, #0x58]
    orreq r0, r0, #0x10000
    streq r0, [r9, #0x58]
    ldr r0, [r9, #0x58]
    ands r0, r0, #0x10
    bne _a08
    ldr r0, [sp, #0x14]
    ands r0, r0, #1
    beq _8e4
    ldr r0, [sp, #0x10]
    cmp r0, #0
    rsblt r0, r0, #0
    mov r0, r0, asr #8
    add r0, r0, #3
    mov r0, r0, lsl #0x18
    mov r0, r0, asr #0x18
    b _908
_8e4:
    ldr r0, [sp, #0xc]
    cmp r0, #0
    rsblt r0, r0, #0
    strlt r0, [sp, #0xc]
    ldr r0, [sp, #0xc]
    mov r0, r0, asr #8
    add r0, r0, #3
    mov r0, r0, lsl #0x18
    mov r0, r0, asr #0x18
_908:
    cmp r0, #0xb
    movgt r0, #0xb
    cmp r4, r0
    bgt _9d4
    ldr r0, [r9, #0x58]
    cmp r5, #3
    orr r0, r0, #1
    str r0, [r9, #0x58]
    addls pc, pc, r5, lsl #2
    b _940
    b _958
    b _948
    b _940
    b _950
_940:
    add r6, r6, r4
    b _95c
_948:
    sub r7, r7, r4
    b _95c
_950:
    sub r6, r6, r4
    b _95c
_958:
    add r7, r7, r4
_95c:
    ldr r1, [r9, #0x58]
    ands r0, r1, #0x200
    bne _a08
    ands r0, r1, #0x40
    bne _a08
    ldr r0, [r9, #0x84]
    cmp r0, #0
    bne _a08
    ldr r2, [sp, #0x20]
    str r5, [sp]
    mov r0, #0
    str r0, [sp, #4]
    str r0, [sp, #8]
    mov r0, r9
    add r1, r7, r11
    add r2, r6, r2
    mov r3, r10
    bl func_02067670
    ldr r1, [sp, #0x1c]
    ldr r2, [sp, #0x24]
    str r5, [sp]
    mov r0, #0
    str r0, [sp, #4]
    str r0, [sp, #8]
    mov r0, r9
    add r1, r7, r1
    add r2, r6, r2
    mov r3, r10
    bl func_02067670
    b _a08
_9d4:
    ldr r0, [r9, #0x58]
    bic r0, r0, #1
    str r0, [r9, #0x58]
    b _a08
_9e4:
    ldr r0, [r9, #0x58]
    ands r0, r0, #0x10
    beq _9fc
    ldrsh r0, [r9, #0x30]
    cmp r0, #0
    blt _a08
_9fc:
    ldr r0, [r9, #0x58]
    orr r0, r0, #1
    str r0, [r9, #0x58]
_a08:
    ldr r1, [r9, #0x58]
    ands r0, r1, #1
    beq _b84
    ldrh r0, [r9, #0x5e]
    ands r0, r0, #1
    bne _ab0
    ands r0, r1, #0x40
    beq _ab0
    ldr r0, [sp, #0x58]
    cmp r8, r0
    blt _a74
    cmp r8, r0
    ldrgtb r0, [sp, #0x65]
    strgtb r0, [sp, #0x64]
    bgt _a74
    ldrb r1, [r9, #1]
    ldrb r0, [r9, #5]
    ldrb r2, [sp, #0x65]
    add r0, r1, r0
    and r1, r0, #0xff
    subs r3, r1, r2
    ldrb r0, [sp, #0x64]
    rsbmi r3, r3, #0
    subs r0, r1, r0
    rsbmi r0, r0, #0
    cmp r0, r3
    strgtb r2, [sp, #0x64]
_a74:
    ldrb r1, [r9, #5]
    cmp r1, #0
    ldrneb r0, [sp, #0x64]
    addne r0, r0, r1
    strneb r0, [sp, #0x64]
    ldr r0, [r9, #0x58]
    ands r0, r0, #0x800000
    ldreqb r0, [sp, #0x64]
    streqb r0, [r9, #1]
    beq _ab0
    ldrb r0, [r9, #1]
    ldrb r1, [sp, #0x64]
    mov r2, #1
    bl ObjAngle_MoveToward8
    strb r0, [r9, #1]
_ab0:
    ldr r1, [r9, #0x58]
    ands r0, r1, #0x4000
    bne _b8c
    ands r0, r1, #0x10
    movne r0, #0
    ldreqb r0, [r9, #1]
    addeq r0, r0, #0x20
    andeq r0, r0, #0xc0
    moveq r0, r0, asr #6
    andeq r0, r0, #0xff
    cmp r0, #3
    addls pc, pc, r0, lsl #2
    b _af4
    b _af4
    b _b48
    b _b5c
    b _b70
_af4:
    ldrsh r0, [r9, #0x30]
    cmp r0, #0
    ble _b8c
    ands r1, r1, #0x20000
    beq _b3c
    ldrb r1, [r9, #1]
    ldr r2, [pc, #0x244]
    ldr r3, [pc, #0x244]
    mov r1, r1, lsl #0x18
    and r1, r2, r1, lsr #16
    mov r1, r1, asr #4
    mov r1, r1, lsl #2
    ldrsh r1, [r3, r1]
    ldrsh r2, [r9, #0x2e]
    smulbb r0, r0, r1
    mov r0, r0, lsl #4
    add r0, r2, r0, asr #16
    strh r0, [r9, #0x2e]
_b3c:
    mov r0, #0
    strh r0, [r9, #0x30]
    b _b8c
_b48:
    ldrsh r0, [r9, #0x2e]
    cmp r0, #0
    movlt r0, #0
    strlth r0, [r9, #0x2e]
    b _b8c
_b5c:
    ldrsh r0, [r9, #0x30]
    cmp r0, #0
    movlt r0, #0
    strlth r0, [r9, #0x30]
    b _b8c
_b70:
    ldrsh r0, [r9, #0x2e]
    cmp r0, #0
    movgt r0, #0
    strgth r0, [r9, #0x2e]
    b _b8c
_b84:
    ldr r0, [sp, #0x18]
    str r0, [r9, #0x80]
_b8c:
    ldr r0, [sp, #0x10]
    cmp r0, #0x100
    bge _d24
    cmp r5, #3
    addls pc, pc, r5, lsl #2
    b _bb4
    b _c10
    b _bd4
    b _bb4
    b _bf0
_bb4:
    ldrsb r0, [r9, #0x65]
    mov r4, #3
    add r0, r0, #2
    mov r0, r0, lsl #0x18
    mov r0, r0, asr #0x18
    str r0, [sp, #0x20]
    str r0, [sp, #0x24]
    b _c28
_bd4:
    ldrsb r0, [r9, #0x66]
    mov r4, #0
    sub r0, r0, #2
    mov r0, r0, lsl #0x18
    mov r11, r0, asr #0x18
    str r11, [sp, #0x1c]
    b _c28
_bf0:
    ldrsb r0, [r9, #0x67]
    mov r4, #2
    sub r0, r0, #2
    mov r0, r0, lsl #0x18
    mov r0, r0, asr #0x18
    str r0, [sp, #0x20]
    str r0, [sp, #0x24]
    b _c28
_c10:
    ldrsb r0, [r9, #0x64]
    mov r4, #1
    add r0, r0, #2
    mov r0, r0, lsl #0x18
    mov r11, r0, asr #0x18
    str r11, [sp, #0x1c]
_c28:
    ldr r2, [sp, #0x20]
    orr r5, r10, #0x80
    str r4, [sp]
    mov r1, #0
    str r1, [sp, #4]
    str r1, [sp, #8]
    mov r0, r9
    add r1, r7, r11
    add r2, r6, r2
    and r3, r5, #0xff
    bl func_02066010
    mov r1, r0, lsl #0x18
    mov r8, r1, asr #0x18
    ldr r1, [sp, #0x1c]
    ldr r2, [sp, #0x24]
    str r4, [sp]
    mov r0, #0
    str r0, [sp, #4]
    str r0, [sp, #8]
    add r1, r7, r1
    add r2, r6, r2
    and r3, r5, #0xff
    mov r0, r9
    bl func_02066010
    mov r0, r0, lsl #0x18
    mov r0, r0, asr #0x18
    cmp r8, r0
    movge r8, r0
    cmp r8, #0
    bgt _d24
    ldr r1, [r9, #0x58]
    mvn r0, #0xd
    orr r1, r1, #2
    str r1, [r9, #0x58]
    cmp r8, r0
    blt _d24
    cmp r4, #3
    addls pc, pc, r4, lsl #2
    b _cd4
    b _cec
    b _cdc
    b _cd4
    b _ce4
_cd4:
    add r6, r6, r8
    b _cf0
_cdc:
    sub r7, r7, r8
    b _cf0
_ce4:
    sub r6, r6, r8
    b _cf0
_cec:
    add r7, r7, r8
_cf0:
    ldr r0, [r9, #0x58]
    ands r0, r0, #0x4000
    bne _d24
    ldr r0, [sp, #0x10]
    cmp r0, #0
    bge _d24
    add r0, r4, #0xfe
    and r0, r0, #0xff
    cmp r0, #1
    movls r0, #0
    strlsh r0, [r9, #0x30]
    movhi r0, #0
    strhih r0, [r9, #0x2e]
_d24:
    ldr r1, [r9, #0x10]
    mov r0, r1, asr #8
    sub r0, r0, r7
    sub r0, r1, r0, lsl #8
    str r0, [r9, #0x10]
    ldr r1, [r9, #0x14]
    mov r0, r1, asr #8
    sub r0, r0, r6
    sub r0, r1, r0, lsl #8
    str r0, [r9, #0x14]
    add sp, sp, #0x6c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd 0x0000ffff
    dcd data_0206e470
}
