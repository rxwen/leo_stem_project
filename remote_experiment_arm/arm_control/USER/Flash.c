#include "include.h"

/************************************************
SPI¿¿¿
¿¿¿¿: ¿
¿¿¿¿: ¿
************************************************/
u8 SPIx_ReadWriteByte(u8 TxData);
SPI_InitTypeDef SPI_InitStructure;
void InitSpi(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; // SPI CS
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ¿¿¿¿¿¿
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // ¿¿SPI¿¿¿¿¿¿¿¿¿¿¿:SPI¿¿¿¿¿¿¿¿¿¿
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      // ¿¿SPI¿¿¿¿:¿¿¿¿SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  // ¿¿SPI¿¿¿¿¿:SPI¿¿¿¿8¿¿¿¿
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                        // ¿¿¿¿¿¿¿¿¿¿:¿¿¿¿¿
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                       // ¿¿¿¿¿¿¿¿¿¿¿
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // NSS¿¿¿¿¿¿NSS¿¿¿¿¿¿¿¿¿¿SSI¿¿¿¿:¿¿NSS¿¿¿SSI¿¿¿
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // ¿¿¿¿¿¿¿¿¿¿:¿¿¿¿¿¿¿¿256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 // ¿¿¿¿¿¿¿MSB¿¿¿LSB¿¿¿:¿¿¿¿¿MSB¿¿¿
    SPI_InitStructure.SPI_CRCPolynomial = 7;                           // CRC¿¿¿¿¿¿¿
    SPI_Init(SPI1, &SPI_InitStructure);                                // ¿¿SPI_InitStruct¿¿¿¿¿¿¿¿¿¿¿SPIx¿¿¿

    SPI_Cmd(SPI1, ENABLE); // ¿¿SPI¿¿

    SPIx_ReadWriteByte(0xff); // ¿¿¿¿
}

void InitFlash(void)
{
    InitSpi();
    DelayMs(150);
}

// /************************************************
// ¿¿SPI¿¿¿Flash¿¿¿¿¿¿
// ¿¿¿¿:
//     dat : ¿¿¿¿¿¿¿
// ¿¿¿¿:
//     ¿Flash¿¿¿¿¿¿
// ************************************************/
// BYTE SpiShift(BYTE dat)
// {
//     SPDAT = dat;                                //¿¿SPI¿¿
//     while (!(SPSTAT & SPIF));                   //¿¿SPI¿¿¿¿¿¿
//     SPSTAT = SPIF | WCOL;                       //¿¿SPI¿¿
//
//     return SPDAT;
// }

u8 SPIx_ReadWriteByte(u8 TxData)
{
    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) // ¿¿¿¿¿SPI¿¿¿¿¿¿¿:¿¿¿¿¿¿¿¿
    {
        retry++;
        if (retry > 200)
            return 0;
    }
    SPI_I2S_SendData(SPI1, TxData); // ¿¿¿¿SPIx¿¿¿¿¿¿
    retry = 0;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
        ; // ¿¿¿¿¿SPI¿¿¿¿¿¿¿:¿¿¿¿¿¿¿¿¿
    {
        retry++;
        if (retry > 200)
            return 0;
    }
    return SPI_I2S_ReceiveData(SPI1); // ¿¿¿¿SPIx¿¿¿¿¿¿¿
}

// /************************************************
// ¿¿Flash¿¿¿¿
// ¿¿¿¿: ¿
// ¿¿¿¿:
//     0 : Flash¿¿¿¿¿¿
//     1 : Flash¿¿¿¿¿
// ************************************************/
// BOOL IsFlashBusy()
// {
//     BYTE dat;
//
//     SS = 0;
//     SpiShift(SFC_RDSR);                         //¿¿¿¿¿¿¿¿
//     dat = SpiShift(0);                          //¿¿¿¿
//     SS = 1;
//
//     return (dat & 0x01);                        //¿¿¿¿Bit0¿¿¿¿¿
// }

u8 SPI_Flash_ReadSR(void)
{
    u8 byte = 0;
    SPI_FLASH_CS = 0;                       // ¿¿¿¿
    SPIx_ReadWriteByte(W25X_ReadStatusReg); // ¿¿¿¿¿¿¿¿¿¿¿
    byte = SPIx_ReadWriteByte(0Xff);        // ¿¿¿¿¿¿
    SPI_FLASH_CS = 1;                       // ¿¿¿¿
    return byte;
}

