ENTRY(_start)  

MEMORY {
    ROM (rx)  : ORIGIN = 0xFFFFFFFFFFFF0000, LENGTH = 0x0FFFF
    RAM (rwx) : ORIGIN = 0x0000000000000000, LENGTH = 0x10000000  
}

SECTIONS {
   . = 0xFFFFFFFFFFFF0000;

   .text ALIGN(8): {
      *(.text*)
      . = ALIGN(8);
   } > ROM

   .data ALIGN(8): {
      *(.data*)
      . = ALIGN(8);
   } > ROM

   .rodata ALIGN(8): {
      *(.rodata*)
      . = ALIGN(8);
   } > ROM

    . = 0x100000;
   .bss ALIGN(8): {
      *(.bss*)
      . = ALIGN(8);
   } > RAM
}
