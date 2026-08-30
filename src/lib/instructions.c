#include <instructions.h>
#include <cpu.h>

static instruction instructions[0x100] = {
    [0x00] = {.type = IN_NOP, .mode = AM_IMP},
    [0X05] = {.type = IN_DEC, .mode = AM_R, .reg_1 = RT_B},
    [0x0E] = {.type = IN_LD, .mode = AM_R_D8, .reg_1 = RT_C},
    [0xAF] = {.type = IN_XOR, .mode = AM_R, .reg_1 = RT_A},
    [0xC3] = {.type = IN_JP, .mode = AM_D16},
    [0xF3] = {.type = IN_DI},
    //[0x31] = {.type = IN_JR}
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