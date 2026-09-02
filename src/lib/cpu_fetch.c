#include <cpu.h>
#include <bus.h>

#include <emu.h>


extern cpu_context cpu_ctx;

void fetch_data()
{
    cpu_ctx.mem_dest = 0;
    cpu_ctx.dest_is_mem = false;

    if (cpu_ctx.cur_inst == NULL) {
        return;
    }

    const reg_type reg_1 = cpu_ctx.cur_inst->reg_1;
    const reg_type reg_2 = cpu_ctx.cur_inst->reg_2;

    switch (cpu_ctx.cur_inst->mode)
    {
        case AM_IMP:
            return;

        // --- operand is a register value ---
        case AM_R:
            cpu_ctx.fetched_data = cpu_read_reg(reg_1);
            return;

        case AM_R_R:
            cpu_ctx.fetched_data = cpu_read_reg(reg_2);
            return;

        // --- operand is one immediate byte from the program stream ---
        case AM_R_D8:
        case AM_D8:
        case AM_R_A8:
        case AM_HL_SPR:
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc += 1;
            return;

        // --- operand is an immediate word from the program stream ---
        case AM_R_D16:
        case AM_D16: {
            const u16 lo = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            const u16 hi = bus_read(cpu_ctx.regs.pc + 1);
            emu_cycles(1);
            cpu_ctx.fetched_data = lo | (hi << 8);
            cpu_ctx.regs.pc += 2;
            return;
        }

        // --- operand read from memory at a register-held address ---
        case AM_R_MR: {
            u16 addr = cpu_read_reg(reg_2);
            if (reg_1 == RT_C) {
                addr |= 0xFF00;
            }
            cpu_ctx.fetched_data = bus_read(addr);
            emu_cycles(1);
            return;
        }

        case AM_MR: {
            const u16 addr = cpu_read_reg(reg_1);
            cpu_ctx.mem_dest = addr;
            cpu_ctx.dest_is_mem = true;
            cpu_ctx.fetched_data = bus_read(addr);
            emu_cycles(1);
            return;
        }

        case AM_R_HLI:
            cpu_ctx.fetched_data = bus_read(cpu_read_reg(reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;

        case AM_R_HLD:
            cpu_ctx.fetched_data = bus_read(cpu_read_reg(reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;

        // --- operand read from an absolute address in the program stream ---
        case AM_R_A16: {
            const u16 lo = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            const u16 hi = bus_read(cpu_ctx.regs.pc + 1);
            emu_cycles(1);
            cpu_ctx.regs.pc += 2;
            cpu_ctx.fetched_data = bus_read(lo | (hi << 8));
            emu_cycles(1);
            return;
        }

        // --- result written to memory at a register-held address ---
        case AM_MR_R:
            cpu_ctx.fetched_data = cpu_read_reg(reg_2);
            cpu_ctx.mem_dest = cpu_read_reg(reg_1);
            cpu_ctx.dest_is_mem = true;
            if (reg_1 == RT_C) {
                cpu_ctx.mem_dest |= 0xFF00;
            }
            return;

        case AM_HLI_R:
            cpu_ctx.fetched_data = cpu_read_reg(reg_2);
            cpu_ctx.mem_dest = cpu_read_reg(reg_1);
            cpu_ctx.dest_is_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;

        case AM_HLD_R:
            cpu_ctx.fetched_data = cpu_read_reg(reg_2);
            cpu_ctx.mem_dest = cpu_read_reg(reg_1);
            cpu_ctx.dest_is_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;

        case AM_MR_D8:
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc += 1;
            cpu_ctx.mem_dest = cpu_read_reg(reg_1);
            cpu_ctx.dest_is_mem = true;
            return;

        // --- result written to memory at an address from the program stream ---
        case AM_A8_R:
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc) | 0xFF00;
            cpu_ctx.dest_is_mem = true;
            emu_cycles(1);
            cpu_ctx.regs.pc += 1;
            return;

        case AM_A16_R:
        case AM_D16_R: {
            const u16 lo = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            const u16 hi = bus_read(cpu_ctx.regs.pc + 1);
            emu_cycles(1);
            cpu_ctx.mem_dest = lo | (hi << 8);
            cpu_ctx.dest_is_mem = true;
            cpu_ctx.regs.pc += 2;
            cpu_ctx.fetched_data = cpu_read_reg(reg_2);
            return;
        }

        default:
            printf("Addressing mode not known (%d) (%02X)\n",
                   cpu_ctx.cur_inst->mode, cpu_ctx.cur_opcode);
            exit(-7);
    }
}