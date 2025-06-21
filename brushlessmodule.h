#ifndef BRUSHLESSMODULE_H
#define BRUSHLESSMODULE_H
//---------------------------------------------------------------------------
//
// File:     BrushlessModule.h
// Info:     Definitions for the Brushless control module BrushlessModule.cpp
// Created:  Daniele Belloni
//
//---------------------------------------------------------------------------


#include "comminterface.h"
#include "BrushlessModuleDefs.h"
#include "myserialport.h"
#include "serialcom.h"

#define MOTOR_DATA_LEN		512

#define MOTOR_ERROR			-1
#define MOTOR_OK			0

#define FAST_DECAY			0
#define SLOW_DECAY			1

#define CW					0x01
#define CCW					0x00

#define IN1					0x01
#define IN2					0x02
#define IN3					0x04

#define IOIN				0
#define IOOUT				1

#define LOW					0
#define HIGH				1

#define NORAMP				0
#define RAMP				1

#define OUT0				0
#define OUT1				1
#define OUT2				2

// Maschere per la funzione MotorStatus
#define MOTOR_RUNNING		0x0001		// Il motore si sta muovendo
#define MOTOR_OVERCURRENT	0x0002		// Overcurrent
#define MOTOR_PROCERROR		0x0004		// Errore nella procedura (usato in ricerca zero)
#define MOTOR_TIMEOUT		0x0008		// Timeout nel movimento
#define MOTOR_ZERO			0x0010		// Ricerca origini in corso
#define MOTOR_OVERRUN		0x0020		// Raggiunto un finecorsa
#define MOTOR_DANGER		0x0040		// Raggiunto il limite software di movimento
#define MOTOR_OVERSPEED		0x0080		// Overspeed
#define MOTOR_NOENC			0x0100		// Nessuna variazione encoder rilevata
#define MOTOR_PIDSPDON		0x0200		// PID di velocita' attivo
#define MOTOR_PIDPOSON		0x0400		// PID di posizione attivo
#define MOTOR_NEARTARGET	0x0800		// Il motore e' in prossimita della posizione obiettivo
#define MOTOR_MULTIMOVE		0x1000		// Il motore ha eseguito un movimento "multi-motore"
#define MOTOR_NO_FOLLOW		0x2000		// Errore inseguimento traiettoria "eccessivo"
#define MOTOR_SECURITY		0x4000		// Sensore di sicurezza attivo
#define MOTOR_STEADYPOS		0x8000		// Errore mantenimento posizione di regime


// Maschere per la funzione PIDStatus
#define PID_SPDDEF			0x0001		// Parametri di default PID di velocita' attivi
#define PID_SPD0			0x0002		// Parametri di regime PID di velocita' attivi
#define PID_SPDSPD			0x0004		// Parametri di loop di velocita' PID di velocita' attivi
#define PID_POSDEF			0x0008		// Parametri di default PID di posizione attivi
#define PID_POS0			0x0010		// Parametri di regime PID di posizione attivi

// Maschere per la funzione EncoderStatus
#define ENC_MF_ERROR		0x0001		// Magnetic field error
#define ENC_OVERRUN_ERROR	0x0002		// Overrun error
#define ENC_FRAME_ERROR		0x0004		// Frame error

// Attivazione o disattivazione check dei limiti dei movimenti
#define LIMITCHECK_OFF		0
#define LIMITCHECK_ON		1
// Livello check limiti (attivi alti o bassi)
#define LIMITLEVEL_LOW		0
#define LIMITLEVEL_HIGH		1

// Livello check sensore di zero (attivo alto o basso)
#define ZEROLEVEL_HIGH					0
#define ZEROLEVEL_LOW					1

// Attivazione o disattivazione check del sensore di sicurezza
#define SECURITYCHECK_OFF   0
#define SECURITYCHECK_ON    1
// Livello check sensore sicurezza (attivo alto o basso)
#define SECURITYLEVEL_LOW   0
#define SECURITYLEVEL_HIGH  1

// Attivazione, disattivazione o stato dei movimenti del motore
#define MOTOR_OFF			0
#define MOTOR_ON			1
#define MOTOR_SPD_ON		2