// ¿¿¿¿
void SPI_Flash_Wait_Busy(void)
{
    while ((SPI_Flash_ReadSR() & 0x01) == 0x01)
        ; // ¿¿BUSY¿¿¿
}
// /************************************************
// ¿¿Flash¿¿¿
// ¿¿¿¿: ¿
// ¿¿¿¿: ¿
// ************************************************/
// void FlashWriteEnable()
// {
//     while (IsFlashBusy());                      //Flash¿¿¿
//     SS = 0;
//     SpiShift(SFC_WREN);                         //¿¿¿¿¿¿¿
//     SS = 1;
// }

// SPI_FLASH¿¿¿
// ¿WEL¿¿
void SPI_FLASH_Write_Enable(void)
{
    SPI_FLASH_CS = 0;                     // ¿¿¿¿
    SPIx_ReadWriteByte(W25X_WriteEnable); // ¿¿¿¿¿
    SPI_FLASH_CS = 1;                     // ¿¿¿¿
}

// /************************************************
// ¿¿¿¿Flash
// ¿¿¿¿: ¿
// ¿¿¿¿: ¿
// ************************************************/
// void FlashErase()
// {
//     FlashWriteEnable();                     //¿¿Flash¿¿¿
//     SS = 0;
//     SpiShift(SFC_CHIPER);                   //¿¿¿¿¿¿¿
//     SS = 1;
// }

/************************************************
¿¿¿¿ ¿¿¿¿4096,Flash¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
¿¿¿¿:
        addr
¿¿¿¿: ¿
************************************************/
void FlashEraseSector(DWORD addr)
{
    SPI_FLASH_Write_Enable(); // SET WEL
    SPI_Flash_Wait_Busy();
    SPI_FLASH_CS = 0;                       // ¿¿¿¿
    SPIx_ReadWriteByte(W25X_SectorErase);   // ¿¿¿¿¿¿¿¿
    SPIx_ReadWriteByte((u8)((addr) >> 16)); // ¿¿24bit¿¿
    SPIx_ReadWriteByte((u8)((addr) >> 8));
    SPIx_ReadWriteByte((u8)addr);
    SPI_FLASH_CS = 1;      // ¿¿¿¿
    SPI_Flash_Wait_Busy(); // ¿¿¿¿¿¿
}

/************************************************
¿Flash¿¿¿¿¿
¿¿¿¿:
    addr   : ¿¿¿¿
    size   : ¿¿¿¿¿
    buffer : ¿¿¿Flash¿¿¿¿¿¿
¿¿¿¿:
    ¿
************************************************/
void FlashRead(DWORD addr, DWORD size, BYTE* buffer)
{
    u16 i;
    SPI_FLASH_CS = 0;                       // ¿¿¿¿
    SPIx_ReadWriteByte(W25X_ReadData);      // ¿¿¿¿¿¿
    SPIx_ReadWriteByte((u8)((addr) >> 16)); // ¿¿24bit¿¿
    SPIx_ReadWriteByte((u8)((addr) >> 8));
    SPIx_ReadWriteByte((u8)addr);
    for (i = 0; i < size; i++) {
        buffer[i] = SPIx_ReadWriteByte(0XFF); // ¿¿¿¿
    }
    SPI_FLASH_CS = 1; // ¿¿¿¿
}

/************************************************
¿¿¿¿Flash¿
¿¿¿¿:
    addr   : ¿¿¿¿
    size   : ¿¿¿¿¿
    buffer : ¿¿¿¿¿¿Flash¿¿¿
¿¿¿¿: ¿
************************************************/
void FlashWrite(DWORD addr, DWORD size, BYTE* buffer)
{
    u16 i;
    SPI_FLASH_Write_Enable();               // SET WEL
    SPI_FLASH_CS = 0;                       // ¿¿¿¿
    SPIx_ReadWriteByte(W25X_PageProgram);   // ¿¿¿¿¿¿
    SPIx_ReadWriteByte((u8)((addr) >> 16)); // ¿¿24bit¿¿
    SPIx_ReadWriteByte((u8)((addr) >> 8));
    SPIx_ReadWriteByte((u8)addr);
    for (i = 0; i < size; i++)
        SPIx_ReadWriteByte(buffer[i]); // ¿¿¿¿
    SPI_FLASH_CS = 1;                  // ¿¿¿¿
    SPI_Flash_Wait_Busy();             // ¿¿¿¿¿¿
}
