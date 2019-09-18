OUTPUT_ARCH( "riscv" )

ENTRY( _start )

SECTIONS
{
    . =  0x80000000;
    PROVIDE (_mem_start = .);
    .text : { 
      PROVIDE (_text_base = .);
      KEEP(*(.text.init));
      *(.text .text.* .gnu.linkonce.t*);
      PROVIDE (_text_end = .);
    }

    .rodata : {
      PROVIDE (_rodata_base = .);
      *(.rodata .rodata.* .gnu.linkonce.r*);
      PROVIDE (_rodata_end = .);
    }

    PROVIDE(_data = .);
    
    .data : {
      PROVIDE (_data_base = .);
      *(.data .data.* .gnu.linkonce.d*);
      PROVIDE (_data_end = .);
    }

    .bss (NOLOAD) : {
        . = ALIGN(16);
        PROVIDE(__bss_start = .);
        *(.bss .bss.*);
        *(COMMON);
        PROVIDE(__bss_end = .);
    }
    
    PROVIDE (_mem_end = .);

   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }

}