// Modalita' di funzionamento dei moduli (con o senza encoder) //$$$ENC
#define NORMAL_MODE			0
#define ENCODER_MODE		1

// Modalita' di rotazione senza encoder
#define PHASEROT_DISABLE    0
#define PHASEROT_POS        1
#define PHASEROT_NEG        2

// Tipo encoder (quadratura o seriale)
#define ENCODER_QEP			0
#define ENCODER_SER			1

// Modalita' di funzionamento dell'encoder (normale o invertito)
#define ENCODER_NORMAL		0
#define ENCODER_INVERTED	1

// Modalita' di generazione angolo elettrico con encoder
#define ELECTTHETA_ON		0
#define ELECTTHETA_OFF		1

// Logger channel IDs
enum eLogChannels
{
    LOG_CH1 = 0,
    LOG_CH2,
    LOG_CH3,
    LOG_CH4,
    LOG_CH_NUM
};

#define LOG_BUF_SIZE		100

// Livello del trigger del logger
#define LOWTOHIGH_TRIG 		0
#define HIGHTOLOW_TRIG 		1

// Logger channel IDs
#define LOGGER_CH1			0
#define LOGGER_CH2			1
#define LOGGER_CH3			2
#define LOGGER_CH4			3

// Ingressi canali logger
#define LOGGER_MECHTHETA		0
#define LOGGER_SPEED			1
#define LOGGER_CLARKA			2
#define LOGGER_CLARKB			3
#define LOGGER_PIDIDOUT			4
#define LOGGER_PIDIQOUT			5
#define LOGGER_PIDIDFDB			6
#define LOGGER_PIDIQFDB			7
#define LOGGER_PIDIDREF			8
#define LOGGER_PIDIQREF			9
#define LOGGER_SPEEDREF			10
#define LOGGER_POSREF			11
#define LOGGER_POSERR			12
#define LOGGER_PIDIDERR			13
#define LOGGER_PIDIQERR			14
#define LOGGER_SPEEDERR			15
#define LOGGER_SPEEDREFNOPOS	16
#define LOGGER_ELECERROR		17
#define LOGGER_ELECRAMP			18
#define LOGGER_VPWR				19

// Indici fasi motore
#define PHASE1_INDEX		0
#define PHASE2_INDEX		1
#define PHASE3_INDEX		2

// Attivazione e disattivazione dei PWM delle fasi del motore
#define PWM_OFF				0
#define PWM_ON				1

// Attivazione e disattivazione della modalita' calibrazione ADC
#define ADCCAL_OFF			0
#define ADCCAL_ON			1

// Parametri usati durante la calibrazione dello zero elettrico
#define CAL_ANGLE_CURRENT	0.3
#define CAL_PHASE_ROT_DELAY	2000
#define CAL_ANGLE_DELAY		2000
#define CAL_PARKANGLE		0.1
#define CAL_ERROR			40

// Canali ADC
#define ADC_CHANNEL0		0
#define ADC_CHANNEL1		1
#define ADC_CHANNEL2		2

// Tipo encoder
#define ROTATIVE_ENC		0
#define LINEAR_ENC			1

// Bits di modalita' del modulo (relativi alla variabile work_vars dedicata...)
#define MODE_LIMITS         0
#define MODE_LIMITSLEVEL    1
#define MODE_RAMP           2 //n.u.
#define MODE_SECURITY       3
#define MODE_SECURITYLEVEL  4

// Attivazione/disattivazione rampe accelerazione/decelerazione
#define RAMP_ON  			0
#define RAMP_OFF  			1

// Lettura velocita'
#define SPEED_PU			0
#define SPEED_RPM			1
#define SPEED_MS			2

// Set di parametri del PID di velocita'
#define PIDSET_DEF			0
#define PIDSET_0			1
#define PIDSET_SPD			2

// Attivazione/disattivazione compensazione decelerazione
#define DECCOMP_ON  		0
#define DECCOMP_OFF  		1

// Attivazione/disattivazione limiti di pericolosita'
#define DANGERLIMIT_ON  	0
#define DANGERLIMIT_OFF  	1

