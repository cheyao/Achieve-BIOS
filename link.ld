ENTRY(_start)

MEMORY {
   RAM (RWX) : ORIGIN = 0x0000, LENGTH = 0x1800  /* 6kB RAM */
}

SECTIONS {
   . = 0;

   .text ALIGN(4): {
      build/start.o (.text)
      *(.text*)
      . = ALIGN(4);
   }

   .data ALIGN(4): {
      *(.data*)
      . = ALIGN(4);
   }

   .rodata ALIGN(4): {
      *(.rodata*)
      . = ALIGN(4);
   }

   .bss ALIGN(4): {
      *(.bss*)
      . = ALIGN(4);
   }
}

