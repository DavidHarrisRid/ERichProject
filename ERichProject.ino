/*****************************************************************************
* Project: eRich
* File   : ERichProject.ino
* Date   : 05.09.2022
* Author : Marcus Schaal (MS)
*
* These coded instructions, statements, and computer programs contain
* proprietary information of the author and are protected by Federal
* copyright law. They may not be disclosed to third parties or copied
* or duplicated in any form, in whole or in part, without the prior
* written consent of the author.
*
* History:
*	05.09.22	MS	Created
******************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPi.h>
//Für Visual Studio benötigt
//#include <cstdint>
//#include "Makeblock.h"


/*****************************************************************************/
// Konstanten
/*****************************************************************************/

const int I_ZERO     = 0;
const int I_NEGATIVE = -1;

const float F_NULL   = 0.0f;
const float F_MILLIS = 1000.0f;

const int I_FORWARD = 0;
const int I_STOP    = 3;
const int I_LEFT    = 1;
const int I_RIGHT   = 2;


/*****************************************************************************/
// Maths
/*****************************************************************************/
const double D_PI        = 3.141592653589793;
const double D_ANGLE     = 180.0;
const double D_ANGLE_RAD = D_PI / D_ANGLE;
const double D_ANGLE_DEG = D_ANGLE / D_PI;


/*****************************************************************************/
// Line Follower
/*****************************************************************************/

const uint8_t I_FOLLOWER_PORT = 8;


/*****************************************************************************/
// Encoder Board
/*****************************************************************************/

int I_SLOT_1 = 1;
int I_SLOT_2 = 2;
int I_SLOT_3 = 3;
int I_SLOT_4 = 4;


/*****************************************************************************/
// Moving Power
/*****************************************************************************/

const int I_MOVING_POWER = 50 / 100.0 * 255;
const int I_ZERO_POWER = 0 / 100.0 * 255;


/*****************************************************************************/
// Delay
/*****************************************************************************/

const int I_ONE_SEC = 1000; 


/*****************************************************************************/
// Sensor Output
/*****************************************************************************/

const uint8_t I_ZERO_OUTPUT = 3;
const uint8_t I_LEFT_OUTPUT = 1;     
const uint8_t I_RIGHT_OUTPUT = 2;    
const uint8_t I_DOUBLE_OUTPUT = 0;


/*****************************************************************************/
// Methoden
/*****************************************************************************/
MeLineFollower lineFollower8( I_FOLLOWER_PORT ); 

MeEncoderOnBoard Encoder1(I_SLOT_1); 
MeEncoderOnBoard Encoder2(I_SLOT_2);
MeEncoderOnBoard Encoder3(I_SLOT_3);
MeEncoderOnBoard Encoder4(I_SLOT_4); 


/*****************************************************************************/
// Funktionen
/*****************************************************************************/
// Funktion die Bewegungsrichtung und Geschwindigkeit berechnet
// und dem Roboter �bergibt

void MoveRobot( int a_iDirection, int a_iSpeed ) 
{
  int leftSpeed   = I_ZERO;
  int rightSpeed  = I_ZERO;

  if( a_iDirection == I_RIGHT ) 
  {
    leftSpeed   = a_iSpeed * I_NEGATIVE;
    rightSpeed  = a_iSpeed;
  }

  else if ( a_iDirection == I_LEFT ) 
  {
    leftSpeed   = a_iSpeed ;
    rightSpeed  = a_iSpeed * I_NEGATIVE;
  }

  else if ( a_iDirection == I_STOP ) 
  {
    leftSpeed   = a_iSpeed * I_ZERO;
    rightSpeed  = a_iSpeed * I_ZERO;
  }

  else if ( a_iDirection == I_FORWARD )
  {
    leftSpeed   = a_iSpeed * I_NEGATIVE;
    rightSpeed  = a_iSpeed * I_NEGATIVE;
  }

  Encoder1.setTarPWM( rightSpeed );
  Encoder2.setTarPWM( leftSpeed );
}

void DelayLoop( float a_fSeconds )
{
    //if-Schleife
    if (a_fSeconds < F_NULL)
    {
      a_fSeconds = F_NULL;
    }
    long endTime = millis() + a_fSeconds * F_MILLIS;
    while (millis() == endTime) loop();

}
void _loop( void )
{
    Encoder1.loop();
    Encoder2.loop();
}


/*****************************************************************************/
// Callbacks
/*****************************************************************************/

void OnEncoder1ReadProc( void )
{
    if( digitalRead( Encoder1.getPortB() ) == I_ZERO )
    {
        Encoder1.pulsePosMinus();
    }
    else
    {
        Encoder1.pulsePosPlus();
    }
}

void OnEncoder2ReadProc( void )
{
    if ( digitalRead( Encoder2.getPortB() ) == I_ZERO )
    {
        Encoder2.pulsePosMinus();
    }
    else
    {
        Encoder2.pulsePosPlus();
    }
}


/*****************************************************************************/
// Setup:
/*****************************************************************************/
// Sets Parameters and Connections one time when Arduino is startet or reset

void setup ()
{
  attachInterrupt( Encoder1.getIntNum(), OnEncoder1ReadProc, RISING );
  attachInterrupt( Encoder2.getIntNum(), OnEncoder2ReadProc, RISING );

  TCCR1A = _BV( WGM10 );
  TCCR1B = _BV( CS11 )  | _BV( WGM12 );
  TCCR2A = _BV( WGM21 ) | _BV( WGM20 );
  TCCR2B = _BV( CS21 );
}


/*****************************************************************************/
//Loop:
/*****************************************************************************/
//Repeats as long the Arduino is running
//Output behaviour to Motors in correlation to the Sensor Input ist controlled

void loop ()
{
  if( lineFollower8.readSensors() == I_ZERO_OUTPUT )
  {
    MoveRobot( I_STOP, I_ZERO_POWER );
  }
  if ( lineFollower8.readSensors() == I_DOUBLE_OUTPUT )
  {
    MoveRobot( I_FORWARD, I_MOVING_POWER );
  }
  if ( lineFollower8.readSensors() == I_LEFT_OUTPUT )
  {
    MoveRobot( I_LEFT, I_MOVING_POWER );
  }
  if ( lineFollower8.readSensors() == I_RIGHT_OUTPUT )
  {
    MoveRobot ( I_RIGHT, I_MOVING_POWER );
  }
  _loop();
}
/*****************************************************************************/
/*****************************************************************************/