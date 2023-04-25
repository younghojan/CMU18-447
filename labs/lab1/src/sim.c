#include <stdio.h>
#include "shell.h"
#include "sim.h"

void process_instruction() {
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */

    uint32_t IR = mem_read_32(CURRENT_STATE.PC);
    uint8_t OP = IR >> 26;

    uint32_t jump_target, immediate, temp_res, vaddr;
    uint16_t offset;
    uint8_t rs, rt, base, sa, secondary_OP, rd;

    switch (OP) {

        case J:
            jump_target = IR & 0x03FFFFFF;
            NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) | (jump_target << 2);
            break;

        case JAL:
            jump_target = IR & 0x03FFFFFF;
            NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) | (jump_target << 2);
            NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 8;
            break;

        case BEQ:
            rs = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            offset = IR & 0x0000FFFF;
            jump_target = (offset << 16) >> 14; // signed
            if (CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt]) {
                NEXT_STATE.PC = CURRENT_STATE.PC + jump_target;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;

        case BNE:
            rs = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            offset = IR & 0x0000FFFF;
            jump_target = (offset << 16) >> 16;
            if (CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt]) {
                NEXT_STATE.PC = CURRENT_STATE.PC + jump_target;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;

        case BLEZ:
            rs = (IR & 0x03E00000) >> 21;
            offset = IR & 0x0000FFFF;
            jump_target = (offset << 16) >> 14; // signed
            if ((rs >> 4) == 1 || rs == 0) {
                NEXT_STATE.PC = CURRENT_STATE.PC + jump_target;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;

        case BGTZ:
            rs = (IR & 0x03E00000) >> 21;
            offset = IR & 0x0000FFFF;
            jump_target = offset << 16;
            if ((rs >> 4) == 0 && rs != 0) {
                NEXT_STATE.PC = CURRENT_STATE.PC + jump_target;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;

        case ADDI:
            rs = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            immediate = ((IR & 0x0000FFFF) << 16) >> 16;
            temp_res = CURRENT_STATE.REGS[rs] + immediate;
            if (!((CURRENT_STATE.REGS[rs] >> 31) & (immediate >> 31) & !(temp_res >> 31) ||
                  !(CURRENT_STATE.REGS[rs] >> 31) & !(immediate >> 31) & (temp_res >> 31))) {
                CURRENT_STATE.REGS[rt] = temp_res;
            }
            break;

        case ADDIU:
            rs = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            immediate = ((IR & 0x0000FFFF) << 16) >> 16;
            CURRENT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + immediate;
            break;

        case SLTI:
            rs = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            immediate = ((IR & 0x0000FFFF) << 16) >> 16;
            if (CURRENT_STATE.REGS[rs] < immediate) {
                CURRENT_STATE.REGS[rt] = 1;
            } else {
                CURRENT_STATE.REGS[rt] = 0;
            }
            break;

        case SLTIU:
            rs = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            immediate = ((IR & 0x0000FFFF) << 16) >> 16;
            if (CURRENT_STATE.REGS[rs] < (unsigned int) immediate) {
                CURRENT_STATE.REGS[rt] = 1;
            } else {
                CURRENT_STATE.REGS[rt] = 0;
            }
            break;

        case ANDI:
            rs = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            immediate = IR & 0x0000FFFF;
            CURRENT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & immediate;
            break;

        case ORI:
            rs = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            immediate = (IR & 0x0000FFFF) << 16;
            CURRENT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | immediate;
            break;

        case XORI:
            rs = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            immediate = IR & 0x0000FFFF;
            CURRENT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ immediate;
            break;

        case LUI:
            rt = (IR & 0x001F0000) >> 16;
            immediate = (IR & 0x0000FFFF) << 16;
            CURRENT_STATE.REGS[rt] = immediate;
            break;

        case LB:
            base = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            offset = ((IR & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + offset;
            NEXT_STATE.REGS[rt] = ((int8_t) mem_read_32(vaddr) << 24) >> 24;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        case LH:
            base = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            offset = ((IR & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + ((offset << 16) >> 16);
            NEXT_STATE.REGS[rt] = ((int16_t) mem_read_32(vaddr) << 16) >> 16;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        case LW:
            base = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            offset = ((IR & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + ((offset << 16) >> 16);
            NEXT_STATE.REGS[rt] = mem_read_32(vaddr);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        case LBU:
            base = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            offset = ((IR & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + offset;
            NEXT_STATE.REGS[rt] = mem_read_32(vaddr) & 0x000000FF;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        case LHU:
            base = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            offset = ((IR & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + ((offset << 16) >> 16);
            NEXT_STATE.REGS[rt] = mem_read_32(vaddr) & 0x0000FFFF;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        case SB:
            base = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            offset = ((IR & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + ((offset << 16) >> 16);
            mem_write_32(vaddr, (uint8_t) CURRENT_STATE.REGS[rt]);
            break;

        case SH:
            base = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            offset = ((IR & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + ((offset << 16) >> 16);
            mem_write_32(vaddr, (uint16_t) CURRENT_STATE.REGS[rt]);
            break;

        case SW:
            base = (IR & 0x03E00000) >> 21;
            rt = (IR & 0x001F0000) >> 16;
            offset = ((IR & 0x0000FFFF) << 16) >> 16;
            vaddr = CURRENT_STATE.REGS[base] + ((offset << 16) >> 16);
            mem_write_32(vaddr, CURRENT_STATE.REGS[rt]);
            break;

        case REGIMM:
            rs = (IR & 0x03E00000) >> 21;
            secondary_OP = (IR & 0x001F0000) >> 16;
            offset = ((IR & 0x0000FFFF) << 16) >> 14;
            switch (secondary_OP) {

                case BLTZ:
                    if (CURRENT_STATE.REGS[rs] >> 31 == 1) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;

                case BGEZ:
                    if (CURRENT_STATE.REGS[rs] >> 31 == 0) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;

                case BLTZAL:
                    jump_target = CURRENT_STATE.PC + offset;
                    if (CURRENT_STATE.REGS[rs] >> 31 == 1) {
                        NEXT_STATE.PC = jump_target;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    CURRENT_STATE.REGS[rs] = CURRENT_STATE.PC + 4;
                    break;

                default:
                    break;
            }

        case SPECIAL:
            secondary_OP = IR & 0x0000003F;
            rt = (IR & 0x001F0000) >> 16;
            rd = (IR & 0x0000F800) >> 11;
            sa = (IR & 0x000007C0) >> 6;
            switch (secondary_OP) {

                case SLL:
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << sa;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case SRL:
                    NEXT_STATE.REGS[rd] = (CURRENT_STATE.REGS[rt] >> sa);
                    if (CURRENT_STATE.REGS[rt] >> 31 == 1) {
                        NEXT_STATE.REGS[rd] &= (0xFFFFFFFF >> sa);
                    }
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case SRA:
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> sa;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case SLLV:
                    rs = (IR & 0x03E00000) >> 21;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << (CURRENT_STATE.REGS[rs] & 0x0000001F);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case SRLV:
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> (CURRENT_STATE.REGS[rs] & 0x0000001F);
                    if (CURRENT_STATE.REGS[rt] >> 31 == 1) {
                        NEXT_STATE.REGS[rd] &= (0xFFFFFFFF >> (CURRENT_STATE.REGS[rs] & 0x0000001F));
                    }
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case SRAV:
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> (CURRENT_STATE.REGS[rs] & 0x0000001F);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case JR:
                    NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
                    break;

                case JALR:
                    NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
                    break;

                case ADD:
                    temp_res = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
                    if (!((CURRENT_STATE.REGS[rs] >> 31) & (CURRENT_STATE.REGS[rt] >> 31) & !(temp_res >> 31) ||
                          !(CURRENT_STATE.REGS[rs] >> 31) & !(CURRENT_STATE.REGS[rt] >> 31) & (temp_res >> 31))) {
                        CURRENT_STATE.REGS[rt] = temp_res;
                    }
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case ADDU:
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case SUB:
                    temp_res = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
                    if (!(((CURRENT_STATE.REGS[rs] >> 31 == 1) && (CURRENT_STATE.REGS[rt] >> 31 == 0) &&
                           (temp_res >> 31 == 0)) ||
                          ((CURRENT_STATE.REGS[rs] >> 31 == 0) && (CURRENT_STATE.REGS[rt] >> 31 == 1) &&
                           (temp_res >> 31 == 1)))) {
                        NEXT_STATE.REGS[rd] = temp_res;
                    }
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case SUBU:
                    temp_res = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
                    if (temp_res <= CURRENT_STATE.REGS[rs]) {
                        NEXT_STATE.REGS[rd] = temp_res;
                    }
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case AND:
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case OR:
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case XOR:
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case NOR:
                    NEXT_STATE.REGS[rd] = ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case SLT:
                    if (CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt]) {
                        NEXT_STATE.REGS[rd] = 1;
                    } else {
                        NEXT_STATE.REGS[rd] = 0;
                    }
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case SLTU:
                    if (CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt]) {
                        NEXT_STATE.REGS[rd] = 1;
                    } else {
                        NEXT_STATE.REGS[rd] = 0;
                    }
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case MULT:
                    temp_res = (int64_t) CURRENT_STATE.REGS[rs] * (int64_t) CURRENT_STATE.REGS[rt];
                    NEXT_STATE.HI = (temp_res) & 0xFFFFFFFF00000000;
                    NEXT_STATE.LO = (temp_res) & 0x00000000FFFFFFFF;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case MFHI:
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case MFLO:
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case MULTU:
                    temp_res = CURRENT_STATE.REGS[rs] * CURRENT_STATE.REGS[rt];
                    NEXT_STATE.HI = (temp_res) & 0xFFFFFFFF00000000;
                    NEXT_STATE.LO = (temp_res) & 0x00000000FFFFFFFF;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case DIV:
                    NEXT_STATE.HI = ((int32_t) CURRENT_STATE.REGS[rs] % (int32_t) CURRENT_STATE.REGS[rt]);
                    NEXT_STATE.LO = ((int32_t) CURRENT_STATE.REGS[rs] / (int32_t) CURRENT_STATE.REGS[rt]);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case DIVU:
                    NEXT_STATE.HI = (CURRENT_STATE.REGS[rs] % CURRENT_STATE.REGS[rt]);
                    NEXT_STATE.LO = (CURRENT_STATE.REGS[rs] / CURRENT_STATE.REGS[rt]);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;

                case SYSCALL:
                    if (CURRENT_STATE.REGS[2] == 0x0A) {
                        RUN_BIT = FALSE;
                    }

                    break;

                default:
                    break;
            }

        default:
            break;
    }
}
