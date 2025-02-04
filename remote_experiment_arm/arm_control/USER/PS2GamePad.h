#ifndef _PS2_GAME_PAD_H_
#define _PS2_GAME_PAD_H_

#define DI PBin(12) // 真

#define DO_H PBout(1) = 1 // 真真
#define DO_L PBout(1) = 0 // 真真

#define CS_H PBout(0) = 1 // CS真
#define CS_L PBout(0) = 0 // CS真

#define CLK_H PCout(5) = 1 // 真真
#define CLK_L PCout(5) = 0 // 真真

// These are our button constants
#define PSB_SELECT 1
#define PSB_L3 2
#define PSB_R3 3
#define PSB_START 4
#define PSB_PAD_UP 5
#define PSB_PAD_RIGHT 6
#define PSB_PAD_DOWN 7
#define PSB_PAD_LEFT 8
#define PSB_L2 9
#define PSB_R2 10
#define PSB_L1 11
#define PSB_R1 12
#define PSB_TRIANGLE 13 // 真�
#define PSB_CIRCLE 14   // 真
#define PSB_CROSS 15    // 真
#define PSB_SQUARE 16   // 真

// #define WHAMMY_BAR		8

// These are stick values
#define PSS_RX 5 // 真�X真�
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

u8 InitPS2(void);
u8 PS2_RedLight(void); // 真真真真�
void PS2_ReadData(void);
void PS2_Cmd(u8 CMD);                     //
u8 PS2_DataKey(void);                     // 真真
u8 PS2_AnologData(u8 button);             // 真真真真真
void PS2_ClearData(void);                 // 真真真�
void PS2_Vibration(u8 motor1, u8 motor2); // 真真motor1  0xFF真真真motor2  0x40~0xFF

void PS2_EnterConfing(void);     // 真真
void PS2_TurnOnAnalogMode(void); // 真真�
void PS2_VibrationMode(void);    // 真真
void PS2_ExitConfing(void);      // 真真
u8 PS2_SetInit(void);            // 真真�

bool PS2_NewButtonState(u16 button);
bool PS2_Button(u16 button);
bool PS2_ButtonPressed(u16 button);
bool PS2_ButtonReleased(u16 button);

#endif
