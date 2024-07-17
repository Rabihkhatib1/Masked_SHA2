    .syntax unified
    .thumb
    .global b2a_asm
    .type b2a_asm, %function

b2a_asm:
        push {r4-r11, lr}            @ Save registers
        mov r12, r0
        sub sp, sp, #8               @ Allocate space for y on the stack (64 bits)

        ldr r3, [r0, #0]             @ Load x->xs lower 32 bits into r3
        ldr r4, [r0, #4]             @ Load x->xs upper 32 bits into r4
        ldr r5, [r0, #8]             @ Load x->xr lower 32 bits into r5
        ldr r6, [r0, #12]            @ Load x->xr upper 32 bits into r6

        mov r0, sp                   @ Move address of y on stack to r0
        movs r1, #8                  @ Size 8 for get_share_rand
        bl randombytes            @ Call get_share_rand(&y, 8)
        mov r0, r12
        ldr r7, [sp]                 @ Load y lower 32 bits into r7
        ldr r8, [sp, #4]             @ Load y upper 32 bits into r8

        eor r9, r3, r7               @ T_lower = x->xs lower ^ y lower
        eor r10, r4, r8               @ T_upper = x->xs upper ^ y upper

        subs r11, r9, r7             @ T_lower = T_lower - y lower
        sbcs r12, r10, r8             @ T_upper = T_upper - y upper

        eor r11, r11, r3             @ T_lower = T_lower ^ x->xs lower
        eor r12, r12, r4             @ T_upper = T_upper ^ x->xs upper

        eor r7, r7, r5               @ y_lower = y lower ^ x->xr lower
        eor r8, r8, r6               @ y_upper = y upper ^ x->xr upper

        eor r3, r3, r7               @ xa_lower = x->xs lower ^ y lower
        eor r4, r4, r8               @ xa_upper = x->xs upper ^ y upper

        subs r3, r3, r7              @ xa_lower = xa_lower - y lower
        sbcs r4, r4, r8              @ xa_upper = xa_upper - y upper

        eor r3, r3, r11              @ xa_lower = xa_lower ^ T_lower
        eor r4, r4, r12              @ xa_upper = xa_upper ^ T_upper

        str r3, [r0, #0]             @ Store xa lower 32 bits into x->xs
        str r4, [r0, #4]             @ Store xa upper 32 bits into x->xs

        add sp, sp, #8               @ Deallocate space for y from the stack
        pop {r4-r11, pc}             @ Restore registers and return Restore registers and return



    .global a2b_asm
    .type a2b_asm, %function
a2b_asm:
        push {r0,r4-r11, lr}            @ Save registers
        sub sp, sp, #8               @ Allocate space for y on the stack (64 bits)

        ldr r6, [r0, #0]             @ Load x->xs lower 32 bits into r6
        ldr r3, [r0, #4]             @ Load x->xs upper 32 bits into r3
        ldr r4, [r0, #8]             @ Load x->xr lower 32 bits into r4
        ldr r5, [r0, #12]            @ Load x->xr upper 32 bits into r5

        mov r0, sp                   @ Move address of y on stack to r0
        movs r1, #8                  @ Size 8 for randombytes
        movs r1, #8                  @ Size 8 for randombytes
        bl randombytes               @ Call randombytes(&y, 8)
        ldr r2, [sp]                 @ Load y lower 32 bits into r2
        ldr r7, [sp, #4]             @ Load y upper 32 bits into r7

        lsls r8, r2, #1               @ T_lower = (2 * y lower)
        adc r9, r7, r7               @ T_upper = (2 * y upper) + carry from lower

        eor r10, r2, r4              @ xb_lower = y lower ^ x->xr lower
        eor r11, r7, r5              @ xb_upper = y upper ^ x->xr upper

        and r12, r2, r10             @ O_lower = y lower & xb lower
        and r1, r7, r11              @ O_upper = y upper & xb upper

        eor r10, r10, r8             @ xb_lower = xb_lower ^ T_lower
        eor r11, r11, r9             @ xb_upper = xb_upper ^ T_upper

        eor r2, r2, r10              @ y_lower = y lower ^ xb lower
        eor r7, r7, r11              @ y_upper = y upper ^ xb upper

        and r2, r2, r4               @ y_lower = y lower & x->xr lower
        and r7, r7, r5               @ y_upper = y upper & x->xr upper

        eor r12, r12, r2             @ O_lower = O_lower ^ y lower
        eor r1, r1, r7               @ O_upper = O_upper ^ y upper

        and r2, r8, r6               @ y_lower = T_lower & x->xs lower
        and r7, r9, r3               @ y_upper = T_upper & x->xs upper

        eor r12, r12, r2             @ O_lower = O_lower ^ y lower
        eor r1, r1, r7               @ O_upper = O_upper ^ y upper

        movs r0, #1                  @ i = 1

loop:
        and r2, r8, r4               @ y_lower = T_lower & x->xr lower
        and r7, r9, r5               @ y_upper = T_upper & x->xr upper

        eor r2, r2, r12              @ y_lower = y lower ^ O lower
        eor r7, r7, r1               @ y_upper = y upper ^ O upper

        and r8, r8, r6               @ T_lower = T_lower & x->xs lower
        and r9, r9, r3               @ T_upper = T_upper & x->xs upper

        eor r2, r2, r8               @ y_lower = y lower ^ T lower
        eor r7, r7, r9               @ y_upper = y upper ^ T upper

        lsls r8, r2, #1               @ T_lower = (2 * y lower)
        adc r9, r7, r7               @ T_upper = (2 * y upper) + carry from lower

        adds r0, r0, #1              @ i++
        cmp r0, #64                  @ Compare i with WORD_SIZE (64)
        blt loop                     @ If i < 64, repeat loop

        eor r10, r10, r8             @ xb_lower = xb_lower ^ T lower
        eor r11, r11, r9             @ xb_upper = xb_upper ^ T upper

        add sp, sp, #8               @ Deallocate space for y from the stack
        pop {r0}
        str r10, [r0, #0]            @ Store xb lower 32 bits into x->xs
        str r11, [r0, #4]            @ Store xb upper 32 bits into x->xs
        pop {r4-r11, pc}             @ Restore registers and return