#include <bus.h>
#include <cart.h>

/*
    Game Boy Memory Map
    --------------------

    0000 - 3FFF : 16 KiB ROM Bank 00         // From cartridge, usually a fixed bank
    4000 - 7FFF : 16 KiB ROM Bank 01–NN      // From cartridge, switchable bank via mapper (if any)
    8000 - 9FFF : 8 KiB Video RAM (VRAM)     // In CGB mode, switchable bank 0/1
    A000 - BFFF : 8 KiB External RAM         // From cartridge, switchable bank if any
    C000 - CFFF : 4 KiB Work RAM (WRAM)
    D000 - DFFF : 4 KiB Work RAM (WRAM)      // In CGB mode, switchable bank 1–7
    E000 - FDFF : 7.5 KiB Echo RAM           // Mirror of C000–DDFF; use is prohibited
    FE00 - FE9F : 160 bytes OAM              // Object Attribute Memory (sprite attribute table)
    FEA0 - FEFF : 96 bytes Not Usable        // Nintendo prohibits use of this area
    FF00 - FF7F : 128 bytes I/O Registers
    FF80 - FFFE : 127 bytes High RAM (HRAM)
    FFFF         : 1 byte Interrupt Enable   // Interrupt Enable Register (IE)
*/

u8 bus_read(u16 address)
{
    if (address < 0x8000)
    {
        // ROM Date
        return cart_read(address);
    }

    NO_IMPL
}
void bus_write(u16 address, u8 value)
{
    if (address < 0x8000)
    {
        // ROM Date
        cart_write(address, 1);
        return;
    }

    NO_IMPL
}
