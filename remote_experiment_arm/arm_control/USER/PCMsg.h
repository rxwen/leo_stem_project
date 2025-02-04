#ifndef _PC_MSG_H_
#define _PC_MSG_H_
#include "include.h"

// 真
#define CMD_MULT_SERVO_MOVE 3 // 真真真真真
#define CMD_FULL_ACTION_RUN 6
#define CMD_FULL_ACTION_STOP 7
#define CMD_FULL_ACTION_ERASE 8
#define CMD_ACTION_DOWNLOAD 25

// 真真真
#define MEM_LOBOT_LOGO_BASE 0L         //"LOBOT"真真真真真真真FLASH
#define MEM_FRAME_INDEX_SUM_BASE 4096L // 真真真真真真真真真真真�256真真
#define MEM_ACT_FULL_BASE 8192L        // 真真真真真真真

// 真
#define ACT_SUB_FRAME_SIZE 64L // 真真真64真真
#define ACT_FULL_SIZE 16384L   // 16KB,真真真真14kb真

static bool UartRxOK(void);
void FlashEraseAll(void);
void McuToPCSendData(uint8 cmd, uint8 prm1, uint8 prm2);
void InitUart1(void);
void SaveAct(uint8 fullActNum, uint8 frameIndexSum, uint8 frameIndex, uint8* pBuffer);
void TaskPCMsgHandle(void);
void InitMemory(void);

#endif
