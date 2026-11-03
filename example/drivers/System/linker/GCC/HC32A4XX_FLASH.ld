/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: HC32A4XX.ld
 *
 * Description: Implementation of SYSTEM
 *
 *
 * (c) Copyright 2023 - 2026 XIAOHUA SEMICONDUCTOR CO., LTD. All rights reserved.
 *
 * This software is licensed by XHSC PLATFORM SOFTWARE LICENSE AGREEMENT (the "License"). By downloading, installing, or
 * using the Software, you indicate that you accept the terms of the License, and you acknowledge that you have the
 * authority, for yourself or on behalf of your company, to bind your company to these terms. If you do not agree to all
 * of the terms of this License, you should not download or install the Software.
 *
 **********************************************************************************************************************/

ENTRY(Reset_Handler)

MEMORY
{
    init_flash_vector        : ORIGIN = 0x00000000, LENGTH = 0x00000280    /* 640B */
    init_flash_fls_rsv       : ORIGIN = 0x0000046C, LENGTH = 0x00000594    /* 1428B for acfls  */    
    init_flash               : ORIGIN = 0x00000A00, LENGTH = 0x001FF694    /* 2045.64KB */
    init_sramh               : ORIGIN = 0x1FFE0000, LENGTH = 0x0001F000    /* 124KB  */
    init_sramh_stack         : ORIGIN = 0x1FFFF000, LENGTH = 0x00001000    /* 4KB  */    
    init_sram_fls_rsv        : ORIGIN = 0x20000000, LENGTH = 0x00000594    /* 1428B for acfls ram */
    init_sram1_4             : ORIGIN = 0x20000594, LENGTH = 0x0005FA6C    /* 382.6KB */
    init_icg0                : ORIGIN = 0x00000400, LENGTH = 0x00000004    /* 4B */
    init_icg1                : ORIGIN = 0x00000404, LENGTH = 0x00000004    /* 4B */
    init_icg3                : ORIGIN = 0x0000040c, LENGTH = 0x00000004    /* 4B */
    init_icg8a               : ORIGIN = 0x00000460, LENGTH = 0x00000004    /* 4B */
    init_icg8b               : ORIGIN = 0x00000464, LENGTH = 0x00000004    /* 4B */
    init_icg8c               : ORIGIN = 0x00000468, LENGTH = 0x00000004    /* 4B */
    ram_end                  : ORIGIN = 0x2005FFFF, LENGTH = 0x00000000
}

