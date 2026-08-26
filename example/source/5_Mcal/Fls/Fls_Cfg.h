/*******************************************************
  * Name    :Fls_Cfg.h
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _FLS_CFG_H_
#define _FLS_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "hc32_ll.h"
/*-------------define---------------------------------*/
#define FLASH_BASE        (EFM_START_ADDR)
#define FLASH_SIZE        (EFM_END_ADDR + 1U)
#define FLASH_SECTOR_SIZE (SECTOR_SIZE)
#define FLASH_SECTOR_NUM  (64U)
/* Vector table */
#define VECT_TAB_STEP     (0x400UL)
#endif

