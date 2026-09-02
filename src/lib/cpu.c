#include <cpu.h>
#include <bus.h>

cpu_context cpu_ctx = {0};

void cpu_init()
{
    cpu_ctx.regs.pc = 0x100;
    cpu_ctx.regs.a = 0X01;
}

void fetch_instruction()
{
    cpu_ctx.cur_opcode = bus_read(cpu_ctx.regs.pc++);
    cpu_ctx.cur_inst = instruction_by_opcode(cpu_ctx.cur_opcode);
}

void execute()
{
    const IN_PROC proc = inst_get_processor(cpu_ctx.cur_inst->type);

    if (!proc) {
        NO_IMPL
    }

    proc (&cpu_ctx);
}

bool cpu_step()
{
    if (!cpu_ctx.halted)
    {
        const u16 pc = cpu_ctx.regs.pc;
        fetch_instruction();
        fetch_data();

        printf("%04X: %-7s (%02X %02X %02X) A: %02X B: %02X C: %02X\n",
            pc, inst_name(cpu_ctx.cur_inst->type), cpu_ctx.cur_opcode,
            bus_read(pc + 1), bus_read(pc + 2),
            cpu_ctx.regs.a, cpu_ctx.regs.b, cpu_ctx.regs.c);

        if (cpu_ctx.cur_inst == NULL) {
            printf("Instruction not known! %02X\n", cpu_ctx.cur_opcode);
        }

        execute();
    }
    return true;
}
