ENTRY(_start)

SECTIONS {
   . = 0xFFFFFFFFFFFF0000;

   .text ALIGN(8): {
      build/start.o (.text)
      *(.text*)
      . = ALIGN(8);
   }

   .data ALIGN(8): {
      *(.data*)
      . = ALIGN(8);
   }

   .rodata ALIGN(8): {
      *(.rodata*)
      . = ALIGN(8);
   }

   .bss ALIGN(8): {
      *(.bss*)
      . = ALIGN(8);
   }
}

