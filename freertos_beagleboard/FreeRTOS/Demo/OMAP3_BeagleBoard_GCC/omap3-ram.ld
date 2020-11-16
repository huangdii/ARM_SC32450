OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY 
{
	loady	: ORIGIN = 0x80100000, LENGTH = 10M
}

__stack_end__ = 0x80100000 + 2560K - 4;

SECTIONS 
{
	. = 0;
	startup : { *(.startup)} >loady

  .text  : 
  {
  	*boot.o(.text)
    *(.text)     
    . = ALIGN(4);

  } >loady

  .rodata :
  {
    rodata_start_addr = .;

    *(.rodata)
    *(.rodata.*)
    . = ALIGN(4);
       
     rodata_end_addr = .; 
  } >loady

  .data :
  {
    data_start_addr = .;
    __DATA_START__ = .;
    
    *(.data)
    *(.data.*)
    . = ALIGN(8);

    data_end_addr = .;
    __DATA_END__ = .; 
    
  } >loady
  
  .bss :
  {
    bss_start_addr = .;
    __BSS_START__ = .;	    

    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(8);
    bss_end_addr = .;
    __BSS_END__ = .;
    
  } >loady
  end = .;
  PROVIDE(end = .);
 }
