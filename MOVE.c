/*! @file IROBOT.c
 *
 *  @brief Module to move the iRobot.
 *
 *  This contains the functions for moving the iRobot through the create open
 *  interface.
 *
 *  @author A.Pope, K.Leone, C.Stewart, J.Lynch, A.Truong
 *  @date 02-09-2016
 */
#include "USART.h"
#include "OPCODES.h"
#include "MOVE.h"

bool MOVE_Init(void){
  return true; //No initialisation needed for the move module
}

bool MOVE_Straight(int16_t velocity, uint16_t distance){
  uint16union_t rxdata;
  SensorsStatus_t sensStatus;
  int16_t distanceTravelled = 0;
  bool sensorTrig = false;

  //Get data from the Irobot regarding distance to reset the distance travelled
  USART_OutChar(OP_SENSORS); USART_OutChar(OP_SENS_DIST);
  USART_InChar(); USART_InChar(); //Dummy read of sensor values

  MOVE_DirectDrive(velocity, velocity); //Tell the IROBOT to drive straight at speed

  //Let the robot drive until it reaches the desired distance
  while((distanceTravelled < distance) && !sensorTrig){
    //Get distance traveled since last call
    USART_OutChar(OP_SENSORS);
    USART_OutChar(OP_SENS_DIST);
    rxdata.s.Hi = USART_InChar();
    rxdata.s.Lo = USART_InChar();

    if(velocity >= 0){
      distanceTravelled += (int16_t) rxdata.l; //Positive velocity returns positive distance
    } else {
      distanceTravelled += ((int16_t)rxdata.l * -1); //Negative vel returns neg dist (must normalise)
    }

    sensorTrig = MOVE_CheckSensor(&sensStatus);
  }

  MOVE_DirectDrive(0, 0); //Tell the IROBOT to stop moving

  return sensorTrig;
}

bool MOVE_Rotate(uint16_t velocity, uint16_t angle, TDIRECTION dir){
  uint16union_t rxdata;
  SensorsStatus_t sensStatus;
  int16_t angleMoved = 0;
  bool sensorTrig = false;

  //Get current angle moved to reset the angle moved count
  USART_OutChar(OP_SENSORS); USART_OutChar(OP_SENS_ANGLE);
  USART_InChar(); USART_InChar();  //Dummy read to clear recieve buffer

  if (dir == DIR_CCW){
    MOVE_DirectDrive((velocity * -1), velocity); //Make the robot turn CCW @ 210mm/s
  } else {
    MOVE_DirectDrive(velocity, (velocity * -1)); //Make the robot turn CW @ 210mm/s
  }

  while ((angleMoved < angle) && !sensorTrig)
  {
    //Get Angle since last movement
    USART_OutChar(OP_SENSORS); USART_OutChar(OP_SENS_ANGLE);
    rxdata.s.Hi = USART_InChar();
    rxdata.s.Lo = USART_InChar();

    if(dir == DIR_CCW){
      angleMoved += (int16_t) rxdata.l; //CCW direction returns positive angles
    }else{
      angleMoved += ((int16_t) rxdata.l * -1); //CW direction returns negative angles
    }

    sensorTrig = MOVE_CheckSensor(&sensStatus);
  }

  MOVE_DirectDrive(0, 0); //Tell the IROBOT to stop rotating
  return sensorTrig;
}

void MOVE_DirectDrive(int16_t leftWheelVel, int16_t rightWheelVel){
  int16union_t rightBytes, leftBytes;

  rightBytes.l = rightWheelVel;
  leftBytes.l = leftWheelVel;

  USART_OutChar(OP_DRIVE_DIRECT);
  USART_OutChar(rightBytes.s.Hi); //Send the velocity for the right wheel
  USART_OutChar(rightBytes.s.Lo);
  USART_OutChar(leftBytes.s.Hi); //Send the velocity for the left wheel
  USART_OutChar(leftBytes.s.Lo);
}

bool MOVE_CheckSensor(SensorsStatus_t * sensStatus){
  uint8_t data;

  //Tell the Robot to send back information regarding a group of sensors
  USART_OutChar(OP_QUERY);
  USART_OutChar(2);         //Get information about 3 sensors TODO: Implement victim
  USART_OutChar(OP_SENS_BUMP);
  USART_OutChar(OP_SENS_VWALL);
  
  //1. Packet ID: 7 (Bump and Wheel drop)
  data = USART_InChar();
  sensStatus->sensBits.bump = (data & 0b00000011);   //We only care about the bump data so AND with mask

  //2. Packet ID: 13 (Virtual Wall)
  sensStatus->sensBits.virtWall = USART_InChar();

  //3. TODO: Implement victim
  sensStatus->sensBits.victim = 0;

  return (sensStatus->sensors > 0); //A value greater than 0 indicates one of the sensors have been tripped
}