#include "brushlessmodule.h"



BrushlessModule::BrushlessModule(CommInterface* ComInt, int address):
    m_comInt(ComInt), m_Address(address), serialComStatus(true), errorCode(0), ver(0), rev(0)
{

}
BrushlessModule::~BrushlessModule()
{
}
int BrushlessModule::Read_Reg_Micro(unsigned char regAdd, unsigned short* value)
{
    unsigned char rx_buffer[2];
    unsigned short rx_len = 0;

    if (!serialComStatus)
    {
        *value = 0;
        return MOTOR_OK;
    }

    errorCode = TWSBus1_Send(m_comInt, m_Address, READREGISTER_CMD, &regAdd, 1, rx_buffer, &rx_len);

    if (errorCode == 1)
        return 1;

    *value = (rx_buffer[0] << 8) + rx_buffer[1];
    return MOTOR_OK;
}
int BrushlessModule::Write_Reg_Micro(unsigned char RegAdd, unsigned short Value)
{
    unsigned char tx_buffer[3];
    tx_buffer[0] = RegAdd;
    tx_buffer[1] = Value >> 8;      // High byte
    tx_buffer[2] = Value & 0xFF;    // Low byte

    unsigned short rx_len = 0;
    unsigned char rx_buffer[8];     // Adjust size as needed

    errorCode = TWSBus1_Send(
        m_comInt,
        m_Address,
        WRITEREGISTER_CMD,
        tx_buffer,
        3,
        rx_buffer,
        &rx_len
        );

    if (errorCode == 1)
        return 1;

    return MOTOR_OK;
}
unsigned short BrushlessModule::GetVersion()
{
    unsigned short val = 0;

    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, GETVERSION_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }
    val = (m_Data[0]<<8) + m_Data[1];

    ver = m_Data[0];
    rev = m_Data[1];

    return val;
}
int BrushlessModule::SetNominalCurrent( int val )
{
    if( Write_Reg_Micro( 135, val) )
    {
        return MOTOR_ERROR;
    }

    if( errorCode == TWSBus1_Send_Command( m_comInt, m_Address, UPDATECURRENT_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetNominalCurrent()
{
    unsigned short value = 0;
    int result = Read_Reg_Micro(0x12, &value); // Use the same hardcoded address as in your original code

    if (result != MOTOR_OK)
        return -1;

    return static_cast<int>(value);
}
int BrushlessModule::StopRotation( int ramp )
{
    if( errorCode == TWSBus1_Send_Command( m_comInt, m_Address, IMMEDIATESTOP_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::ActualPosition()
{
    unsigned short valH = 0, valL = 0;

    // Read high word (assuming register 5)
    if (Read_Reg_Micro(5, &valH))
        return -1; // or another error code

    // Read low word (assuming register 6)
    if (Read_Reg_Micro(6, &valL))
        return -1; // or another error code

    return (static_cast<long>(valH) << 16) + valL;
}
int BrushlessModule::Home()
{
    if( errorCode == TWSBus1_Send_Command( m_comInt, m_Address, HOME_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GotoPos0( long pos )
{
    // Setta il numero di passi (32 bit)
    if( Write_Reg_Micro( 5, pos >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 6, pos & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, POSUPDATE_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::MinimumFreq( int mFreq )
{
    // Setta la velocita' di start/stop (32 bit)
    if( Write_Reg_Micro( 79, mFreq >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 80, mFreq & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::MaximumFreq( long MFreq )
{
    // Setta la velocita' (32 bit)
    if( Write_Reg_Micro( 3, MFreq >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 4, MFreq & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::Aceleration( long acc )
{
    if( Write_Reg_Micro( 81, acc >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 82, acc & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::Deceleration( long dec )
{
    if( Write_Reg_Micro( 103, dec >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 104, dec & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::ActualVelocity()
{
    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;
    if (Read_Reg_Micro( 3, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 4, &valL))
    {
        return MOTOR_ERROR;
    }

    return (valH << 16) + valL;
}
int BrushlessModule::MotorStatus()
{
    unsigned short val = 0;

    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    if( Read_Reg_Micro( 74, &val) )
    {
        return MOTOR_ERROR;
    }

    return val;
}

int BrushlessModule::ActualInputs()
{
    unsigned short val = 0;

    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    if( Read_Reg_Micro( 1, &val) )
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetLimitsCheck(int limit, int limitLevel)
{
    unsigned short val = 0;

    // Read mode register
    if (Read_Reg_Micro(77, &val))
    {
        return MOTOR_ERROR;
    }

    if (limit == LIMITCHECK_ON)
    {
        // Set the limits check flag to true
        val |= (1 << MODE_LIMITS);
    }
    else
    {
        // Set the limits check flag to false
        val &= ~(1 << MODE_LIMITS);
    }

    if (limitLevel == LIMITLEVEL_HIGH)
    {
        // Set the limits check level to high
        val |= (1 << MODE_LIMITSLEVEL);
    }
    else
    {
        // Set the limits check level to low
        val &= ~(1 << MODE_LIMITSLEVEL);
    }

    // Write mode register
    if (Write_Reg_Micro(77, val))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::ResetAlarms()
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, ALARMRESET_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::MotorEnable(int command)
{
    if (command == MOTOR_OFF)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, LOOPDISABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
            return MOTOR_ERROR;
    }
    else if (command == MOTOR_ON)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, LOOPENABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
            return MOTOR_ERROR;
    }
    else if (command == MOTOR_SPD_ON)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, LOOPSPDENABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
            return MOTOR_ERROR;
    }
    else
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::ResetDrive()
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, MICRORESET_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetEncoderActualPosition()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETENCODER_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) +
          (static_cast<long>(m_Data[1]) << 16) +
          (static_cast<long>(m_Data[2]) << 8) +
          static_cast<long>(m_Data[3]);

    return val;
}
int BrushlessModule::EncoderMode(int type, int mode)
{
    if (type == ENCODER_QEP)
    {
        if (mode == ENCODER_NORMAL)
        {
            errorCode = TWSBus1_Send_Command(m_comInt, m_Address, SETENCODERQEPNORM_CMD, m_Data, serialComStatus);
            if (errorCode != 0)
                return MOTOR_ERROR;
        }
        else
        {
            errorCode = TWSBus1_Send_Command(m_comInt, m_Address, SETENCODERQEPINV_CMD, m_Data, serialComStatus);
            if (errorCode != 0)
                return MOTOR_ERROR;
        }
    }
    else // ENCODER_SER
    {
        if (mode == ENCODER_NORMAL)
        {
            errorCode = TWSBus1_Send_Command(m_comInt, m_Address, SETENCODERSERNORM_CMD, m_Data, serialComStatus);
            if (errorCode != 0)
                return MOTOR_ERROR;
        }
        else
        {
            errorCode = TWSBus1_Send_Command(m_comInt, m_Address, SETENCODERSERINV_CMD, m_Data, serialComStatus);
            if (errorCode != 0)
                return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
int BrushlessModule::GetEncoderMode(int type)
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    if (type == ENCODER_QEP)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETENCODERQEPMODE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    else
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETENCODERSERMODE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }

    return static_cast<int>(m_Data[0]);
}
int BrushlessModule::InputsSetting(long val)
{
    if (Write_Reg_Micro(75, (val & 0xFFFF0000) >> 16))
    {
        return MOTOR_ERROR;
    }

    if (Write_Reg_Micro(76, (val & 0x0000FFFF)))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SearchPos0( int dir )
{
    if( (errorCode = TWSBus1_Send_Command( m_comInt, m_Address, HOMESEARCH_CMD, dir, m_Data, serialComStatus )) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SuspendDrive()
{
    if( (errorCode = TWSBus1_Send_Command( m_comInt, m_Address, SUSPENDSERIAL_CMD, m_Data, serialComStatus )) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SetPIDIqParams( long KP, long KI, long KD, long KC, long OMax, long OMin )
{
    // Setta il KP
    if( Write_Reg_Micro( 43, KP >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 44, KP & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta il KI
    if( Write_Reg_Micro( 45, KI >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 46, KI & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta il KD
    if( Write_Reg_Micro( 47, KD >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 48, KD & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta il KC
    if( Write_Reg_Micro( 49, KC >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 50, KC & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta OutMax
    if( Write_Reg_Micro( 51, OMax >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 52, OMax & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta OutMin
    if( Write_Reg_Micro( 53, OMin >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 54, OMin & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Aggiorna i valori del PID
    if( (errorCode = TWSBus1_Send_Command( m_comInt, m_Address, PIDIQUPDATE_CMD, m_Data, serialComStatus )) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetPIDIqParams( long *KP, long *KI, long *KD, long *KC, long *OMax, long *OMin )
{
    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    // KP
    if (Read_Reg_Micro( 43, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 44, &valL))
    {
        return MOTOR_ERROR;
    }
    *KP = (valH << 16) + valL;

    // KI
    if (Read_Reg_Micro( 45, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 46, &valL))
    {
        return MOTOR_ERROR;
    }
    *KI = (valH << 16) + valL;

    // KD
    if (Read_Reg_Micro( 47, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 48, &valL))
    {
        return MOTOR_ERROR;
    }
    *KD = (valH << 16) + valL;

    // KC
    if (Read_Reg_Micro( 49, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 50, &valL))
    {
        return MOTOR_ERROR;
    }
    *KC = (valH << 16) + valL;

    // OutMax
    if (Read_Reg_Micro( 51, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 52, &valL))
    {
        return MOTOR_ERROR;
    }
    *OMax = (valH << 16) + valL;

    // OutMin
    if (Read_Reg_Micro( 53, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 54, &valL))
    {
        return MOTOR_ERROR;
    }
    *OMin = (valH << 16) + valL;

    return MOTOR_OK;
}
int BrushlessModule::SetPIDIdParams( long KP, long KI, long KD, long KC, long OMax, long OMin )
{
    // Setta il KP
    if( Write_Reg_Micro( 31, KP >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 32, KP & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta il KI
    if( Write_Reg_Micro( 33, KI >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 34, KI & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta il KD
    if( Write_Reg_Micro( 35, KD >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 36, KD & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta il KC
    if( Write_Reg_Micro( 37, KC >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 38, KC & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta OutMax
    if( Write_Reg_Micro( 39, OMax >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 40, OMax & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta OutMin
    if( Write_Reg_Micro( 41, OMin >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 42, OMin & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Aggiorna i valori del PID
    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, PIDIDUPDATE_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetPIDIdParams( long *KP, long *KI, long *KD, long *KC, long *OMax, long *OMin )
{
    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    // KP
    if (Read_Reg_Micro( 31, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 32, &valL))
    {
        return MOTOR_ERROR;
    }
    *KP = (valH << 16) + valL;

    // KI
    if (Read_Reg_Micro( 33, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 34, &valL))
    {
        return MOTOR_ERROR;
    }
    *KI = (valH << 16) + valL;

    // KD
    if (Read_Reg_Micro( 35, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 36, &valL))
    {
        return MOTOR_ERROR;
    }
    *KD = (valH << 16) + valL;

    // KC
    if (Read_Reg_Micro( 37, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 38, &valL))
    {
        return MOTOR_ERROR;
    }
    *KC = (valH << 16) + valL;

    // OutMax
    if (Read_Reg_Micro( 39, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 40, &valL))
    {
        return MOTOR_ERROR;
    }
    *OMax = (valH << 16) + valL;

    // OutMin
    if (Read_Reg_Micro( 41, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 42, &valL))
    {
        return MOTOR_ERROR;
    }
    *OMin = (valH << 16) + valL;

    return MOTOR_OK;
}
int BrushlessModule::GetPIDSpdParams( unsigned short set, long *KP, long *KI, long *KD, long *KC, long *OMax, long *OMin )
{
    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;
    unsigned char KP_add, KI_add, KD_add, KC_add;

    switch( set )
    {
    case PIDSET_DEF:
        KP_add = 7;
        KI_add = 9;
        KD_add = 11;
        KC_add = 13;
        break;

    case PIDSET_0:
        KP_add = 83;
        KI_add = 85;
        KD_add = 87;
        KC_add = 89;
        break;

    case PIDSET_SPD:
        KP_add = 121;
        KI_add = 123;
        KD_add = 125;
        KC_add = 127;
        break;

    default:
        KP_add = 7;
        KI_add = 9;
        KD_add = 11;
        KC_add = 13;
        break;
    }

    // KP
    if (Read_Reg_Micro( KP_add, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( KP_add+1, &valL))
    {
        return MOTOR_ERROR;
    }
    *KP = (valH << 16) + valL;

    // KI
    if (Read_Reg_Micro( KI_add, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( KI_add+1, &valL))
    {
        return MOTOR_ERROR;
    }
    *KI = (valH << 16) + valL;

    // KD
    if (Read_Reg_Micro( KD_add, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( KD_add+1, &valL))
    {
        return MOTOR_ERROR;
    }
    *KD = (valH << 16) + valL;

    // KC
    if (Read_Reg_Micro( KC_add, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( KC_add+1, &valL))
    {
        return MOTOR_ERROR;
    }
    *KC = (valH << 16) + valL;

    // OutMax
    if (Read_Reg_Micro( 15, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 16, &valL))
    {
        return MOTOR_ERROR;
    }
    *OMax = (valH << 16) + valL;

    // OutMin
    if (Read_Reg_Micro( 17, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 18, &valL))
    {
        return MOTOR_ERROR;
    }
    *OMin = (valH << 16) + valL;

    return MOTOR_OK;
}
int BrushlessModule::SetPIDPosParams( unsigned short set, long KP, long KI, long KD, long KC, long OMax, long OMin )
{
    unsigned char KP_add, KI_add, KD_add, KC_add;

    switch( set )
    {
    case PIDSET_DEF:
        KP_add = 19;
        KI_add = 21;
        KD_add = 23;
        KC_add = 25;
        break;

    case PIDSET_0:
        KP_add = 91;
        KI_add = 93;
        KD_add = 95;
        KC_add = 97;
        break;

    default:
        KP_add = 19;
        KI_add = 21;
        KD_add = 23;
        KC_add = 25;
        break;
    }

    // Setta il KP
    if( Write_Reg_Micro( KP_add, KP >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( KP_add+1, KP & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta il KI
    if( Write_Reg_Micro( KI_add, KI >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( KI_add+1, KI & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta il KD
    if( Write_Reg_Micro( KD_add, KD >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( KD_add+1, KD & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta il KC
    if( Write_Reg_Micro( KC_add, KC >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( KC_add+1, KC & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta OutMax
    if( Write_Reg_Micro( 27, OMax >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 28, OMax & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Setta OutMin
    if( Write_Reg_Micro( 29, OMin >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 30, OMin & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Aggiorna i valori del PID (se aggiornato il set per motore fermo...)
    if( set == PIDSET_0)
    {
        if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, PIDPOSUPDATE_CMD, m_Data, serialComStatus ) )
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
int BrushlessModule::GetPIDPosParams( unsigned short set, long *KP, long *KI, long *KD, long *KC, long *OMax, long *OMin )
{
    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;
    unsigned char KP_add, KI_add, KD_add, KC_add;

    switch( set )
    {
    case PIDSET_DEF:
        KP_add = 19;
        KI_add = 21;
        KD_add = 23;
        KC_add = 25;
        break;

    case PIDSET_0:
        KP_add = 91;
        KI_add = 93;
        KD_add = 95;
        KC_add = 97;
        break;

    default:
        KP_add = 19;
        KI_add = 21;
        KD_add = 23;
        KC_add = 25;
        break;
    }

    // KP
    if (Read_Reg_Micro( KP_add, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( KP_add+1, &valL))
    {
        return MOTOR_ERROR;
    }
    *KP = (valH << 16) + valL;

    // KI
    if (Read_Reg_Micro( KI_add, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( KI_add+1, &valL))
    {
        return MOTOR_ERROR;
    }
    *KI = (valH << 16) + valL;

    // KD
    if (Read_Reg_Micro( KD_add, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( KD_add+1, &valL))
    {
        return MOTOR_ERROR;
    }
    *KD = (valH << 16) + valL;

    // KC
    if (Read_Reg_Micro( KC_add, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( KC_add+1, &valL))
    {
        return MOTOR_ERROR;
    }
    *KC = (valH << 16) + valL;

    // OutMax
    if (Read_Reg_Micro( 27, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 28, &valL))
    {
        return MOTOR_ERROR;
    }
    *OMax = (valH << 16) + valL;

    // OutMin
    if (Read_Reg_Micro( 29, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 30, &valL))
    {
        return MOTOR_ERROR;
    }
    *OMin = (valH << 16) + valL;

    return MOTOR_OK;
}
long BrushlessModule::GetLineEncoder()
{
    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;
    if (Read_Reg_Micro( 55, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro( 56, &valL))
    {
        return MOTOR_ERROR;
    }

    return (valH << 16) + valL;
}

int BrushlessModule::SetLineEncoder( long lines )
{
    if( Write_Reg_Micro( 55, lines >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 56, lines & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Aggiorna i valori
    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, LINEENCUPDATE_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}

long BrushlessModule::GetPulsesPerRev()
{
    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;
    if( Read_Reg_Micro( 138, &valH) )
    {
        return MOTOR_ERROR;
    }
    if( Read_Reg_Micro( 139, &valL) )
    {
        return MOTOR_ERROR;
    }

    return (valH << 16) + valL;
}
int BrushlessModule::SetPulsesPerRev( long ppr )
{
    if( Write_Reg_Micro( 138, ppr >> 16) )
    {
        return MOTOR_ERROR;
    }
    if( Write_Reg_Micro( 139, ppr & 0xFFFF) )
    {
        return MOTOR_ERROR;
    }

    // Aggiorna i valori
    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, PPRUPDATE_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetPoles()
{
    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    unsigned short val = 0;
    if (Read_Reg_Micro( 58, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetPoles( int poles )
{
    if( Write_Reg_Micro( 58, poles) )
    {
        return MOTOR_ERROR;
    }

    // Aggiorna i valori
    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, ENCPARAMUPDATE_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetCalibratedAngle()
{
    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    unsigned short val = 0;
    if (Read_Reg_Micro( 59, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetCalibratedAngle( int angle )
{
    if( Write_Reg_Micro( 59, angle) )
    {
        return MOTOR_ERROR;
    }

    // Aggiorna i valori
    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, CALANGUPDATE_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetLoggerTrigger()
{
    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    unsigned short val = 0;
    if (Read_Reg_Micro( 60, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetLoggerTrigger( int trig )
{
    if( Write_Reg_Micro( 60, trig) )
    {
        return MOTOR_ERROR;
    }

    // Aggiorna i valori
    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, LOGTRIGUPDATE_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetLoggerPrescaler()
{
    if( m_Address == 0 )
    {
        return MOTOR_ERROR;
    }

    unsigned short val = 0;
    if (Read_Reg_Micro( 61, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetLoggerPrescaler( int prescal )
{
    if( Write_Reg_Micro( 61, prescal) )
    {
        return MOTOR_ERROR;
    }

    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, LOGPRESCALUPDATE_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetLoggerChannelData( int channel )
{
    unsigned char cmd1, cmd2;
    switch( channel )
    {
    case LOG_CH1:
        cmd1 = GETLOG1L_CMD;
        cmd2 = GETLOG1H_CMD;
        break;

    case LOG_CH2:
        cmd1 = GETLOG2L_CMD;
        cmd2 = GETLOG2H_CMD;
        break;

    case LOG_CH3:
        cmd1 = GETLOG3L_CMD;
        cmd2 = GETLOG3H_CMD;
        break;

    case LOG_CH4:
        cmd1 = GETLOG4L_CMD;
        cmd2 = GETLOG4H_CMD;
        break;

    default:
        return MOTOR_ERROR;
        break;
    }

    // Legge la parte bassa del buffer
    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, cmd1, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }
    for( int i=0; i<50; i++ )
        bufDataLog[channel][i] = (m_Data[i*2]<<8) + m_Data[i*2+1];

    // Legge la parte alta del buffer
    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, cmd2, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }
    for( int i=0; i<50; i++ )
        bufDataLog[channel][i+50] = (m_Data[i*2]<<8) + m_Data[i*2+1];

    return MOTOR_OK;
}
int BrushlessModule::SetLoggerChannelInput(int channel, int input)
{
    if (Write_Reg_Micro(62 + channel, input))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetEncoderActualSpeed(int mode)
{
    long val = 0;

    if (mode == SPEED_PU)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETSPEEDPU_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    else if (mode == SPEED_RPM)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETSPEEDRPM_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    else if (mode == SPEED_MS)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETSPEEDMS_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
int BrushlessModule::PWMEnable(int command)
{
    if (command == PWM_OFF)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, PWMDISABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    else if (command == PWM_ON)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, PWMENABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
int BrushlessModule::PhaseEnable(int phase, int command)
{
    if (command == PWM_OFF)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, PHASEDISABLE_CMD, phase, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    else if (command == PWM_ON)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, PHASEENABLE_CMD, phase, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
int BrushlessModule::ADCCalEnable(int command)
{
    if (command == ADCCAL_OFF)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, ADCCALDISABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    else if (command == ADCCAL_ON)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, ADCCALENABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
long BrushlessModule::GetClarkeAs()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETCLARKEA_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetClarkeBs()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETCLARKEB_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetADCOffset(int channel)
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (channel == ADC_CHANNEL0)
    {
        if (Read_Reg_Micro(66, &valH))
        {
            return MOTOR_ERROR;
        }
        if (Read_Reg_Micro(67, &valL))
        {
            return MOTOR_ERROR;
        }
    }
    else if (channel == ADC_CHANNEL1)
    {
        if (Read_Reg_Micro(68, &valH))
        {
            return MOTOR_ERROR;
        }
        if (Read_Reg_Micro(69, &valL))
        {
            return MOTOR_ERROR;
        }
    }
    else if (channel == ADC_CHANNEL2)
    {
        if (Read_Reg_Micro(136, &valH))
        {
            return MOTOR_ERROR;
        }
        if (Read_Reg_Micro(137, &valL))
        {
            return MOTOR_ERROR;
        }
    }
    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetADCOffset(int channel, long val)
{
    if (channel == ADC_CHANNEL0)
    {
        if (Write_Reg_Micro(66, val >> 16))
        {
            return MOTOR_ERROR;
        }
        if (Write_Reg_Micro(67, val & 0xFFFF))
        {
            return MOTOR_ERROR;
        }
    }
    else if (channel == ADC_CHANNEL1)
    {
        if (Write_Reg_Micro(68, val >> 16))
        {
            return MOTOR_ERROR;
        }
        if (Write_Reg_Micro(69, val & 0xFFFF))
        {
            return MOTOR_ERROR;
        }
    }
    else if (channel == ADC_CHANNEL2)
    {
        if (Write_Reg_Micro(136, val >> 16))
        {
            return MOTOR_ERROR;
        }
        if (Write_Reg_Micro(137, val & 0xFFFF))
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
long BrushlessModule::GetPIDIqOut()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETPIDIQOUT_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetPIDIdOut()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETPIDIDOUT_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetPIDSpdOut()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETPIDSPDOUT_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetPIDPosOut()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETPIDPOSOUT_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetPIDIqFdb()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETPIDIQFDB_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetPIDIdFdb()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETPIDIDFDB_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetPIDSpdFdb()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETPIDSPDFDB_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetPIDPosFdb()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETPIDPOSFDB_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetIqRef()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(70, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(71, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetIqRef(long val)
{
    if (Write_Reg_Micro(70, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(71, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetIdRef()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(72, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(73, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetIdRef(long val)
{
    if (Write_Reg_Micro(72, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(73, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetEncoderType()
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETENCODERTYPE_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return static_cast<int>(m_Data[0]);
}
int BrushlessModule::SetEncoderType(int type)
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, SETENCODERTYPE_CMD, type, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::ElecThetaGeneration(int mode)
{
    if (mode == ELECTTHETA_ON)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, ENCODERENABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    else
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, ENCODERDISABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
int BrushlessModule::GetMaxSpeed()
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short val = 0;
    if (Read_Reg_Micro(78, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetMaxSpeed(int spd)
{
    if (Write_Reg_Micro(78, spd))
    {
        return MOTOR_ERROR;
    }

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, ENCPARAMUPDATE_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetRamps()
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short val = 0;
    if (Read_Reg_Micro(77, &val))
    {
        return MOTOR_ERROR;
    }

    return (val >> MODE_RAMP) & 0x0001;
}
int BrushlessModule::SetRamps(int mode)
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short val = 0;
    if (Read_Reg_Micro(77, &val))
    {
        return MOTOR_ERROR;
    }

    if (mode == RAMP_ON)
    {
        val &= ~(1 << MODE_RAMP);
    }
    else
    {
        val |= 1 << MODE_RAMP;
    }

    if (Write_Reg_Micro(77, val))
    {
        return MOTOR_ERROR;
    }
    return MOTOR_OK;
}
long BrushlessModule::ActualStartStopVelocity()
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;
    if (Read_Reg_Micro(79, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(80, &valL))
    {
        return MOTOR_ERROR;
    }

    return (valH << 16) + valL;
}
long BrushlessModule::ActualAceleration()
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;
    if (Read_Reg_Micro(81, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(82, &valL))
    {
        return MOTOR_ERROR;
    }

    return (valH << 16) + valL;
}
long BrushlessModule::ActualDeceleration()
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;
    if (Read_Reg_Micro(103, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(104, &valL))
    {
        return MOTOR_ERROR;
    }

    return (valH << 16) + valL;
}
int BrushlessModule::SetActualPIDSpd(unsigned short set)
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, SETACTPIDSPD_CMD, set, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SetActualPIDPos(unsigned short set)
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, SETACTPIDPOS_CMD, set, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::LoggerTriggerMode(int mode)
{
    if (mode == LOWTOHIGH_TRIG)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, LOWTOHIGHTRIG_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    else
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, HIGHTOLOWTRIG_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
long BrushlessModule::GetEncoderActualPulses()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETENCODERPULSES_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
int BrushlessModule::SetPhaseCurrent(long val)
{
    if (Write_Reg_Micro(99, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(100, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetPhaseCurrent()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(99, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(100, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::GetDecelerationAdvance()
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short val = 0;
    if (Read_Reg_Micro(101, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetDecelerationAdvance(int val)
{
    if (Write_Reg_Micro(101, val))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetPulsesToMm()
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short val = 0;
    if (Read_Reg_Micro(102, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetPulsesToMm(int ptm)
{
    if (Write_Reg_Micro(102, ptm))
    {
        return MOTOR_ERROR;
    }

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, PTMUPDATE_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}

int BrushlessModule::DecCompensationMode(int mode){
    if (mode == DECCOMP_ON)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, DECCOMPENABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    else
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, DECCOMPDISABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
int BrushlessModule::SetProximityGap(long val)
{
    if (Write_Reg_Micro(105, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(106, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetProximityGap()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(105, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(106, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetPIDPosActivationGap(long val)
{
    if (Write_Reg_Micro(107, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(108, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetPIDPosActivationGap()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(107, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(108, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetIParkAngle(long val)
{
    if (Write_Reg_Micro(109, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(110, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetIParkAngle()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(109, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(110, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
long BrushlessModule::GetDangerLimit(int limit)
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (limit == LOW)
    {
        if (Read_Reg_Micro(111, &valH))
        {
            return MOTOR_ERROR;
        }
        if (Read_Reg_Micro(112, &valL))
        {
            return MOTOR_ERROR;
        }
    }
    else
    {
        if (Read_Reg_Micro(113, &valH))
        {
            return MOTOR_ERROR;
        }
        if (Read_Reg_Micro(114, &valL))
        {
            return MOTOR_ERROR;
        }
    }
    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetDangerLimit(int limit, long val)
{
    if (limit == LOW)
    {
        if (Write_Reg_Micro(111, val >> 16))
        {
            return MOTOR_ERROR;
        }
        if (Write_Reg_Micro(112, val & 0xFFFF))
        {
            return MOTOR_ERROR;
        }
    }
    else
    {
        if (Write_Reg_Micro(113, val >> 16))
        {
            return MOTOR_ERROR;
        }
        if (Write_Reg_Micro(114, val & 0xFFFF))
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
int BrushlessModule::DangerLimitMode(int mode)
{
    if (mode == DANGERLIMIT_ON)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, DANGERENABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    else
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, DANGERDISABLE_CMD, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
int BrushlessModule::ResetEncoder()
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, RESETENCODER_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::HomeSensorInput(int val)
{
    if (Write_Reg_Micro(115, val))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetHomingSpeed(int speed)
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (speed == HOME_SLOW)
    {
        if (Read_Reg_Micro(118, &valH))
        {
            return MOTOR_ERROR;
        }
        if (Read_Reg_Micro(119, &valL))
        {
            return MOTOR_ERROR;
        }
    }
    else
    {
        if (Read_Reg_Micro(116, &valH))
        {
            return MOTOR_ERROR;
        }
        if (Read_Reg_Micro(117, &valL))
        {
            return MOTOR_ERROR;
        }
    }
    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetHomingSpeed(int speed, long val)
{
    if (speed == HOME_SLOW)
    {
        if (Write_Reg_Micro(118, val >> 16))
        {
            return MOTOR_ERROR;
        }
        if (Write_Reg_Micro(119, val & 0xFFFF))
        {
            return MOTOR_ERROR;
        }
    }
    else
    {
        if (Write_Reg_Micro(116, val >> 16))
        {
            return MOTOR_ERROR;
        }
        if (Write_Reg_Micro(117, val & 0xFFFF))
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}
int BrushlessModule::GetOverSpeed()
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short val = 0;
    if (Read_Reg_Micro(120, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetOverSpeed(int val)
{
    if (Write_Reg_Micro(120, val))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SetReferenceActualPos()
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, SETREFACTUALPOS_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::ResetPIDSpd()
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, PIDSPDRESET_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::ResetPIDPos()
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, PIDPOSRESET_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::PIDStatus()
{
    unsigned short val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    if (Read_Reg_Micro(133, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::GetMaxReadCurrent()
{
    unsigned short val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    if (Read_Reg_Micro(134, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetMaxReadCurrent(int val)
{
    if (Write_Reg_Micro(134, val))
    {
        return MOTOR_ERROR;
    }

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, UPDATECURRENT_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetPhaseC()
{
    long val = 0;

    if (TWSBus1_Send_Command(m_comInt, m_Address, GETPHASEC_CMD, m_Data, serialComStatus))
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetEncoder360ActualPosition()
{
    long val = 0;

    if (TWSBus1_Send_Command(m_comInt, m_Address, GETENCODER360_CMD, m_Data, serialComStatus))
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetElecTheta()
{
    long val = 0;

    if (TWSBus1_Send_Command(m_comInt, m_Address, GETELECTHETA_CMD, m_Data, serialComStatus))
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetAccFFWD()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(147, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(148, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetAccFFWD(long val)
{
    if (Write_Reg_Micro(147, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(148, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetSpdFFWD()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(149, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(150, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetSpdFFWD(long val)
{
    if (Write_Reg_Micro(149, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(150, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetMaxJerk()
{
    unsigned short val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    if (Read_Reg_Micro(144, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetMaxJerk(int val)
{
    if (Write_Reg_Micro(144, val))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetCFriction()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(153, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(154, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetCFriction(long val)
{
    if (Write_Reg_Micro(153, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(154, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetEndMovementSpeed()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(145, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(146, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetEndMovementSpeed(long val)
{
    if (Write_Reg_Micro(145, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(146, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetEndMovementDelta()
{
    unsigned short val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(142, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(143, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetEndMovementDelta(long val)
{
    if (Write_Reg_Micro(142, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(143, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GotoPos0_Multi(unsigned char addX, long posX, unsigned char addY, long posY, long maxFreq, long acc, long proximityDeltaX, long proximityDeltaY)
{
    unsigned char tx_buffer[26];
    tx_buffer[0] = addX;
    tx_buffer[1] = posX >> 24;
    tx_buffer[2] = posX >> 16;
    tx_buffer[3] = posX >> 8;
    tx_buffer[4] = posX;
    tx_buffer[5] = proximityDeltaX >> 24;
    tx_buffer[6] = proximityDeltaX >> 16;
    tx_buffer[7] = proximityDeltaX >> 8;
    tx_buffer[8] = proximityDeltaX;
    tx_buffer[9] = addY;
    tx_buffer[10] = posY >> 24;
    tx_buffer[11] = posY >> 16;
    tx_buffer[12] = posY >> 8;
    tx_buffer[13] = posY;
    tx_buffer[14] = proximityDeltaY >> 24;
    tx_buffer[15] = proximityDeltaY >> 16;
    tx_buffer[16] = proximityDeltaY >> 8;
    tx_buffer[17] = proximityDeltaY;
    tx_buffer[18] = maxFreq >> 24;
    tx_buffer[19] = maxFreq >> 16;
    tx_buffer[20] = maxFreq >> 8;
    tx_buffer[21] = maxFreq;
    tx_buffer[22] = acc >> 24;
    tx_buffer[23] = acc >> 16;
    tx_buffer[24] = acc >> 8;
    tx_buffer[25] = acc;

    errorCode = TWSBus1_Send(m_comInt, m_Address, MULTIMOVE_CMD, tx_buffer, 26);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::ResetFlag_Multi()
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, RESET_MULTIMOVE_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetEncoder2ActualPosition()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETENCODER2_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetEncoder2ActualPulses()
{
    long val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETENCODER2PULSES_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
long BrushlessModule::GetLineEncoder2()
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;
    if (Read_Reg_Micro(140, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(141, &valL))
    {
        return MOTOR_ERROR;
    }

    return (valH << 16) + valL;
}
int BrushlessModule::SetLineEncoder2(long lines)
{
    if (Write_Reg_Micro(140, lines >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(141, lines & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, LINEENC2UPDATE_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetEncoderInterpActualPosition()
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETENCODERINTERP_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    long val = (static_cast<long>(m_Data[0]) << 24) + (static_cast<long>(m_Data[1]) << 16) + (static_cast<long>(m_Data[2]) << 8) + static_cast<long>(m_Data[3]);

    return val;
}
int BrushlessModule::GetStatus_ResetFlagMulti()
{
    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETSTATUS_RESETMM_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    long val = (m_Data[0] << 8) + m_Data[1];

    return val;
}
int BrushlessModule::SetFollowingError(long val)
{
    if (Write_Reg_Micro(162, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(163, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetFollowingError()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(162, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(163, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::EncoderStatus()
{
    unsigned short val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    if (Read_Reg_Micro(164, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetStartTicks(int ticks)
{
    if (Write_Reg_Micro(2, ticks & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SetPhaseRotation(int value)
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, ROTATEPHASE_CMD, value, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SetPhaseRotationNumber(int value)
{
    if (Write_Reg_Micro(57, value))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SetSpeedFilter(unsigned short window)
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, UPDATESPDFILTER_CMD, window, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SetSecurityCheck(int limit, int limitLevel)
{
    unsigned short val = 0;

    if (Read_Reg_Micro(77, &val))
    {
        return MOTOR_ERROR;
    }

    if (limit == SECURITYCHECK_ON)
    {
        val |= (1 << MODE_SECURITY);
    }
    else
    {
        val &= ~(1 << MODE_SECURITY);
    }

    if (limitLevel == SECURITYLEVEL_HIGH)
    {
        val |= (1 << MODE_SECURITYLEVEL);
    }
    else
    {
        val &= ~(1 << MODE_SECURITYLEVEL);
    }

    if (Write_Reg_Micro(77, val))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SetSecurityInput(int val)
{
    if (Write_Reg_Micro(166, (val & 0x0000FFFF)))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetVPower()
{
    if( errorCode = TWSBus1_Send_Command( m_comInt, m_Address, GETVPWR_CMD, m_Data, serialComStatus ) )
    {
        return MOTOR_ERROR;
    }

    long val = (m_Data[0]<<24) + (m_Data[1]<<16) + (m_Data[2]<<8) + m_Data[3];
    return val;
}
int BrushlessModule::SetSteadyPosFollowError(long val)
{
    if (Write_Reg_Micro(167, val >> 16))
    {
        return MOTOR_ERROR;
    }
    if (Write_Reg_Micro(168, val & 0xFFFF))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
long BrushlessModule::GetSteadyPosFollowError()
{
    long val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    unsigned short valH = 0, valL = 0;

    if (Read_Reg_Micro(167, &valH))
    {
        return MOTOR_ERROR;
    }
    if (Read_Reg_Micro(168, &valL))
    {
        return MOTOR_ERROR;
    }

    val = (valH << 16) + valL;

    return val;
}
int BrushlessModule::SetEncoderSpykeDelta(int delta)
{
    if (Write_Reg_Micro(169, (delta & 0x0000FFFF)))
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetEncoderSpykeDelta()
{
    unsigned short val = 0;

    if (m_Address == 0)
    {
        return MOTOR_ERROR;
    }

    if (Read_Reg_Micro(169, &val))
    {
        return MOTOR_ERROR;
    }

    return val;
}
int BrushlessModule::SetSpeedProfile(int mode)
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, SETPROFILE_CMD, mode, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::GetTemperature()
{
    int val = 0;

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, GETSENSORTEMP_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }
    val = (m_Data[0] << 8) + m_Data[1];

    return val;
}
int BrushlessModule::UpdateOverSpeed()
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, UPDATEOVERSPEED_CMD, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::OverSpeedCheck(int enable)
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, OVERSPEEDCHECK_CMD, enable, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SetHomeSensorLevel(int level)
{
    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, HOMESENSORMODE_CMD, level, m_Data, serialComStatus);
    if (errorCode != 0)
    {
        return MOTOR_ERROR;
    }

    return MOTOR_OK;
}
int BrushlessModule::SetOutputs(int outId, int outVal)
{
    if (outVal == LOW)
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, SET_OUTPUT_LOW_CMD, outId, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }
    else
    {
        errorCode = TWSBus1_Send_Command(m_comInt, m_Address, SET_OUTPUT_HIGH_CMD, outId, m_Data, serialComStatus);
        if (errorCode != 0)
        {
            return MOTOR_ERROR;
        }
    }

    return MOTOR_OK;
}

int BrushlessModule::ResetSpeed()
{
    if (ver < 5 || (ver == 5 && rev < 1))
    {
        return MOTOR_OK;
    }

    errorCode = TWSBus1_Send_Command(m_comInt, m_Address, RESETSPEED_CMD, m_Data, serialComStatus);
    return errorCode ? MOTOR_ERROR : MOTOR_OK;
}
//DONE
