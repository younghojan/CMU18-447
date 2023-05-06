#include <stdio.h>
#include "shell.h"
#include "sim.h"

void process_instruction() {
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */

    uint32_t instruction = mem_read_32(CURRENT_STATE.PC);
    uint8_t op = instruction >> 26, secondary_OP;

    int32_t immediate;
    uint32_t jump_target, temp_res, vaddr;
    int32_t offset;
    uint8_t rs, rt, base, rd, sa;
    int64_t mult_result;

    switch (op) {

        /* Jump */
        case J:
            jump_target = instruction & 0x03FFFFFF;
            NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) + (jump_target << 2);
            break;

        /* Jump And Link */
        case JAL:
            jump_target = instruction & 0x03FFFFFF;
            NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) + (jump_target << 2);

            /* according to the lab guide, it should be PC + 4, not PC + 8. */
            NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;

            break;

        /* Branch On Equal */
        case BEQ:
            rs = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;

            /* parentheses are irreducible, we need them to ensure the correct order of operations. */
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 14;

            if (CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt]) {
                NEXT_STATE.PC = CURRENT_STATE.PC + offset;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;

        /* Branch On Not Equal */
        case BNE:
            rs = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 14;
            if (CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt]) {
                NEXT_STATE.PC = CURRENT_STATE.PC + offset;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;

        /* Branch on Less Than Or Equal To Zero */
        case BLEZ:
            rs = (instruction & 0x03E00000) >> 21;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 14;
            if ((CURRENT_STATE.REGS[rs] & 0x8000000) || CURRENT_STATE.REGS[rs] == 0) {
                NEXT_STATE.PC = CURRENT_STATE.PC + offset;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;

        /* Branch On Greater Than Zero */
        case BGTZ:
            rs = (instruction & 0x03E00000) >> 21;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 14;
            if ((CURRENT_STATE.REGS[rs] & 0x8000000) == 0 && CURRENT_STATE.REGS[rs]) {
                NEXT_STATE.PC = CURRENT_STATE.PC + offset;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;

        /* Add Immediate */
        case ADDI:
            rs = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            immediate = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + immediate;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;

            /* according to the lab guide, overflow exceptions is not required to be handled. */

            break;

        /* Add Immediate Unsigned */
        case ADDIU:
            rs = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            immediate = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + immediate;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;

            /* according to the MIPS R4000 User Manual, ADDIU never causes an overflow exception. */

            break;

        /* Set On Less Than Immediate */
        case SLTI:
            rs = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            immediate = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            if ((int32_t) CURRENT_STATE.REGS[rs] < immediate) {
                NEXT_STATE.REGS[rt] = (uint32_t) 1;
            } else {
                NEXT_STATE.REGS[rt] = (uint32_t) 0;
            }
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Set On Less Than Immediate Unsigned */
        case SLTIU:
            rs = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            immediate = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            if (CURRENT_STATE.REGS[rs] < (uint32_t) immediate) {
                CURRENT_STATE.REGS[rt] = (uint32_t) 1;
            } else {
                CURRENT_STATE.REGS[rt] = (uint32_t) 0;
            }
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* And Immediate */
        case ANDI:
            rs = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            immediate = instruction & 0x0000FFFF;
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & immediate;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Or Immediate */
        case ORI:
            rs = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            immediate = instruction & 0x0000FFFF;
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | immediate;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Exclusive OR Immediate */
        case XORI:
            rs = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            immediate = instruction & 0x0000FFFF;
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ immediate;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Load Upper Immediate */
        case LUI:
            rt = (instruction & 0x001F0000) >> 16;
            immediate = (instruction & 0x0000FFFF) << 16;
            NEXT_STATE.REGS[rt] = immediate;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Load Byte */
        case LB:
            base = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + offset;
            NEXT_STATE.REGS[rt] = (int32_t) (int8_t) mem_read_32(vaddr);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Load Halfword */
        case LH:
            base = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + offset;
            NEXT_STATE.REGS[rt] = (int32_t) (int16_t) mem_read_32(vaddr);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Load Word */
        case LW:
            base = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + offset;
            NEXT_STATE.REGS[rt] = mem_read_32(vaddr);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Load Byte Unsigned */
        case LBU:
            base = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + offset;
            NEXT_STATE.REGS[rt] = (uint8_t) mem_read_32(vaddr);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Load Halfword Unsigned */
        case LHU:
            base = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + offset;
            NEXT_STATE.REGS[rt] = (uint16_t) mem_read_32(vaddr);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Store Byte */
        case SB:
            base = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + offset;
            mem_write_32(vaddr, (uint8_t) CURRENT_STATE.REGS[rt]);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Store Halfword */
        case SH:
            base = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + offset;
            mem_write_32(vaddr, (uint16_t) CURRENT_STATE.REGS[rt]);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        /* Store Word */
        case SW:
            base = (instruction & 0x03E00000) >> 21;
            rt = (instruction & 0x001F0000) >> 16;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + offset;
            mem_write_32(vaddr, CURRENT_STATE.REGS[rt]);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        case REGIMM:
            rs = (instruction & 0x03E00000) >> 21;
            secondary_OP = (instruction & 0x001F0000) >> 16;
            offset = (((int32_t) instruction & 0x0000FFFF) << 16) >> 14;
            switch (secondary_OP) {
                
                /* Branch On Less Than Zero */
                case BLTZ:
                    if (CURRENT_STATE.REGS[rs] >> 31) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;

                /* Branch On Greater Than Or Equal To Zero */
                case BGEZ:
                    if (CURRENT_STATE.REGS[rs] >> 31 == 0) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;

                /* Branch On Less Than Zero And Link */
                case BLTZAL:
                    CURRENT_STATE.REGS[rs] = CURRENT_STATE.PC + 4;
                    if (CURRENT_STATE.REGS[rs] >> 31) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;

                default:
                    break;
            }

        case SPECIAL:
            secondary_OP = instruction & 0x0000003F;
            switch (secondary_OP) {
                
                /* Shift Left Logical */
                case SLL:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    sa = (instruction & 0x000007C0) >> 6;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << sa;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Shift Right Logical */
                case SRL:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    sa = (instruction & 0x000007C0) >> 6;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> sa;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Shift Right Arithmetic */
                case SRA:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    sa = (instruction & 0x000007C0) >> 6;
                    NEXT_STATE.REGS[rd] = ((int32_t) CURRENT_STATE.REGS[rt]) >> sa;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Shift Left Logical Variable */
                case SLLV:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << (CURRENT_STATE.REGS[rs] & 0x0000001F);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Shift Right Logical Variable */
                case SRLV:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> (CURRENT_STATE.REGS[rs] & 0x0000001F);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                
                /* Shift Right Arithmetic Variable */
                case SRAV:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = ((int32_t) CURRENT_STATE.REGS[rt]) >> (CURRENT_STATE.REGS[rs] & 0x0000001F);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Jump Register */
                case JR:
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
                    break;

                /* Jump And Link Register */
                case JALR:
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.PC + 4;
                    NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
                    break;

                /* Add */
                case ADD:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = (int32_t) CURRENT_STATE.REGS[rs] + (int32_t) CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Add Unsigned */
                case ADDU:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = (int32_t) CURRENT_STATE.REGS[rs] + (int32_t) CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Subtract */
                case SUB:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = (int32_t) CURRENT_STATE.REGS[rs] - (int32_t) CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Subtract Unsigned */
                case SUBU:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = (int32_t) CURRENT_STATE.REGS[rs] - (int32_t) CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* And */
                case AND:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Or */
                case OR:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Exclusive Or */
                case XOR:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Nor */
                case NOR:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Set On Less Than */
                case SLT:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    if ((int32_t) CURRENT_STATE.REGS[rs] < (int32_t) CURRENT_STATE.REGS[rt]) {
                        NEXT_STATE.REGS[rd] = 1;
                    } else {
                        NEXT_STATE.REGS[rd] = 0;
                    }
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Set On Less Than Unsigned */
                case SLTU:
                    rt = (instruction & 0x001F0000) >> 16;
                    rd = (instruction & 0x0000F800) >> 11;
                    rs = (instruction & 0x03E00000) >> 21;
                    if (CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt]) {
                        NEXT_STATE.REGS[rd] = 1;
                    } else {
                        NEXT_STATE.REGS[rd] = 0;
                    }
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Multiply */
                case MULT:
                    rt = (instruction & 0x001F0000) >> 16;
                    rs = (instruction & 0x03E00000) >> 21;
                    mult_result = (int64_t) CURRENT_STATE.REGS[rs] * (int64_t) CURRENT_STATE.REGS[rt];
                    NEXT_STATE.HI = mult_result & 0xFFFFFFFF00000000;
                    NEXT_STATE.LO = mult_result & 0x00000000FFFFFFFF;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Move From HI */
                case MFHI:
                    rd = (instruction & 0x0000F800) >> 11;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Move From Lo */
                case MFLO:
                    rd = (instruction & 0x0000F800) >> 11;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Multiply Unsigned */
                case MULTU:
                    rt = (instruction & 0x001F0000) >> 16;
                    rs = (instruction & 0x03E00000) >> 21;
                    mult_result = (uint64_t) CURRENT_STATE.REGS[rs] * (uint64_t) CURRENT_STATE.REGS[rt];
                    NEXT_STATE.HI = mult_result & 0xFFFFFFFF00000000;
                    NEXT_STATE.LO = mult_result & 0x00000000FFFFFFFF;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Divide */
                case DIV:
                    rt = (instruction & 0x001F0000) >> 16;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.HI = (int64_t) CURRENT_STATE.REGS[rs] % (int64_t) CURRENT_STATE.REGS[rt];
                    NEXT_STATE.LO = (int64_t) CURRENT_STATE.REGS[rs] / (int64_t) CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                /* Divide Unsigned */
                case DIVU:
                    rt = (instruction & 0x001F0000) >> 16;
                    rs = (instruction & 0x03E00000) >> 21;
                    NEXT_STATE.HI = (uint64_t) CURRENT_STATE.REGS[rs] % (uint64_t) CURRENT_STATE.REGS[rt];
                    NEXT_STATE.LO = (uint64_t) CURRENT_STATE.REGS[rs] / (uint64_t) CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case SYSCALL:
                    if (CURRENT_STATE.REGS[2] == 0x0000000A) {
                        RUN_BIT = 0;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;

                default:
                    break;
            }

        default:
            break;
    }
}
