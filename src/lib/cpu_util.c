#include <cpu.h>

extern cpu_context cpu_ctx;

u16 reverse(u16 n)
{
    return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
}

u16 cpu_read_reg(reg_type rt)
{
    switch (rt)
    {
    case RT_A:
        return cpu_ctx.regs.a;
    case RT_F:
        return cpu_ctx.regs.f;
    case RT_B:
        return cpu_ctx.regs.b;
    case RT_C:
        return cpu_ctx.regs.c;
    case RT_D:
        return cpu_ctx.regs.d;
    case RT_E:
        return cpu_ctx.regs.e;
    case RT_H:
        return cpu_ctx.regs.h;
    case RT_L:
        return cpu_ctx.regs.l;

    case RT_AF:
        return reverse(*((u16 *)&cpu_ctx.regs.a));
    case RT_BC:
        return reverse(*((u16 *)&cpu_ctx.regs.b));
    case RT_DE:
        return reverse(*((u16 *)&cpu_ctx.regs.d));
    case RT_HL:
        return reverse(*((u16 *)&cpu_ctx.regs.h));

    case RT_PC:
        return cpu_ctx.regs.pc;
    case RT_SP:
        return cpu_ctx.regs.sp;
    default:
        return 0;
    }
}

void cpu_set_reg(reg_type rt, u16 val) {
    switch (rt) {
        case RT_A:
            cpu_ctx.regs.a = val & 0xFF;
            break;
        case RT_F:
            cpu_ctx.regs.f = val & 0xFF;
            break;
        case RT_B:
            cpu_ctx.regs.b = val & 0xFF;
            break;
        case RT_C:
            cpu_ctx.regs.c = val & 0xFF;
            break;
        case RT_D:
            cpu_ctx.regs.d = val & 0xFF;
            break;
        case RT_E:
            cpu_ctx.regs.e = val & 0xFF;
            break;
        case RT_H:
            cpu_ctx.regs.h = val & 0xFF;
            break;
        case RT_L:
            cpu_ctx.regs.l = val & 0xFF;
            break;
        case RT_AF:
            *(u16 *)&cpu_ctx.regs.a = reverse(val);
            break;
        case RT_BC:
            *(u16 *)&cpu_ctx.regs.b = reverse(val);
            break;
        case RT_DE:
            *(u16 *)&cpu_ctx.regs.d = reverse(val);
            break;
        case RT_HL:
            *(u16 *)&cpu_ctx.regs.h = reverse(val);
            break;
        case RT_SP:
            cpu_ctx.regs.sp = val;
            break;
        case RT_PC:
            cpu_ctx.regs.pc = val;
            break;

        case RT_NONE:
        default:
            break;
    }
}