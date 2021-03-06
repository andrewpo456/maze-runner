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

/*! @brief Initiates the IROBOT maze-run.
 *  
 *  @note Assumes the robot is placed at position (1,3)
 */
void IROBOT_MazeRun(void);
#ifdef	__cplusplus
}
#endif

#endif	/* IROBOT_H */

