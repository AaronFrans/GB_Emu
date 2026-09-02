#include <instructions.h>
#include <cpu.h>

static instruction instructions[0x100] = {
    [0x00] = {.type = IN_NOP, .mode = AM_IMP},
    [0x01] = {.type = IN_LD, .mode = AM_R_D16, .reg_1 = RT_BC},
    [0x02] = {.type = IN_LD, .mode = AM_MR_R, .reg_1 = RT_BC, .reg_2 = RT_A},

    [0x05] = {.type = IN_DEC, .mode = AM_R, .reg_1 = RT_B},
    [0x06] = {.type = IN_LD, .mode = AM_R_D8, .reg_1 = RT_B},

    [0x08] = {.type = IN_LD, .mode = AM_A16_R, .reg_1 = RT_NONE, .reg_2 = RT_SP},

    [0x0A] = {.type = IN_LD, .mode = AM_R_MR, .reg_1 = RT_A, .reg_2 = RT_BC},

    [0x0E] = {.type = IN_LD, .mode = AM_R_D8, .reg_1 = RT_C},

    //0x1X
    [0x11] = {.type = IN_LD, .mode = AM_R_D16, .reg_1 = RT_DE},
    [0x12] = {.type = IN_LD, .mode = AM_MR_R, .reg_1 = RT_DE, .reg_2 = RT_A},
    [0x15] = {.type = IN_DEC, .mode = AM_R, .reg_1 = RT_D},
    [0x16] = {.type = IN_LD, .mode = AM_R_D8, .reg_1 = RT_D},
    [0x1A] = {.type = IN_LD, .mode = AM_R_MR, .reg_1 = RT_A, .reg_2 = RT_DE},
    [0x1E] = {.type = IN_LD, .mode = AM_R_D8, .reg_1 = RT_E},

    //0x2X
    [0x21] = {.type = IN_LD, .mode = AM_R_D16, .reg_1 = RT_HL},
    [0x22] = {.type = IN_LD, .mode = AM_HLI_R, .reg_1 = RT_HL, .reg_2 = RT_A},
    [0x25] = {.type = IN_DEC, .mode = AM_R, .reg_1 = RT_H},
    [0x26] = {.type = IN_LD, .mode = AM_R_D8, .reg_1 = RT_H},
    [0x2A] = {.type = IN_LD, .mode = AM_R_HLI, .reg_1 = RT_A, .reg_2 = RT_HL},
    [0x2E] = {.type = IN_LD, .mode = AM_R_D8, .reg_1 = RT_L},

    //0x3X
    [0x31] = {.type = IN_LD, .mode = AM_R_D16, .reg_1 = RT_SP},
    [0x32] = {.type = IN_LD, .mode = AM_HLD_R, .reg_1 = RT_HL, .reg_2 = RT_A},
    [0x35] = {.type = IN_DEC, .mode = AM_R, .reg_1 = RT_HL},
    [0x36] = {.type = IN_LD, .mode = AM_MR_D8, .reg_1 = RT_HL},
    [0x3A] = {.type = IN_LD, .mode = AM_R_HLD, .reg_1 = RT_A, .reg_2 = RT_HL},
    [0x3E] = {.type = IN_LD, .mode = AM_R_D8, .reg_1 = RT_A},

    //0x4X
    [0x40] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_B, .reg_2 = RT_B},
    [0x41] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_B, .reg_2 = RT_C},
    [0x42] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_B, .reg_2 = RT_D},
    [0x43] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_B, .reg_2 = RT_E},
    [0x44] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_B, .reg_2 = RT_H},
    [0x45] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_B, .reg_2 = RT_L},
    [0x46] = {.type = IN_LD, .mode = AM_R_MR, .reg_1 = RT_B, .reg_2 = RT_HL},
    [0x47] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_B, .reg_2 = RT_A},
    [0x48] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_C, .reg_2 = RT_B},
    [0x49] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_C, .reg_2 = RT_C},
    [0x4A] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_C, .reg_2 = RT_D},
    [0x4B] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_C, .reg_2 = RT_E},
    [0x4C] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_C, .reg_2 = RT_H},
    [0x4D] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_C, .reg_2 = RT_L},
    [0x4E] = {.type = IN_LD, .mode = AM_R_MR, .reg_1 = RT_C, .reg_2 = RT_HL},
    [0x4F] = {.type = IN_LD, .mode = AM_R_R, .reg_1 = RT_C, .reg_2 = RT_A},

    //0x5X
    [0x50] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_D, .reg_2 = RT_B},
    [0x51] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_D, .reg_2 = RT_C},
    [0x52] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_D, .reg_2 = RT_D},
    [0x53] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_D, .reg_2 = RT_E},
    [0x54] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_D, .reg_2 = RT_H},
    [0x55] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_D, .reg_2 = RT_L},
    [0x56] = {.type = IN_LD, .mode = AM_R_MR, .reg_1 = RT_D, .reg_2 = RT_HL},
    [0x57] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_D, .reg_2 = RT_A},
    [0x58] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_E, .reg_2 = RT_B},
    [0x59] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_E, .reg_2 = RT_C},
    [0x5A] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_E, .reg_2 = RT_D},
    [0x5B] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_E, .reg_2 = RT_E},
    [0x5C] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_E, .reg_2 = RT_H},
    [0x5D] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_E, .reg_2 = RT_L},
    [0x5E] = {.type = IN_LD, .mode = AM_R_MR, .reg_1 = RT_E, .reg_2 = RT_HL},
    [0x5F] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_E, .reg_2 = RT_A},

    //0x6X
    [0x60] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_H, .reg_2 = RT_B},
    [0x61] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_H, .reg_2 = RT_C},
    [0x62] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_H, .reg_2 = RT_D},
    [0x63] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_H, .reg_2 = RT_E},
    [0x64] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_H, .reg_2 = RT_H},
    [0x65] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_H, .reg_2 = RT_L},
    [0x66] = {.type = IN_LD, .mode = AM_R_MR, .reg_1 = RT_H, .reg_2 = RT_HL},
    [0x67] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_H, .reg_2 = RT_A},
    [0x68] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_L, .reg_2 = RT_B},
    [0x69] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_L, .reg_2 = RT_C},
    [0x6A] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_L, .reg_2 = RT_D},
    [0x6B] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_L, .reg_2 = RT_E},
    [0x6C] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_L, .reg_2 = RT_H},
    [0x6D] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_L, .reg_2 = RT_L},
    [0x6E] = {.type = IN_LD, .mode = AM_R_MR, .reg_1 = RT_L, .reg_2 = RT_HL},
    [0x6F] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_L, .reg_2 = RT_A},

    //0x7X
    [0x70] = {.type = IN_LD, .mode = AM_MR_R,  .reg_1 = RT_HL, .reg_2 = RT_B},
    [0x71] = {.type = IN_LD, .mode = AM_MR_R,  .reg_1 = RT_HL, .reg_2 = RT_C},
    [0x72] = {.type = IN_LD, .mode = AM_MR_R,  .reg_1 = RT_HL, .reg_2 = RT_D},
    [0x73] = {.type = IN_LD, .mode = AM_MR_R,  .reg_1 = RT_HL, .reg_2 = RT_E},
    [0x74] = {.type = IN_LD, .mode = AM_MR_R,  .reg_1 = RT_HL, .reg_2 = RT_H},
    [0x75] = {.type = IN_LD, .mode = AM_MR_R,  .reg_1 = RT_HL, .reg_2 = RT_L},
    [0x76] = {.type = IN_HALT},
    [0x77] = {.type = IN_LD, .mode = AM_MR_R,  .reg_1 = RT_HL, .reg_2 = RT_A},
    [0x78] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_A, .reg_2 = RT_B},
    [0x79] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_A, .reg_2 = RT_C},
    [0x7A] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_A, .reg_2 = RT_D},
    [0x7B] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_A, .reg_2 = RT_E},
    [0x7C] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_A, .reg_2 = RT_H},
    [0x7D] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_A, .reg_2 = RT_L},
    [0x7E] = {.type = IN_LD, .mode = AM_R_MR, .reg_1 = RT_A, .reg_2 = RT_HL},
    [0x7F] = {.type = IN_LD, .mode = AM_R_R,  .reg_1 = RT_A, .reg_2 = RT_A},
    //0x7X
    //0x8X
    //0x9X
    //0xAX
    [0xAF] = {.type = IN_XOR, .mode = AM_R, .reg_1 = RT_A},
    //0xBX
    //0xCX
    [0xC3] = {.type = IN_JP, .mode = AM_D16},

    //0xEX
    [0xE2] = {.type = IN_LD, .mode = AM_MR_R, .reg_1 = RT_C, .reg_2 = RT_A},
    [0xEA] = {.type = IN_LD, .mode = AM_A16_R, .reg_1 = RT_NONE, .reg_2 = RT_A},


    //0xFX
    [0xF2] = {.type = IN_LD, .mode = AM_R_MR, .reg_1 = RT_A, .reg_2 = RT_C},
    [0xF3] = {.type = IN_DI},
    [0xFA] = {.type = IN_LD, .mode = AM_R_A16, .reg_1 = RT_A},

};

static char *inst_lookup[] = {

    "<NONE>",
    "NOP",
    "LD",
    "INC",
    "DEC",
    "RLCA",
    "ADD",
    "RRCA",
    "STOP",
    "RLA",
    "JR",
    "RRA",
    "DAA",
    "CPL",
    "SCF",
    "CCF",
    "HALT",
    "ADC",
    "SUB",
    "SBC",
    "AND",
    "XOR",
    "OR",
    "CP",
    "POP",
    "JP",
    "PUSH",
    "RET",
    "CB",
    "CALL",
    "RETI",
    "LDH",
    "JPHL",
    "DI",
    "EI",
    "RST",
    "ERR",
    "RLC",
    "RRC",
    "RL",
    "RR",
    "SLA",
    "SRA",
    "SWAP",
    "SRL",
    "BIT",
    "RES",
    "SET",
};

instruction *instruction_by_opcode(const u8 opcode)
{
    return &instructions[opcode];
}


char *inst_name(const in_type t) {
    return inst_lookup[t];
}