#include "include.h"

u16 Handkey;
u8 Comd[2] = { 0x01, 0x42 };                                           // ¿¿¿¿¿¿¿¿¿
u8 Data[9] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // ¿¿¿¿¿¿
u16 MASK[] = {
    PSB_SELECT,
    PSB_L3,
    PSB_R3,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1,
    PSB_TRIANGLE,
    PSB_CIRCLE,
    PSB_CROSS,
    PSB_SQUARE
}; // ¿¿¿¿¿¿¿

static void Delay(unsigned int time)
{
    // 	time *=6;
    // 	while(--time);
    DelayUs(time);
}

u8 InitPS2(void)
{
    u8 mode;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // ¿¿¿¿
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // ¿¿¿¿
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    mode = PS2_SetInit(); // ¿¿¿¿¿¿,¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
    return mode;
}

// ¿¿¿¿¿¿¿
void PS2_Cmd(u8 CMD)
{
    volatile u16 ref = 0x01;
    Data[1] = 0;
    for (ref = 0x01; ref < 0x0100; ref <<= 1) {
        if (ref & CMD) {
            DO_H; // ¿¿¿¿¿¿¿
        } else
            DO_L;

        Delay(10);
        CLK_L;
        Delay(40);
        CLK_H;
        if (DI)
            Data[1] = ref | Data[1];
        Delay(10);
    }
}
// ¿¿¿¿¿¿¿¿¿
// ¿¿¿¿0¿¿¿¿¿
//		  ¿¿¿¿¿¿¿
u8 PS2_RedLight(void)
{
    CS_L;
    PS2_Cmd(Comd[0]); // ¿¿¿¿
    PS2_Cmd(Comd[1]); // ¿¿¿¿
    CS_H;
    if (Data[1] == 0X73)
        return 0;
    else
        return 1;
}
// ¿¿¿¿¿¿
void PS2_ReadData(void)
{
    volatile u8 byte;
    volatile u16 ref;

    CS_L;
    Delay(10);
    PS2_Cmd(Comd[0]);                // ¿¿¿¿
    PS2_Cmd(Comd[1]);                // ¿¿¿¿
    for (byte = 2; byte < 9; byte++) // ¿¿¿¿¿¿
    {
        for (ref = 0x01; ref < 0x100; ref <<= 1) {

            CLK_L;
            Delay(50);
            CLK_H;
            if (DI) {
                Data[byte] = ref | Data[byte];
            }
            Delay(45);
        }
        Delay(10);
    }
    CS_H;
}

// ¿¿¿¿¿PS2¿¿¿¿¿¿¿      ¿¿¿¿¿¿¿¿         ¿¿¿¿¿¿¿¿¿  ¿¿¿¿¿¿¿¿¿
// ¿¿¿0¿ ¿¿¿¿1

u16 LastHandkey = 0xFFFF;
u8 PS2_DataKey()
{
    u8 index;

    PS2_ClearData();
    PS2_ReadData();
    LastHandkey = Handkey;
    Handkey = (Data[4] << 8) | Data[3]; // ¿¿16¿¿¿  ¿¿¿0¿ ¿¿¿¿1
    for (index = 0; index < 16; index++) {
        if ((Handkey & (1 << (MASK[index] - 1))) == 0)
            return index + 1;
    }
    return 0; // ¿¿¿¿¿¿¿¿
}

bool PS2_NewButtonState(u16 button)
{
    button = 0x0001u << (button - 1);                // ¿¿¿button¿¿¿ ¿¿¿¿¿¿¿¿¿bit¿¿+1¿ ¿¿ PSB_SELECT ¿¿¿ ¿ 1¿ ¿¿¿¿¿¿¿0¿¿ ¿¿¿¿¿
    return (((LastHandkey ^ Handkey) & button) > 0); // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿1¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
                                                     // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿1¿ ¿¿0¿¿¿¿¿¿¿true
}

bool PS2_Button(u16 button)
{
    button = 0x0001u << (button - 1);   // ¿¿¿button¿¿¿ ¿¿¿¿¿¿¿¿¿bit¿¿+1¿ ¿¿ PSB_SELECT ¿¿¿ ¿ 1¿ ¿¿¿¿¿¿¿0¿¿ ¿¿¿¿¿
    return (((~Handkey) & button) > 0); // ¿¿¿¿¿¿¿¿¿0¿¿¿¿¿1¿ ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿1¿¿¿¿¿0
                                        // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿1¿¿¿¿¿¿0¿¿¿¿0¿¿¿¿¿¿
}

