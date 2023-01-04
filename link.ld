ENTRY(_start)

MEMORY {
   RAM (RWX) : ORIGIN = 0x0000, LENGTH = 0x1800  /* 6kB RAM */
}

SECTIONS {
   everything : {
      . = ALIGN(4);
      build/start.o (.init)
      *(.*)
   } > RAM
}

