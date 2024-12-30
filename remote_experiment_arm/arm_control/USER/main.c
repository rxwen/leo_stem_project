
#include "include.h"

int main(void)
{
    uint8 ps_ok = 1;
    SystemInit();                                   // ¿¿¿¿¿¿¿¿72M	  SYSCLK_FREQ_72MHz
    InitDelay(72);                                  // ¿¿¿¿¿
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ¿¿NVIC¿¿¿¿2:2¿¿¿¿¿¿¿2¿¿¿¿¿¿
    InitPWM();
    InitTimer2(); // ¿¿¿¿100us¿¿¿¿¿
    InitUart1();  // ¿¿¿PC¿¿¿¿¿
    InitUart3();  // ¿¿¿¿¿¿¿
    InitADC();
    InitLED();
    InitKey();
    InitBuzzer();
    ps_ok = InitPS2(); // PS2¿¿¿¿¿¿¿¿¿¿
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
