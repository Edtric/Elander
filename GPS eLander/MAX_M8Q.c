/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "MAX_M8Q.h"
#include <stdio.h>

char NMEASentence[85];    // Buffer for storing NMEA sentence
uint8 element = 1;            // Element of NMEA sentence
uint8 endSentence;        // End of NMEA sentence condition
char read;
char output[50];

uint8 i;
uint8 j;
uint8 beginSave = 0;
int CHECKSUM;             // Final checksum value (not ASCII)
int chksum_xor;           // 

char *delim;              // String delimiter
char *token;              // Holds the string token from strtok function
char Check_Sum[2];        // Holds unparsed checksum



/*===========================================================================================
       Name: Time_Parse
     Inputs: void
    Outputs: void
Description: Parses the acquired time values into hours, minutes, and seconds.
===========================================================================================*/
void Time_Parse( void )
{
    
}
/*=========================================================================================*/



/*===========================================================================================
       Name: GPGGA_Parse
     Inputs: void
    Outputs: void
Description: Parses the acquired NMEA GPGGA string and converts them into values useable by
             the controller.
===========================================================================================*/
void GPGGA_Parse( void )
{
    i = 0;
    
    *delim = ',';
    token = strtok( NMEASentence , delim );
    i++;
    
    while( token != NULL )
    {
        token = strtok( NULL , delim );
        
        switch( i )
        {
        case 1:                                            
            FixData.time = atof( token );         // Time
            Time_Parse();
            break;
        case 2:
            FixData.lat = atoi( token );          // Latitude
            break;
        case 3:
            FixData.dir_lat = token;              // N or S
            break;
        case 4:
            FixData.lon = atoi( token );          // Longitude
            break;
        case 5:
            FixData.dir_lon = token;              // E or W
            break;
        case 6:
            FixData.quality = atoi( token );      // Fix quality
            break;
        case 7:
            FixData.num_sat = atoi( token );      // Number of satellites in view
            break;
        case 8:
            FixData.HDOP = atof( token );         // Horizontal Dilution of Precision
            break;
        case 9:
            FixData.altitude = atof( token );     // Altitude in meters above mean-sea-level
            break;
        case 11:
            FixData.geoidal_sep = atof( token );  // Geoidal separation in meters
            break;
        case 13:
            FixData.data_age = atof( token );     // Time since last differential GPS data update
            break;
        case 14:
            FixData.ref_ID = atoi( token );       // Reference station ID
            break;
        }
        
        i++;
    }
}
/*=========================================================================================*/



/*===========================================================================================
       Name: GPRMC_Parse
     Inputs: void
    Outputs: void
Description: Parses the acquired NMEA GPRMC string and converts them into values useable by
             the controller.
===========================================================================================*/
void GPRMC_Parse( void )
{
    
}



/*===========================================================================================
       Name: GPS_NMEA_ID
     Inputs: void
    Outputs: void
Description: Checks the first 6 elements for the NMEA sentence identifier, i.e. '$GPGGA...',
             but disregards the talker ID i.e.' 'GP...' or 'GN...'
===========================================================================================*/
void NMEA_ID_Check( void )
{
    // If it's GGA.
    if( NMEASentence[2] == 'G' && NMEASentence[3] == 'G' && NMEASentence[4] == 'A' )
    {
        // Use GGA parsing function
        GPGGA_Parse();
    }
    // If it's RMC.
    else if( NMEASentence[2] == 'R' && NMEASentence[3] == 'M' && NMEASentence[4] == 'C' )
    {
        // Use RMC parsing function
        GPRMC_Parse();
    }
}
/*=========================================================================================*/



/*===========================================================================================
       Name: GPS_I2C_Read
     Inputs: void
    Outputs: void
Description: Addresses the GPS device followed by the register address as the packet. Then,
             streams data from device and stores in a buffer.
===========================================================================================*/
void GPS_I2C_Read( void )
{
    GPS_Data.gps_stream = I2C_ReadByte( MAX_M8Q_ADDRESS , DATA_STREAM_REG );
    GPS_Data.ASCII = GPS_Data.gps_stream;
//    UART_DEBUG_PutChar( GPS_Data.ASCII );
}
/*=========================================================================================*/



/*===========================================================================================
       Name: GPS_UART_Read
     Inputs: void
    Outputs: void
Description: Streams data from device via UART while storing values in a buffer.
===========================================================================================*/
void GPS_UART_Read( void )
{
    GPS_Data.ASCII = UART_DEBUG_GetChar();
}
/*=========================================================================================*/



