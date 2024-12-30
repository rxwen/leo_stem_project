#include "include.h"

bool fRobotRun = FALSE;        // ¿¿¿¿¿¿
uint8 ActFullNum = 0;          // ¿¿¿¿¿
uint32 ActFullRunTimesSum = 1; // ¿¿¿0¿¿¿¿¿

uint32 ActFullRunTimes = 0; // ¿¿¿¿¿¿¿¿¿

uint32 TimeActionRunTotal = 0; // ¿¿¿¿¿¿
bool fFrameRunFinish = TRUE;   // ¿¿¿¿¿¿¿¿¿

uint8 FrameIndexSum = 0; // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿
uint8 FrameIndex = 0;    // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿0¿¿

void FullActRun(uint8 actFullnum, uint32 times) // ¿¿¿¿¿¿¿¿¿¿
{
    uint8 frameIndexSum;
    FlashRead(MEM_FRAME_INDEX_SUM_BASE + actFullnum, 1, &frameIndexSum);
    if (frameIndexSum > 0) // ¿¿¿¿¿¿¿¿¿¿0¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
    {
        FrameIndexSum = frameIndexSum;
        if (ActFullNum != actFullnum) {
            if (actFullnum == 0) { // 0¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
                fRobotRun = FALSE;
                ActFullRunTimes = 0;
                fFrameRunFinish = TRUE;
            }
        } else { // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
            ActFullRunTimesSum = times;
        }

        if (FALSE == fRobotRun) {
            ActFullNum = actFullnum;
            ActFullRunTimesSum = times;
            FrameIndex = 0;
            ActFullRunTimes = 0;
            fRobotRun = TRUE;

            TimeActionRunTotal = gSystemTickCount;
        }
    }
}
void FullActStop(void)
{
    fRobotRun = FALSE;
    ActFullRunTimes = 0;

    fFrameRunFinish = TRUE;

    FrameIndex = 0;
}

uint16 ActSubFrameRun(uint8 fullActNum, uint8 frameIndex)
{
    uint32 i = 0;

    //	uint16 frameSumSum = 0;	//¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
    // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
    uint8 frame[ACT_SUB_FRAME_SIZE];
    uint8 servoCount;
    uint32 time;
    uint8 id;
    uint16 pos;

    FlashRead((MEM_ACT_FULL_BASE) + (fullActNum * ACT_FULL_SIZE) + (frameIndex * ACT_SUB_FRAME_SIZE), ACT_SUB_FRAME_SIZE, frame);

    servoCount = frame[0];
    time = frame[1] + (frame[2] << 8);

    if (servoCount > 8) { // ¿¿¿¿¿8¿¿¿¿¿¿¿¿¿¿¿
        FullActStop();
        return 0;
    }
    for (i = 0; i < servoCount; i++) {
        id = frame[3 + i * 3];
        pos = frame[4 + i * 3] + (frame[5 + i * 3] << 8);
        ServoSetPluseAndTime(id, pos, time);
        BusServoCtrl(id, SERVO_MOVE_TIME_WRITE, pos, time);
    }
    return time;
}

void TaskRobotRun(void)
{

    if (fRobotRun) {
        if (TRUE == fFrameRunFinish) { // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿
            fFrameRunFinish = FALSE;
            TimeActionRunTotal += ActSubFrameRun(ActFullNum, FrameIndex); // ¿¿¿¿¿¿¿¿¿¿¿
        } else {
            if (gSystemTickCount >= TimeActionRunTotal) { // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
                fFrameRunFinish = TRUE;
                if (++FrameIndex >= FrameIndexSum) { // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿
                    FrameIndex = 0;
                    if (ActFullRunTimesSum != 0) {                     // ¿¿¿¿¿¿¿¿0¿¿¿¿¿¿¿¿¿¿¿¿¿¿if¿¿¿¿¿¿¿¿¿
                        if (++ActFullRunTimes >= ActFullRunTimesSum) { // ¿¿¿¿¿¿¿¿¿¿¿
                            McuToPCSendData(CMD_FULL_ACTION_STOP, 0, 0);
                            fRobotRun = FALSE;
                            if (ActFullNum == 100) {
                                FullActRun(101, 1);
                            }
                        }
                    }
                }
            }
        }
    } else {
        FrameIndex = 0;

        ActFullRunTimes = 0;

        fFrameRunFinish = TRUE;

        TimeActionRunTotal = gSystemTickCount;
        // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
    }
}
