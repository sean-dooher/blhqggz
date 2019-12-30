#include <stdio.h>
#include <stdint.h>

#include "paging.h"

extern uint8_t _free_mem_start;
extern uint8_t _free_mem_end;
extern uint8_t _text_base;
extern uint8_t _text_end;

const void *KERNEL_TEXT_BASE = &_text_base;
const void *KERNEL_TEXT_END = &_text_end;
const void *FREE_MEM_BASE = &_free_mem_start;
const void *FREE_MEM_END = &_free_mem_end;

void
vm_init_early () 
{

}