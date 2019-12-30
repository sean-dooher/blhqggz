OUTPUT_ARCH( "riscv" )

ENTRY( _start )

PAGE_SIZE = 4K;

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

    .bss (NOLOAD) : ALIGN(PAGE_SIZE) {
      . = ALIGN(16);
      PROVIDE(__bss_start = .);
      *(.bss .bss.*);
      *(COMMON);
      PROVIDE(__bss_end = .);
    }

    . = ALIGN(PAGE_SIZE);

    PROVIDE (_free_mem_start = .);
    . = . + PAGE_SIZE * 30K;
    PROVIDE (_free_mem_end = .);
    
    PROVIDE (_mem_end = .);

   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }

}
