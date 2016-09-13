/*! @file IROBOT.h
 *
 *  @brief Module to control the iRobot.
 *
 *  This contains the functions for communicating with the iRobot through the
 *  create open interface.
 *
 *  @author A.Pope, K.Leone, C.Stewart, J.Lynch
 *  @date 02-09-2016
 */
#ifndef IROBOT_H
#define	IROBOT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "types.h"

/*! @brief Sets up the iRobot before first use.
 *
 *  @return BOOL - true if the iRobot was successfully initialized.
 */
bool IROBOT_Init(void);

/*! @brief Puts the IROBOT into Full control mode.
 *  
 *  @return void
 *  @note: must be called after the PIC has finished init all modules.
 */
void IROBOT_Start(void);

/*! @brief Performs a 360 scan of the environment using the IR sensor. At the
 *  completion of the scan, the sensor will pointed towards the closest object
 *
 *  @return void
 */
void IROBOT_Scan360(void);

/*! @brief Drive the robot in a straight line 4m.
 *  
 *  @return bool - True if the rotate was interrupted due to sensor triggering
 */
bool IROBOT_DriveStraight(int16_t dist);

/*! @brief Drive the robot in a 1m square.
 *  
 *  @return void
 */
void IROBOT_DriveSquare(void);

void IROBOT_WallFollow(void);

#ifdef	__cplusplus
}
#endif

#endif	/* IROBOT_H */

