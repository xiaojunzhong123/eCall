/*******************************************************
  * Name    :SecuAccs_Cfg.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "SecuAccs_Cfg.h"
/*-------------variable statement---------------------*/
uint16_t g_SecuAccsRand = 0;
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :SecuAccs_Lev1CalcSafeKey
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void SecuAccs_Lev1CalcSafeKey(uint8_t *seed, uint8_t *key)
{
    static unsigned char AppKeyConst[4];
    unsigned char        vKey1[4];
    unsigned char        vKey2[4] = { 0, 0, 0, 0 };
    unsigned char        i, j, vshift, vRshift, vLshift;
    unsigned long        tempkey = 0;
    AppKeyConst[0]               = 0x12;
    AppKeyConst[1]               = 0xDF;
    AppKeyConst[2]               = 0xE4;
    AppKeyConst[3]               = 0x3B;

    for (i = 4; i > 0; i--)
    {
        vKey1[i - 1] = (unsigned char)(seed[i - 1] ^ AppKeyConst[i - 1]);

        vRshift      = 0x80;
        vLshift      = 0x01;
        for (j = 0; j < 8; j++)
        {
            if (seed[4 - i] & vRshift)
            {
                vKey2[i - 1] |= vLshift;
            }
            vRshift >>= 1;
            vLshift <<= 1;
        }

        vKey2[i - 1]  = (unsigned char)(vKey2[i - 1] ^ AppKeyConst[i - 1]);
        vshift        = (unsigned char)((4 - i) << 3);
        tempkey      += (unsigned long)((unsigned long)((unsigned long)vKey1[i - 1] << (unsigned long)vshift) + (unsigned long)((unsigned long)vKey2[i - 1] << (unsigned long)vshift));
        key[i - 1]    = (unsigned char)(tempkey >> vshift);
    }
}

/*******************************************************
  * Name    :SecuAccs_Lev1GetSeedKey
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void SecuAccs_Lev1GetSeedKey(uint8_t *seed, uint8_t *key)
{
    uint8_t        i = 0;
    uint8_t        rand[4];
    static uint8_t randx       = 0;
    const uint8_t  randMask[4] = { 0x11, 0x22, 0x33, 0x44 };

    if (++randx >= 0xFF)
    {
        randx = 1;
    }
    rand[0] = (uint8_t)((g_SecuAccsRand >> 0) & 0xFF);
    rand[1] = (uint8_t)((g_SecuAccsRand >> 8) & 0xFF);
    rand[2] = (uint8_t)((randx));
    rand[3] = (uint8_t)((randx ^ rand[0]));
    for (i = 0; i < 4; i++)
    {
        seed[i] = (uint8_t)(rand[i] ^ randMask[i]);
    }
    SecuAccs_Lev1CalcSafeKey(seed, key);
}

/*-------------variable statement---------------------*/
const SecuAccs_Cfg_Type g_SecuAccsCfgList[SECUACCS_CFG_LIST_NUM] = {
  /* 1*/ {UDS_DCM_SEC_LEV_1, 0x01, 4, 4, 10000, 3, FALSE, SecuAccs_Lev1GetSeedKey}
};

