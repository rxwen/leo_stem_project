#ifndef _PWM_H_
#define _PWM_H_

#define SERVO1 PCout(10) // ¿¿¿¿¿¿¿
#define SERVO2 PCout(11) // ¿¿¿¿¿¿¿
#define SERVO3 PCout(12)
#define SERVO4 PDout(2)
#define SERVO5 PBout(5)
#define SERVO6 PBout(8)

extern uint16 ServoPwmDutySet[];

extern bool ServoPwmDutyHaveChange;

void ServoSetPluseAndTime(uint8 id, uint16 p, uint16 time);
void ServoPwmDutyCompare(void); // ¿¿¿¿¿¿¿¿¿¿¿
void InitPWM(void);

#endif
