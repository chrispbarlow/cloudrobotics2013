
/**********************************************
    GENERATED ON EACH BUILD - DO NOT MODIFY!
 **********************************************/

MEMORY
{
	Flash (rx) : ORIGIN = 0x0,        LENGTH = 4096K
	Code  (rw) : ORIGIN = 0x20000000, LENGTH = 32K
	Data  (rw) : ORIGIN = 0x40000000, LENGTH = 16K
}

SECTIONS
{
	_stext = . ;
	PROVIDE (stext = .);
	
	.text : AT (ORIGIN(Flash))
	{
		_text = . ;
		KEEP(*(.startup))
		*(.text .text.* .gnu.linkonce.t.*)
		*(.init .init.*)
		*(.fini .fini.*)
	} >Code =0

	. = ALIGN(4);

	_etext = . ;
	PROVIDE (etext = .);
	
	.data : AT (ORIGIN(Flash) + SIZEOF(.text))
	{
		_data = . ;
		PROVIDE (data = .);
		KEEP(*(.sdata.config* .data.config*))
		*(.rodata .rodata.*)
		*(.data .data.* .gnu.linkonce.d.*)
		*(.eh_frame .eh_frame.*)
		*(.rdata .rdata.*)
		*(.jcr .jcr.*)
		*(.sdata .sdata.*)
	} >Data

	. = ALIGN(4);

  	_edata = . ;
	PROVIDE (edata = .);

  	_gp = ABSOLUTE(.);

	.bss (NOLOAD) :
	{
		_bss = . ;
		PROVIDE (bss = .);
		*(.sbss .sbss.*)
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(.scommon .scommon.*)
		*(COMMON)
	} >Data

	_ebss = . ;
	PROVIDE (ebss = .);

	.log (NOLOAD) : 
	{
		KEEP(*(.log))
	} >Data

	_end = . ;
	PROVIDE (end = .);
	PROVIDE (_heap_start = .);


.stack (ORIGIN(Data) + LENGTH(Data) - (2048 + 512)) (NOLOAD) :
	{
		PROVIDE (_heap_end = .);
		KEEP(*(.stack .stack.*))
	} >Data
}