// Direzione ricerca zero
#define ZEROSEARCH_POS  	0
#define ZEROSEARCH_NEG  	1

// Velocita' di homing
#define HOME_SLOW  			0
#define HOME_FAST  			1

// Tipo di profilo
#define FIFTHDEGREE_PROFILE				0
#define TRAPEZOIDAL_PROFILE				1

class BrushlessModule
{
public:
    short bufDataLog[LOG_CH_NUM][LOG_BUF_SIZE];

    BrushlessModule( CommInterface* ComInt, int Address );
    ~BrushlessModule();

    void ChangeAddress( int Address ) { m_Address = Address; }
    int GetAddress() { return m_Address; }
    int GetErrorCode() { return errorCode; }

    void SerialComActivation( bool status ) { serialComStatus = status; }

    unsigned short GetVersion();
    int  SetNominalCurrent( int val );
    int  GetNominalCurrent();
    int  StopRotation( int ramp );
    long ActualPosition();
    int  Home();
    int  GotoPos0( long pos );
    int  MinimumFreq( int mFreq );
    int  MaximumFreq( long MFreq );
    int  Aceleration( long acc );
    int  Deceleration( long dec );
    long ActualVelocity();
    int  MotorStatus();
    int  ActualInputs();
    int  SetLimitsCheck( int limit, int limitLevel = LIMITLEVEL_LOW );
    int  ResetAlarms();
    int  MotorEnable( int command );
    int  ResetDrive();
    long GetEncoderActualPosition();
    int  EncoderMode( int type, int mode );
    int  GetEncoderMode( int type );
    int  InputsSetting( long val );
    int  SearchPos0( int dir );
    int  SuspendDrive();
    int  SetPIDIqParams( long KP, long KI, long KD, long KC, long OMax, long OMin );
    int  GetPIDIqParams( long *KP, long *KI, long *KD, long *KC, long *OMax, long *OMin );
    int  SetPIDIdParams( long KP, long KI, long KD, long KC, long OMax, long OMin );
    int  GetPIDIdParams( long *KP, long *KI, long *KD, long *KC, long *OMax, long *OMin );
    int  SetPIDSpdParams( unsigned short set, long KP, long KI, long KD, long KC, long OMax, long OMin );
    int  GetPIDSpdParams( unsigned short set, long *KP, long *KI, long *KD, long *KC, long *OMax, long *OMin );
    int  SetPIDPosParams( unsigned short set, long KP, long KI, long KD, long KC, long OMax, long OMin );
    int  GetPIDPosParams( unsigned short set, long *KP, long *KI, long *KD, long *KC, long *OMax, long *OMin );
    long GetLineEncoder();
    int  SetLineEncoder( long lines );
    long GetPulsesPerRev();
    int  SetPulsesPerRev( long ppr );
    int  GetPoles();
    int  SetPoles( int poles );
    int  GetCalibratedAngle();
    int  SetCalibratedAngle( int angle );
    int  GetLoggerTrigger();
    int  SetLoggerTrigger( int trig );
    int  GetLoggerPrescaler();
    int  SetLoggerPrescaler( int prescal );
    int  GetLoggerChannelData( int channel );
    int  SetLoggerChannelInput( int channel, int input );
    long GetEncoderActualSpeed( int mode );
    int  PWMEnable( int command );
    int  PhaseEnable( int phase, int command );
    int  ADCCalEnable( int command );
    long GetClarkeAs();
    long GetClarkeBs();
    long GetADCOffset( int channel );
    int  SetADCOffset( int channel, long val );
    long GetPIDIqOut();
    long GetPIDIdOut();
    long GetPIDSpdOut();
    long GetPIDPosOut();
    long GetPIDIqFdb();
    long GetPIDIdFdb();
    long GetPIDSpdFdb();
    long GetPIDPosFdb();
    long GetIqRef();
    int  SetIqRef( long val );
    long GetIdRef();
    int  SetIdRef( long val );
    int  GetEncoderType();
    int  SetEncoderType( int type );
    int  ElecThetaGeneration( int mode );
    int  GetMaxSpeed();
    int  SetMaxSpeed( int spd );
    int  GetRamps();
    int  SetRamps( int mode );
    long ActualStartStopVelocity();
    long ActualAceleration();
    long ActualDeceleration();
    int  SetActualPIDSpd( unsigned short set );
    int  SetActualPIDPos( unsigned short set );
    int  LoggerTriggerMode( int mode );
    long GetEncoderActualPulses();
    int  SetPhaseCurrent( long val );
    long GetPhaseCurrent();
    int  GetDecelerationAdvance();
    int  SetDecelerationAdvance( int val );
    int  GetPulsesToMm();
    int  SetPulsesToMm( int ptm );
    int  DecCompensationMode( int mode );
    int  SetProximityGap( long val );
    long GetProximityGap();
    int  SetPIDPosActivationGap( long val );
    long GetPIDPosActivationGap();
    int  SetIParkAngle( long val );
    long GetIParkAngle();
    long GetDangerLimit( int limit );
    int  SetDangerLimit( int limit, long val );
    int  DangerLimitMode( int mode );
    int  ResetEncoder();
    int  HomeSensorInput( int val );
    long GetHomingSpeed( int speed );
    int  SetHomingSpeed( int speed, long val );
    int  GetOverSpeed();
    int  SetOverSpeed( int val );
    int  SetReferenceActualPos();
    int  ResetPIDSpd();
    int  ResetPIDPos();
    int  PIDStatus();
    int  GetMaxReadCurrent();
    int  SetMaxReadCurrent( int val );
    long GetPhaseC();
    long GetEncoder360ActualPosition();
    long GetElecTheta();

