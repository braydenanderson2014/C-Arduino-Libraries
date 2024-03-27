MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 2M
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 320K
}

SECTIONS
{
    .text :
    {
        *(.text)
        *(.rodata)
        *(.text*)
    } > FLASH

    .data :
    {
        *(.data)
    } > RAM

    .bss :
    {
        *(.bss)
    } > RAM
}
