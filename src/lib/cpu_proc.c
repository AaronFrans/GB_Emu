#include <cpu.h>
#include <emu.h>

#include "bus.h"


// process CPU instructions...

// ------ HELPERS ------

//Check zero and carry flags and compare them
//to the current instructions requirement
static bool check_cond(const cpu_context *ctx) {
    const bool z = CPU_FLAG_Z;
    const bool c = CPU_FLAG_C;

    switch (ctx->cur_inst->cond) {
        case CT_NONE:
            return true;
        case CT_C:
            return c;
        case CT_NC:
            return !c;
        case CT_Z:
            return z;
        case CT_NZ:
            return !z;
    }

    return false;
}

// Set the flags for the cpu
static void cpu_set_flags(cpu_context *ctx, const int8_t z, const int8_t n, const int8_t h, const int8_t c) {
    if (z != -1) {
        BIT_SET(ctx->regs.f, 7, z);
    }
    if (n != -1) {
        BIT_SET(ctx->regs.f, 6, n);
    }
    if (h != -1) {
        BIT_SET(ctx->regs.f, 5, h);
    }
    if (c != -1) {
        BIT_SET(ctx->regs.f, 4, c);
    }
}


// ------ INSTRUCTIONS ------

//Used for invalid instructions, should not be called
static void proc_none(cpu_context *ctx) {
    printf("INVALID INSTRUCTION!\n");
    exit(-7);
}

// No-Op -> Does nothing
static void proc_nop(cpu_context *ctx) {
}

// Disables IME (interrupt master enable) flag
static void proc_di(cpu_context *ctx) {
    ctx->int_master_enabled = false;
}

// Load fetched data into register 1
static void proc_ld(cpu_context *ctx) {
    if (ctx->dest_is_mem) {
        // LD (BC), A for instance...

        if (ctx->cur_inst->reg_2 >= RT_AF) {
            //if 16 bit register...
            emu_cycles(1);
            bus_write16(ctx->mem_dest, ctx->fetched_data);
        }
        else {
            bus_write(ctx->mem_dest, ctx->fetched_data);
        }

        return;
    }

    if (ctx->cur_inst->mode == AM_HL_SPR) {
        const u8 hflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xF) +
            (ctx->fetched_data & 0xF) >= 0x10;

        const u8 cflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xFF) +
            (ctx->fetched_data & 0xFF) >= 0x100;

        cpu_set_flags(ctx, 0, 0, hflag, cflag);
        cpu_set_reg(ctx->cur_inst->reg_1, cpu_read_reg(ctx->cur_inst->reg_2) + (char)ctx->fetched_data);
    }

    cpu_set_reg(ctx->cur_inst->reg_1, ctx->fetched_data);
}

// XOR register A with the lower byte of the fetched data
// Then sets the zero flag, and clears the other flags
static void proc_xor(cpu_context *ctx) {
    ctx->regs.a ^= ctx->fetched_data & 0x00FF;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

// Jumps to the address in fetched data,
// if the instruction's condition is met.
static void proc_jp(cpu_context *ctx) {
    if (check_cond(ctx)) {
        ctx->regs.pc = ctx->fetched_data;
        emu_cycles(1);
    }
}

static IN_PROC processors[] = {
    [IN_NONE] = proc_none,
    [IN_NOP] = proc_nop,
    [IN_LD] = proc_ld,
    [IN_JP] = proc_jp,
    [IN_DI] = proc_di,
    [IN_XOR] = proc_xor,
};



IN_PROC inst_get_processor(const in_type type) {
    return processors[type];
}