///*===========================================================================================
//       Name: MAX_M8Q_Get_Byte_Size
//     Inputs: void
//    Outputs: void
//Description: Acquires the size of GPS packet, and places it in a pointer for external 
//             use.
//===========================================================================================*/
//void MAX_M8Q_Get_Byte_Size( void )
//{
//    I2C_ReadBytes( MAX_M8Q_ADDRESS , NUM_BYTES_HI , 2 , size );
//}
///*=========================================================================================*/



/*===========================================================================================
       Name: NMEA_Checksum
     Inputs: void
    Outputs: void
Description: Performs a checksum on the NMEA sentence, to ensure .
===========================================================================================*/
uint8 NMEA_Checksum( void )
{
    j = 0;
    
    while( j < 2 )
    {
        if( (Check_Sum[j] > 47) && Check_Sum[j] < 58 ) Check_Sum[j] = Check_Sum[j] - 48;
        if( Check_Sum[j] > 64 ) Check_Sum[j] = Check_Sum[j] - 55;
        
        CHECKSUM = ( (Check_Sum[0] << 4) + Check_Sum[1] );
    }
    
    j = 0;
    chksum_xor = 0;
    
    while( NMEASentence[j] < strlen( NMEASentence ) )
    {
        chksum_xor ^= NMEASentence[j];
    }
    
    if( chksum_xor == CHECKSUM ) return 1;
    else return 0;
}
/*=========================================================================================*/



/*===========================================================================================
       Name: GPS_I2C_Output
     Inputs: void
    Outputs: void
Description: .
===========================================================================================*/
void GPS_I2C_Output( void )
{
    endSentence = 0; 
    while( endSentence != 1 ) // While not the end of NMEA sentence
    {
        j = 0;
        element = 0;
        GPS_I2C_Read();
        
        // Look for the start of a NMEA sentence
        if( GPS_Data.ASCII == '$' && (GPS_Data.ASCII < 128) && (GPS_Data.ASCII > 0) )
        {
            // Save the string to an array
            while( (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') && (GPS_Data.ASCII != '*') )
            {
                GPS_I2C_Read();
                NMEASentence[element] = GPS_Data.ASCII;
                element++;
            }
            
            if( GPS_Data.ASCII == '*' )
            {
                while( (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') )
                {
                    GPS_I2C_Read();
                    Check_Sum[j] = GPS_Data.ASCII;
                    j++;
                }
            }
            // Performs checksum to ensure data is valid
//            NMEA_Checksum();
            
            // Check the next 5 elements for the NMEA sentence identifier, i.e. 'GPGGA'
            NMEA_ID_Check();
            
            endSentence = 1; // End of NMEA sentence
        }
    }
}
/*=========================================================================================*/



/*===========================================================================================
       Name: GPS_UART_Output
     Inputs: void
    Outputs: void
Description: .
===========================================================================================*/
void GPS_UART_Output( void )
{
    endSentence = 0; 
    while( endSentence != 1 ) // While not the end of NMEA sentence
    {
        j = 0;
        element = 0;
        GPS_UART_Read();
        
        // Look for the start of a NMEA sentence
        if( GPS_Data.ASCII == '$' && (GPS_Data.ASCII < 128) && (GPS_Data.ASCII > 0) )
        {
            // Save the string to an array
            while( (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') && (GPS_Data.ASCII != '*') )
            {
                GPS_UART_Read();
                NMEASentence[element] = GPS_Data.ASCII;
                element++;
            }
            
            if( GPS_Data.ASCII == '*' )
            {
                while( (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') )
                {
                    GPS_UART_Read();
                    Check_Sum[j] = GPS_Data.ASCII;
                    j++;
                }
            }
            // Performs checksum to ensure data is valid
//            NMEA_Checksum();
            
            // Check the next 5 elements for the NMEA sentence identifier, i.e. 'GPGGA'
            NMEA_ID_Check();
            
            endSentence = 1; // End of NMEA sentence
        }
    }
}
/*=========================================================================================*/



/*===========================================================================================
       Name: 
     Inputs: void
    Outputs: void
Description: .
===========================================================================================*/
CY_ISR ( RX_Int_Handle )
{
//    UART_DEBUG_PutChar( UART_DEBUG_GetChar() );
    
    GPS_Data.ASCII = UART_DEBUG_GetChar();
    if( GPS_Data.ASCII == '$' ) beginSave = 1;
    else if( (beginSave == 1) && (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') ) NMEASentence[element++] = GPS_Data.ASCII;
    else if( (GPS_Data.ASCII == '\n') || (GPS_Data.ASCII == '\r') )
    {
        beginSave = 0;
        element = 1;
        NMEA_ID_Check();
        
        sprintf( output , " Position: %d%s , %d%s\n" , FixData.lat , FixData.dir_lat , FixData.lon , FixData.dir_lon );
        UART_DEBUG_PutString( output );
    }
    
    RX_ISR_ClearPending();
}
/*=========================================================================================*/
/* [] END OF FILE */