    // get/set the acceleration feed-forward
    long GetAccFFWD();
    int  SetAccFFWD( long val );

    // get/set the speed feed-forward
    long GetSpdFFWD();
    int  SetSpdFFWD( long val );

    // get/set the maximum jerk
    int  GetMaxJerk();
    int  SetMaxJerk( int val );

    long GetCFriction();
    int  SetCFriction( long val );

    long GetEndMovementSpeed();
    int  SetEndMovementSpeed( long val );
    long GetEndMovementDelta();
    int  SetEndMovementDelta( long val );

    // move multiple motors (absolute motion)
    int  GotoPos0_Multi( unsigned char addX, long posX, unsigned char addY, long posY, long maxFreq, long acc, long proximityDeltaX, long proximityDeltaY );
    int  ResetFlag_Multi();

    long GetEncoder2ActualPosition();
    long GetEncoder2ActualPulses();
    long GetLineEncoder2();
    int  SetLineEncoder2( long lines );
    long GetEncoderInterpActualPosition();
    int  GetStatus_ResetFlagMulti();
    int  SetFollowingError( long val );
    long GetFollowingError();
    int  EncoderStatus();
    int SetStartTicks( int ticks );
    int SetPhaseRotation( int value );
    int SetPhaseRotationNumber( int value );
    int SetSpeedFilter( unsigned short window );
    int SetSecurityCheck( int limit, int limitLevel );
    int SetSecurityInput( int val );

    // from 4.10
    long GetVPower();

    // from 4.11
    int  GetTemperature();
    int  SetSteadyPosFollowError( long val );
    long GetSteadyPosFollowError();
    int  SetEncoderSpykeDelta( int delta );
    int  GetEncoderSpykeDelta();

    // from 4.12
    int  SetSpeedProfile( int mode );

    // from 4.15
    int UpdateOverSpeed();
    int OverSpeedCheck( int enable );

    // from 4.18
    int SetHomeSensorLevel( int level );

    // from 5.00
    int SetOutputs( int outId, int outVal );

    // from 5.1
    int ResetSpeed();
    BrushlessModule();

private:
    CommInterface* m_comInt;

protected:
   // wxCommInterface* m_comInt;
    int m_Address;
    unsigned char m_Data[MOTOR_DATA_LEN];
    int errorCode;

    int Read_Reg_Micro( unsigned char RegAdd, unsigned short* Value );
    int Write_Reg_Micro( unsigned char RegAdd, unsigned short Value );

    unsigned char ver;
    unsigned char rev;

    bool serialComStatus;
};

#endif // BRUSHLESSMODULE_H