SECTIONS
{
    .vector_table :
    {
        . = ALIGN(1024);        
        __code_start = .;        
        __init_vectortable_start = .;
        KEEP(*(.intvec))
        . = ALIGN(4);
        __init_vectortable_end = .;

    } > init_flash_vector
    
    .flash :
    {       
        *(.startup)
        . = ALIGN(4);
        *(.systeminit)
        . = ALIGN(4);
        *(.text.startup)
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
        *(.mcal_text.*)
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        *(.mcal_const_cfg.*)
        . = ALIGN(4);
        *(.mcal_const.*)
        . = ALIGN(4);
        __init_table = .;
        KEEP(*(.init_table))
        . = ALIGN(4);
        __init_table_end = .;
        __zero_table = .;
        KEEP(*(.zero_table))
        . = ALIGN(4);
        __zero_table_end = .;
        KEEP(*(.init))
        . = ALIGN(4);
        KEEP(*(.fini))
        . = ALIGN(4);
    } > init_flash

    .ARM.exidx :
    {
        *(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
    } > init_flash
    
    __init_mcaltextfast_start__ = .;

    .vectortable :
    {
        . = ALIGN(1024);
        __vectortable_start = .;
        . += __init_vectortable_end - __init_vectortable_start;
        . = ALIGN(4);
        __vectortable_end = .;
    } > init_sramh

    .mcaltextfast_sramh :
    {
        __mcaltextfast_start = .;
        *(.mcal_text.fast.*)
        . = ALIGN(4);
        __mcaltextfast_end = .;
    } > init_sramh AT > init_flash
    __init_mcaltextfast_end__ = __init_mcaltextfast_start__ + (__mcaltextfast_end - __mcaltextfast_start);

    .mcaltext_flash :
    {
        *(.mcal_text.*)
        . = ALIGN(4);
    } > init_flash

    __init_mcaldatafast_start__ = .;

    .acfls_code_rom :
    {
        __acfls_code_rom_start = .;
        KEEP(*(.acfls_code_rom))
        . = ALIGN(4);
        __acfls_code_rom_end = .;
    } > init_flash_fls_rsv

    .mcaldatafast_sramh : AT(__init_mcaldatafast_start__)
    {
        __mcaldatafast_start = .;
        *(.mcal_data.fast.*)
        . = ALIGN(4);
        __mcaldatafast_end = .;
    } > init_sramh
    __init_mcaldatafast_end__ = __init_mcaldatafast_start__ + (__mcaldatafast_end - __mcaldatafast_start);

    __init_data_start__ = __init_mcaldatafast_end__;    
    .data_ram : AT(__init_data_start__)
    {
        __data_start = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        *(.mcal_data.*)
        . = ALIGN(4);
        __data_end = .;
    } > init_sram1_4
    __init_data_end__ = __init_data_start__ + (__data_end - __data_start);

    .mcalbssfast_sramh (NOLOAD):
    {
        __mcalbssfast_start = .;
        *(.mcal_bss.fast.*)
        . = ALIGN(4);
        __mcalbssfast_end = .;
    } > init_sramh
    
    .bss_ram (NOLOAD):
    {
        . = ALIGN(16);
        __sram_bss_start = .;
        *(.bss)
        *(.bss.*)
        . = ALIGN(16);
        *(.mcal_bss.*)
        . = ALIGN(4);
        __sram_bss_end = .;
    } > init_sram1_4

    .acfls_code_ram :
    {
        acfls_code_ram_start  = .;
        *(.acfls_code_ram)
        acfls_code_ram_stop   = .;
    } > init_sram_fls_rsv

    .icg0_sec :
    {
        KEEP(*(.icg0_sec))
    } > init_icg0

    .icg1_sec :
    {
        KEEP(*(.icg1_sec))
    } > init_icg1

    .icg3_sec :
    {
        KEEP(*(.icg3_sec))
    } > init_icg3

    .icg8a_sec :
    {
        KEEP(*(.icg8a_sec))
    } > init_icg8a

    .icg8b_sec :
    {
        KEEP(*(.icg8b_sec))
    } > init_icg8b

    .icg8c_sec :
    {
        KEEP(*(.icg8c_sec))
    } > init_icg8c

    __STACK_END                 = ORIGIN(init_sramh_stack);
    __STACK_START               = ORIGIN(init_sramh_stack) + LENGTH(init_sramh_stack);

    __INIT_SRAM_START           = ORIGIN(init_sram_fls_rsv);
    __INIT_SRAM_END             = ORIGIN(ram_end);

    __VECTORTABLE_START         = __vectortable_start;
    __INIT_VECTORTABLE_START    = __init_vectortable_start;
    __INIT_VECTORTABLE_END      = __init_vectortable_end;
    __MCALTEXTFAST_START        = __mcaltextfast_start;
    __INIT_MCALTEXTFAST_START   = __init_mcaltextfast_start__;
    __INIT_MCALTEXTFAST_END     = __init_mcaltextfast_end__;    
    __DATA_START                = __data_start;
    __INIT_DATA_START           = __init_data_start__;
    __INIT_DATA_END             = __init_data_end__;
    __MCALDATAFAST_START        = __mcaldatafast_start;
    __INIT_MCALDATAFAST_START   = __init_mcaldatafast_start__;
    __INIT_MCALDATAFAST_END     = __init_mcaldatafast_end__;    
    __BSS_START                 = __sram_bss_start;
    __BSS_END                   = __sram_bss_end; 
    __BSS_MCALBSSFAST_START     = __mcalbssfast_start;
    __BSS_MCALBSSFAST_END       = __mcalbssfast_end;       
    __INIT_TABLE                = __init_table;
    __INIT_TABLE_END            = __init_table_end;
    __ZERO_TABLE                = __zero_table;
    __ZERO_TABLE_END            = __zero_table_end;

    __RAM_STACK_START           = ORIGIN(init_sramh_stack);
    
    __FLS_AC_ERASE_START_ADDR_IN_ROM         = __acfls_code_rom_start;
    __FLS_AC_ERASE_END_ADDR_IN_ROM           = __acfls_code_rom_end;
    __FLS_AC_ERASE_CODE_SIZE_IN_ROM          = (__acfls_code_rom_end - __acfls_code_rom_start)/4;

    __FLS_AC_WRITE_START_ADDR_IN_ROM         = __acfls_code_rom_start;
    __FLS_AC_WRITE_END_ADDR_IN_ROM           = __acfls_code_rom_end;
    __FLS_AC_WRITE_CODE_SIZE_IN_ROM          = (__acfls_code_rom_end - __acfls_code_rom_start)/4;

    __FLS_AC_BLANKREAD_START_ADDR_IN_ROM     = __acfls_code_rom_start;
    __FLS_AC_BLANKREAD_END_ADDR_IN_ROM       = __acfls_code_rom_end;
    __FLS_AC_BLANKREAD_CODE_SIZE_IN_ROM      = (__acfls_code_rom_end - __acfls_code_rom_start)/4;
    
    __FLS_AC_ERASE_FUNC_PTR_IN_RAM           = ADDR(.acfls_code_ram);
    __FLS_AC_WRITE_FUNC_PTR_IN_RAM           = ADDR(.acfls_code_ram);
    __FLS_AC_BLANKREAD_FUNC_PTR_IN_RAM       = ADDR(.acfls_code_ram);

    __INIT_SRAMH_START                     = ORIGIN(init_sramh);
    __INIT_SRAMH_END                       = ORIGIN(init_sramh) + LENGTH(init_sramh);

    __RAM_INIT                               = 1;
}
