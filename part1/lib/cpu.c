#include <cpu.h>
#include <bus.h>
#include <emu.h>

cpu_context ctx = {0};

void cpu_init()
{
    ctx.regs.pc = 0x100;
}

void fetch_instruction()
{
    ctx.cur_opcode = bus_read(ctx.regs.pc++);
    ctx.cur_inst = instruction_by_opcode(ctx.cur_opcode);

    if (ctx.cur_inst == NULL)
    {
        printf("Instruction not known (%02X)\n", ctx.cur_opcode);
        exit(-7);
    }
}

void fetch_date()
{

    ctx.mem_dest = 0;
    ctx.dest_is_mem = false;

    switch (ctx.cur_inst->mode)
    {
    case AM_IMP:
        return;

    case AM_R:
        ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_1);
        return;

    case AM_R_D8:
        ctx.fetched_data = bus_read(ctx.regs.pc);
        emu_cycles(1);
        ctx.regs.pc++;
        return;

    case AM_D16:
    {
        u16 lo = bus_read(ctx.regs.pc);
        emu_cycles(1);
        u16 hi = bus_read(ctx.regs.pc + 1);
        ctx.fetched_data = lo | (hi << 8);
        ctx.regs.pc += 2;
        return;
    }

    default:
        printf("Addressing mode not known (%d)\n", ctx.cur_inst->mode);
        exit(-7);
    }
}

void execute()
{
    printf("    Not executing yet... \n");
}

bool cpu_step()
{

    printf("CPU Halted: %d\n", ctx.halted);
    if (!ctx.halted)
    {
        u16 pc = ctx.regs.pc;
        fetch_instruction();
        fetch_date();

        printf("Executing Instruction: %02X   PC: %04X\n", ctx.cur_opcode, pc);

        execute();
    }
    return true;
}
