#pragma once

#include <common.h>
#include <instructions.h>

// ------ DEFINES ------
#define CPU_FLAG_Z BIT(ctx->regs.f, 7)
#define CPU_FLAG_C BIT(ctx->regs.f, 4)


// ------ TYPEDEFS ------
typedef struct
{
    u8 a;
    u8 f; // Flags
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 pc;
    u16 sp;
} cpu_registers;

typedef struct
{
    cpu_registers regs;

    // current fetch
    u16 fetched_data;
    u16 mem_dest;
    bool dest_is_mem;
    u8 cur_opcode;
    instruction *cur_inst;

    bool halted;
    bool stepping;

    bool int_master_enabled;
} cpu_context;

typedef void (*IN_PROC) (cpu_context *);


// ------ FUNCTIONS ------
void cpu_init();
bool cpu_step();
u16 cpu_read_reg(reg_type);
void cpu_set_reg(reg_type, u16);
void fetch_data();
IN_PROC inst_get_processor(in_type type);