bool PS2_ButtonPressed(u16 button)
{
    return (PS2_NewButtonState(button) && PS2_Button(button)); // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
}

bool PS2_ButtonReleased(u16 button)
{
    return (PS2_NewButtonState(button) && !PS2_Button(button)); // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
}

// ¿¿¿¿¿¿¿¿¿¿	 ¿¿0~256
u8 PS2_AnologData(u8 button)
{
    return Data[button];
}

// ¿¿¿¿¿¿¿
void PS2_ClearData()
{
    u8 a;
    for (a = 0; a < 9; a++)
        Data[a] = 0x00;
}
/******************************************************
Function:    void PS2_Vibration(u8 motor1, u8 motor2)
Description: ¿¿¿¿¿¿¿
Calls:		 void PS2_Cmd(u8 CMD);
Input: motor1:¿¿¿¿¿¿¿ 0x00¿¿¿¿¿
       motor2:¿¿¿¿¿¿¿ 0x40~0xFF ¿¿¿¿¿¿¿ ¿¿¿¿
******************************************************/
void PS2_Vibration(u8 motor1, u8 motor2)
{
    CS_L;
    Delay(50);
    PS2_Cmd(0x01); // ¿¿¿¿
    PS2_Cmd(0x42); // ¿¿¿¿
    PS2_Cmd(0X00);
    PS2_Cmd(motor1);
    PS2_Cmd(motor2);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    CS_H;
    Delay(50);
}
// short poll
void PS2_ShortPoll(void)
{
    CS_L;
    Delay(50);
    PS2_Cmd(0x01);
    PS2_Cmd(0x42);
    PS2_Cmd(0X00);
    PS2_Cmd(0x00);
    PS2_Cmd(0x00);
    CS_H;
    Delay(50);
}
// ¿¿¿¿
void PS2_EnterConfing(void)
{
    CS_L;
    Delay(50);
    PS2_Cmd(0x01);
    PS2_Cmd(0x43);
    PS2_Cmd(0X00);
    PS2_Cmd(0x01);
    PS2_Cmd(0x00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    CS_H;
    Delay(50);
}
// ¿¿¿¿¿¿
void PS2_TurnOnAnalogMode(void)
{
    CS_L;
    Delay(50);
    PS2_Cmd(0x01);
    PS2_Cmd(0x44);
    PS2_Cmd(0X00);
    PS2_Cmd(0x01); // analog=0x01;digital=0x00  ¿¿¿¿¿¿¿¿
    PS2_Cmd(0x03); // Ox03¿¿¿¿¿¿¿¿¿¿¿¿¿MODE¿¿¿¿¿¿
                   // 0xEE¿¿¿¿¿¿¿¿¿¿¿¿¿¿MODE¿¿¿¿¿¿
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    PS2_Cmd(0X00);
    CS_H;
    Delay(50);
}
// ¿¿¿¿
void PS2_VibrationMode(void)
{
    CS_L;
    Delay(50);
    PS2_Cmd(0x01);
    PS2_Cmd(0x4D);
    PS2_Cmd(0X00);
    PS2_Cmd(0x00);
    PS2_Cmd(0X01);
    CS_H;
    Delay(50);
}
// ¿¿¿¿¿¿¿
void PS2_ExitConfing(void)
{
    CS_L;
    Delay(50);
    PS2_Cmd(0x01);
    PS2_Cmd(0x43);
    PS2_Cmd(0X00);
    PS2_Cmd(0x00);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    PS2_Cmd(0x5A);
    CS_H;
    Delay(50);
}
// ¿¿¿¿¿¿¿
u8 PS2_SetInit(void)
{
    DelayMs(100);
    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_EnterConfing();     // ¿¿¿¿¿¿
    PS2_TurnOnAnalogMode(); // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
    PS2_VibrationMode();    // ¿¿¿¿¿¿
    PS2_ExitConfing();      // ¿¿¿¿¿¿¿
    if (PS2_RedLight() == 0)
        return 0;
    else
        return 1;
}
