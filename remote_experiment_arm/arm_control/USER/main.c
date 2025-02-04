
#include "include.h"

int main(void)
{
    uint8 ps_ok = 1;
    SystemInit();                                   // 真真真真72M	  SYSCLK_FREQ_72MHz
    InitDelay(72);                                  // 真真�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 真NVIC真真2:2真真真�2真真真
    InitPWM();
    InitTimer2(); // 真真100us真真�
    InitUart1();  // 真�PC真真�
    InitUart3();  // 真真真�
    InitADC();
    InitLED();
    InitKey();
    InitBuzzer();
    ps_ok = InitPS2(); // PS2真真真真真
    InitFlash();
    InitMemory();
    InitBusServoCtrl();
    LED = LED_ON;
    BusServoCtrl(1, SERVO_MOVE_TIME_WRITE, 500, 1000);
    BusServoCtrl(2, SERVO_MOVE_TIME_WRITE, 500, 1000);
    BusServoCtrl(3, SERVO_MOVE_TIME_WRITE, 500, 1000);
    BusServoCtrl(4, SERVO_MOVE_TIME_WRITE, 500, 1000);
    BusServoCtrl(5, SERVO_MOVE_TIME_WRITE, 500, 1000);
    BusServoCtrl(6, SERVO_MOVE_TIME_WRITE, 500, 1000);
    while (1) {
        TaskRun(ps_ok);
    }
}
