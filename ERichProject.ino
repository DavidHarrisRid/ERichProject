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
*
* 08.11.22  Team7 Bug fix
******************************************************************************/
// Code von Miso ab hier

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPi.h>
//Für Visual Studio benötigt
//#include <cstdint>
//#include "Makeblock.h"


/*****************************************************************************/
// Constants
/*****************************************************************************/

const int I_ZERO     = 0;
const int I_NEGATIVE = -1;

const float F_MULTIPLIER = 1.4f;

const float F_NULL   = 0.0f;
const float F_MILLIS = 1000.0f;

// Constants Moving Direction

const int I_FORWARD = 0;
const int I_STOP    = 3;
const int I_LEFT    = 1;
const int I_RIGHT   = 2;


/*****************************************************************************/
// Line Follower Port
/*****************************************************************************/

const uint8_t I_FOLLOWER_PORT = 8;


/*****************************************************************************/
// Distance Sensor Port
/*****************************************************************************/

const uint8_t I_DISTANCE_PORT = 7;


/*****************************************************************************/
// Encoder Board
/*****************************************************************************/

int I_PORT_1 = 1;
int I_PORT_2 = 2;
int I_PORT_3 = 3;
int I_PORT_4 = 4;

// Code von Miso bis hier


// Code von Anton ab hier
/*****************************************************************************/
// Moving Power
/*****************************************************************************/
//Constant is negative for ERich to drive forward

const int I_OUTPUT_PWR_RIGHT  = ( 35 / 100.0 * 255 );
const int I_OUTPUT_PWR_LEFT   = ( 35 / 100.0 * 255 ) * I_NEGATIVE;
const int I_OUTPUT_PWR_ZERO   = 0 / 100.0 * 255;


/*****************************************************************************/
// Sensor Input
/*****************************************************************************/

const uint8_t I_ZERO_INPUT    = 3;
const uint8_t I_LEFT_INPUT    = 1;     
const uint8_t I_RIGHT_INPUT   = 2;    
const uint8_t I_DOUBLE_INPUT  = 0;


/*****************************************************************************/
// Methods from included classes
/*****************************************************************************/

MeLineFollower lineFollower8( I_FOLLOWER_PORT ); 
MeUltrasonicSensor ultrasonic_7( I_DISTANCE_PORT );

MeEncoderOnBoard Encoder1( I_PORT_1 ); 
MeEncoderOnBoard Encoder2( I_PORT_2 );
MeEncoderOnBoard Encoder3( I_PORT_3 );
MeEncoderOnBoard Encoder4( I_PORT_4 ); 

// Code von Anton bis hier


// Code von Cem ab hier
/*****************************************************************************/
// Functions
/*****************************************************************************/
// Function for ERich movement in correlation to direction argument

void MoveRobot( int a_iDirection) 
{
  int leftSpeed   = I_OUTPUT_PWR_ZERO;
  int rightSpeed  = I_OUTPUT_PWR_ZERO;

  if( a_iDirection == I_RIGHT ) 
  {
    leftSpeed   = I_OUTPUT_PWR_LEFT * F_MULTIPLIER;
    rightSpeed  = I_OUTPUT_PWR_RIGHT*I_NEGATIVE * F_MULTIPLIER;
  }

  else if ( a_iDirection == I_LEFT ) 
  {
    leftSpeed   = I_OUTPUT_PWR_LEFT*I_NEGATIVE * F_MULTIPLIER;
    rightSpeed  = I_OUTPUT_PWR_RIGHT * F_MULTIPLIER;
  }

  else if ( a_iDirection == I_STOP ) 
  {
    leftSpeed   = I_OUTPUT_PWR_ZERO;
    rightSpeed  = I_OUTPUT_PWR_ZERO;
  }

  else if ( a_iDirection == I_FORWARD )
  {
    leftSpeed   = I_OUTPUT_PWR_LEFT;
    rightSpeed  = I_OUTPUT_PWR_RIGHT;
  }

  Encoder1.setTarPWM( rightSpeed );
  Encoder2.setTarPWM( leftSpeed );
}

// Code von Cem bis hier


// Code von Laszlo ab hier

// Function for giving feedback to the motors encoder

void _loop( void )
{
    Encoder1.loop();
    Encoder2.loop();
}


/*****************************************************************************/
// Callbacks:
/*****************************************************************************/
// Functions which give the correct current to the encoder motor

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

// Code von Laszlo bis hier


// Code von David ab hier
/*****************************************************************************/
// Setup:
/*****************************************************************************/
// Sets Parameters and Connections one time when ERich is startet or reset

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
//Repeats as long ERich is running
//Direction of MoveRobot function in correlation 
//to the Sensor Input is controlled

void loop ()
{
  if( ultrasonic_7.distanceCm() > 20 )
  {
    if( lineFollower8.readSensors() == I_ZERO_INPUT )
    {
      MoveRobot( I_STOP);
    }
    if ( lineFollower8.readSensors() == I_DOUBLE_INPUT )
    {
      MoveRobot( I_FORWARD);
    }
    if ( lineFollower8.readSensors() == I_LEFT_INPUT )
    {
      MoveRobot( I_LEFT);
    }
    if ( lineFollower8.readSensors() == I_RIGHT_INPUT )
    {
      MoveRobot ( I_RIGHT);
    }
  }
  else
  {
    MoveRobot( I_STOP);
  }
  _loop();
}

// Code von David bis hier
/*****************************************************************************/
/*****************************************************************************/