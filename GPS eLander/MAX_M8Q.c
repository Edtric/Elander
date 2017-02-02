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

char NMEASentence[85];    // Buffer for storing NMEA sentence
char read;
char output[50];          // UART output buffer, used only for debugging
char *token;              // Holds the string token from strtok function
char Check_Sum[3];        // Holds unparsed checksum

const char *delim = ",";  // String delimiter

uint8 element = 0;        // Element of NMEA sentence
uint8 endSentence;        // Flag for end of NMEA sentence
uint8 i;
uint8 j;
uint8 timeStringPos;      // Position/element of unconverted time token.
uint8 stringPos;          // Position/element of string token.
uint8 substringPos;       // Position/element of substring token.

bool placeHolder;
bool beginSave = false;   // Flag determining when to start saving full NMEA string

int checksumVal;          // Calculated numeric checksum value (not ASCII)
int checksumXOR;          // Variable holding XOR'd values of each byte in string




/*===========================================================================================
       Name: Time_Convert
     Inputs: void
    Outputs: void
Description: Converts the acquired time values into hours, minutes, and seconds.
===========================================================================================*/
void Time_Convert( void )
{
    FixData.time_hr = atoi( StringTime.hours );
    FixData.time_min = atoi( StringTime.minutes );
    FixData.time_sec = atof( StringTime.seconds );
}
/*=========================================================================================*/



/*===========================================================================================
       Name: Time_Parse
     Inputs: void
    Outputs: void
Description: Parses the acquired time values into their respective substrings of hours, 
             minutes, and seconds.
===========================================================================================*/
void Time_Parse( void )
{
    timeStringPos = 0;
    
    while( timeStringPos < 9 )
    {
        switch( timeStringPos )
        {
            case 0:
            StringTime.hours[0] = FixString.time[timeStringPos];
            break;
            case 1:
            StringTime.hours[1] = FixString.time[timeStringPos];
            break;
            case 2:
            StringTime.minutes[0] = FixString.time[timeStringPos];
            break;
            case 3:
            StringTime.minutes[1] = FixString.time[timeStringPos];
            break;
            case 4:
            StringTime.seconds[0] = FixString.time[timeStringPos];
            break;
            case 5:
            StringTime.seconds[1] = FixString.time[timeStringPos];
            break;
            case 6:
            StringTime.seconds[2] = FixString.time[timeStringPos];
            break;
            case 7:
            StringTime.seconds[3] = FixString.time[timeStringPos];
            break;
            default:
            StringTime.seconds[4] = FixString.time[timeStringPos];
        }
        timeStringPos++;
    }
}
/*=========================================================================================*/



/*===========================================================================================
       Name: NMEA_Token
     Inputs: char *Fix_String: Substring that will store token of NMEA string.
    Outputs: void
Description: Breaks up the unparsed NMEA string into individual tokens to be placed in their 
             respective substring.
===========================================================================================*/
void NMEA_Token( char *Fix_String )
{
    substringPos = 0;
    while( *(NMEASentence + stringPos) != ',' && *(NMEASentence + stringPos) != '\r' && *(NMEASentence + stringPos) != '\n' )
    {
        *(Fix_String + substringPos) = *(NMEASentence + stringPos);
        substringPos++;
        stringPos++;
    }
    stringPos++;
}
/*=========================================================================================*/



/*===========================================================================================
       Name: Checksum_Token
     Inputs: char *subString: Parsed string containing checksum hex value.
    Outputs: void
Description: Removes checksum in string form from Reference ID substring, and places it in its
             own substring.
===========================================================================================*/
void Checksum_Token( void )
{
    j = 0;
    substringPos = 0;
    
    // Increment through Reference ID substring until checksum section
    while( *(FixString.ref_ID + substringPos) != '*' )
    {
        substringPos++;
    }
    
    // Clear the '*'
    *(FixString.ref_ID + substringPos) = '\0';
    substringPos++;
    
    // Clear the checksum section of original substring
    while( *(FixString.ref_ID + substringPos) != '\0' )
    {
        Check_Sum[j] = *(FixString.ref_ID + substringPos);
        *(FixString.ref_ID + substringPos) = '\0';
        substringPos++;
        j++;
    }
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
    stringPos = 6;
    
    // Makes sure that the substrings are all empty.
    memset( FixString.time , '\0' , strlen(FixString.time) );
    memset( FixString.lat , '\0' , strlen(FixString.lat) );
    memset( FixString.dir_lat , '\0' , strlen(FixString.dir_lat) );
    memset( FixString.lon , '\0' , strlen(FixString.lon) );
    memset( FixString.dir_lon , '\0' , strlen(FixString.dir_lon) );
    memset( FixString.quality , '\0' , strlen(FixString.quality) );
    memset( FixString.num_sat , '\0' , strlen(FixString.num_sat) );
    memset( FixString.HDOP , '\0' , strlen(FixString.HDOP) );
    memset( FixString.altitude , '\0' , strlen(FixString.altitude) );
    memset( FixString.M1 , '\0' , strlen(FixString.M1) );
    memset( FixString.geoidal_sep , '\0' , strlen(FixString.geoidal_sep) );
    memset( FixString.M2 , '\0' , strlen(FixString.M2) );
    memset( FixString.data_age , '\0' , strlen(FixString.data_age) );
    memset( FixString.ref_ID , '\0' , strlen(FixString.ref_ID) );
    
    // Breaks the NMEA string into smaller substrings(tokens).
    NMEA_Token( FixString.time );
    NMEA_Token( FixString.lat );
    NMEA_Token( FixString.dir_lat );
    NMEA_Token( FixString.lon );
    NMEA_Token( FixString.dir_lon );
    NMEA_Token( FixString.quality );
    NMEA_Token( FixString.num_sat );
    NMEA_Token( FixString.HDOP );
    NMEA_Token( FixString.altitude );
    NMEA_Token( FixString.M1 );
    NMEA_Token( FixString.geoidal_sep );
    NMEA_Token( FixString.M2 );
    NMEA_Token( FixString.data_age );
    NMEA_Token( FixString.ref_ID );
    
    // Split checksum from ref_ID token
    Checksum_Token();

    // UART outputs for debugging
    UART_DEBUG_PutString( "time: " );
    UART_DEBUG_PutString( FixString.time );
    UART_DEBUG_PutChar('\r');
    
    UART_DEBUG_PutString( "lat: " );
    UART_DEBUG_PutString( FixString.lat );
    UART_DEBUG_PutString(" ");
    UART_DEBUG_PutString( FixString.dir_lat );
    UART_DEBUG_PutChar('\r');
    
    UART_DEBUG_PutString( "lon: " );
    UART_DEBUG_PutString( FixString.lon );
    UART_DEBUG_PutString(" ");
    UART_DEBUG_PutString( FixString.dir_lon );
    UART_DEBUG_PutChar('\r');
    
//    UART_DEBUG_PutString( "quality: " );
//    UART_DEBUG_PutString( FixString.quality );
//    UART_DEBUG_PutChar('\r');
//    
//    UART_DEBUG_PutString( "num_sat: " );
//    UART_DEBUG_PutString( FixString.num_sat );
//    UART_DEBUG_PutChar('\r');
    
//    UART_DEBUG_PutString( "HDOP: " );
//    UART_DEBUG_PutString( FixString.HDOP );
//    UART_DEBUG_PutChar('\r');
    
//    UART_DEBUG_PutString( "altitude: " );
//    UART_DEBUG_PutString( FixString.altitude );
//    UART_DEBUG_PutChar('\r');
    
//    UART_DEBUG_PutString( "geoidal_sep: " );
//    UART_DEBUG_PutString( FixString.geoidal_sep );
//    UART_DEBUG_PutChar('\r');
    
//    UART_DEBUG_PutString( "data_age: " );
//    UART_DEBUG_PutString( FixString.data_age );
//    UART_DEBUG_PutChar('\r');
    
    UART_DEBUG_PutString( "ref_ID: " );
    UART_DEBUG_PutString( FixString.ref_ID );
    UART_DEBUG_PutString("\r");
    
    UART_DEBUG_PutString( "Checksum: " );
    UART_DEBUG_PutString( Check_Sum );
    UART_DEBUG_PutString("\r\n");
    
    // Converts tokens from strings to numerical values.
    Time_Parse();
    FixData.lat = atof( FixString.lat );
    FixData.lon = atof( FixString.lon );
    FixData.quality = atoi( FixString.quality );
    FixData.num_sat = atoi( FixString.num_sat );
    FixData.HDOP = atof( FixString.HDOP );
    FixData.altitude = atof( FixString.altitude );
    FixData.geoidal_sep = atof( FixString.geoidal_sep );
    FixData.data_age = atof( FixString.data_age );
    FixData.ref_ID = atoi( FixString.ref_ID );
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
        GPGGA_Parse();
    }
    // If it's RMC.
    else if( NMEASentence[2] == 'R' && NMEASentence[3] == 'M' && NMEASentence[4] == 'C' )
    {
        GPRMC_Parse();
    }
}
/*=========================================================================================*/



/*===========================================================================================
       Name: GPS_I2C_Read
     Inputs: void
    Outputs: void
Description: Addresses the GPS device followed by the register address as the packet. Then,
             reads data from device and stores in a buffer.
===========================================================================================*/
void GPS_I2C_Read( void )
{
    GPS_Data.gps_stream = I2C_ReadByte( MAX_M8Q_ADDRESS , DATA_STREAM_REG );
    GPS_Data.ASCII = GPS_Data.gps_stream;
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
Description: Performs a checksum on the NMEA sentence to ensure data integrity. All elements
             after '$' is XOR'd and compared to checksum value from the original NMEA string.
===========================================================================================*/
bool NMEA_Checksum( void )
{
    j = 0;
    
    // Convert checksum ASCII hex value into decimal value
    while( j < 2 )
    {
        if( (Check_Sum[j] > 47) && Check_Sum[j] < 58 ) Check_Sum[j] = Check_Sum[j] - 48; // if 0 - 9
        else if( Check_Sum[j] > 64 ) Check_Sum[j] = Check_Sum[j] - 55; // if A - F
        j++;
    }
    
    checksumVal = ( (Check_Sum[0] << 4) + Check_Sum[1] ); // Bit-shift and add to generate full value
    
    j = 0;
    checksumXOR = 0;
    
    // XOR all values after $ and before checksum delimiter, '*'
    while( NMEASentence[j] != '*' )
    {
        checksumXOR ^= NMEASentence[j];
        j++;
    }
    
    if( checksumXOR == checksumVal )
    {
        return true;
    }
    else return false;
}
/*=========================================================================================*/



/*===========================================================================================
       Name: GPS_I2C_Output
     Inputs: void
    Outputs: void
Description: .
===========================================================================================*/
//void GPS_I2C_Output( void )
//{
//    endSentence = 0; 
//    while( endSentence != 1 ) // While not the end of NMEA sentence
//    {
//        j = 0;
//        element = 0;
//        GPS_I2C_Read();
//        
//        // Look for the start of a NMEA sentence
//        if( GPS_Data.ASCII == '$' && (GPS_Data.ASCII < 128) && (GPS_Data.ASCII > 0) )
//        {
//            // Save the string to an array
//            while( (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') && (GPS_Data.ASCII != '*') )
//            {
//                GPS_I2C_Read();
//                NMEASentence[element] = GPS_Data.ASCII;
//                element++;
//            }
//            
//            if( GPS_Data.ASCII == '*' )
//            {
//                while( (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') )
//                {
//                    GPS_I2C_Read();
//                    Check_Sum[j] = GPS_Data.ASCII;
//                    j++;
//                }
//            }
//            // Performs checksum to ensure data is valid
////            NMEA_Checksum();
//            
//            // Check the next 5 elements for the NMEA sentence identifier, i.e. 'GPGGA'
//            NMEA_ID_Check();
//            
//            endSentence = 1; // End of NMEA sentence
//        }
//    }
//}
/*=========================================================================================*/



/*===========================================================================================
       Name: GPS_UART_Output
     Inputs: void
    Outputs: void
Description: .
===========================================================================================*/
//void GPS_UART_Output( void )
//{
//    endSentence = 0; 
//    while( endSentence != 1 ) // While not the end of NMEA sentence
//    {
//        j = 0;
//        element = 0;
//        GPS_UART_Read();
//        
//        // Look for the start of a NMEA sentence
//        if( GPS_Data.ASCII == '$' && (GPS_Data.ASCII < 128) && (GPS_Data.ASCII > 0) )
//        {
//            // Save the string to an array
//            while( (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') && (GPS_Data.ASCII != '*') )
//            {
//                GPS_UART_Read();
//                NMEASentence[element] = GPS_Data.ASCII;
//                element++;
//            }
//            
//            if( GPS_Data.ASCII == '*' )
//            {
//                while( (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') )
//                {
//                    GPS_UART_Read();
//                    Check_Sum[j] = GPS_Data.ASCII;
//                    j++;
//                }
//            }
//            // Performs checksum to ensure data is valid
////            NMEA_Checksum();
//            
//            // Check the next 5 elements for the NMEA sentence identifier, i.e. 'GPGGA'
//            NMEA_ID_Check();
//            
//            endSentence = 1; // End of NMEA sentence
//        }
//    }
//}
/*=========================================================================================*/



/*===========================================================================================
       Name: GPS_I2C_Gather_Data
     Inputs: void
    Outputs: void
Description: Main GPS data gathering function, utilizing I2C.
===========================================================================================*/
/** May be able to combine with GPS_UART_Gather_Data() **/
void GPS_I2C_Gather_Data( void )
{
    if( I2C_MasterGetReadBufSize() )
    {
        GPS_I2C_Read();
        if( GPS_Data.ASCII == '$' ) beginSave = true; // The '$' is ignored, i.e. not saved
        else if( (beginSave == true) && (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') ) NMEASentence[element++] = GPS_Data.ASCII;
        else if( (GPS_Data.ASCII == '\n') || (GPS_Data.ASCII == '\r') )
        {
            beginSave = false;
            element = 0;

            NMEA_ID_Check(); // Determine which NMEA ID is being used, and parse accordingly
            
            placeHolder = NMEA_Checksum(); // Perform checksum
    //        if( placeHolder ) UART_DEBUG_PutString( "Checksum True" );
    //        else if( !placeHolder ) UART_DEBUG_PutString( "Checksum False" );
            memset( NMEASentence , '\0' , strlen( NMEASentence ) ); // Clear the string
        }
        I2C_MasterClearReadBuf();
    }
}
/*=========================================================================================*/



/*===========================================================================================
       Name: GPS_UART_Gather_Data
     Inputs: void
    Outputs: void
Description: Main GPS data gathering function, utilizing UART.
===========================================================================================*/
/** May be able to combine with GPS_I2C_Gather_Data() **/
void GPS_UART_Gather_Data( void )
{
    if( UART_DEBUG_GetRxBufferSize() )
    {
        GPS_UART_Read();
        if( GPS_Data.ASCII == '$' ) beginSave = true; // The '$' is ignored, i.e. not saved
        else if( (beginSave == true) && (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') ) NMEASentence[element++] = GPS_Data.ASCII;
        else if( (GPS_Data.ASCII == '\n') || (GPS_Data.ASCII == '\r') )
        {
            beginSave = false;
            element = 0;

            NMEA_ID_Check(); // Determine which NMEA sentence ID is being used, and parse accordingly
            
            placeHolder = NMEA_Checksum(); // Perform checksum
    //        if( placeHolder ) UART_DEBUG_PutString( "Checksum True" );
    //        else if( !placeHolder ) UART_DEBUG_PutString( "Checksum False" );
            memset( NMEASentence , '\0' , strlen( NMEASentence ) ); // Clear the string
        }
        UART_DEBUG_ClearRxBuffer();
    }
}
/*=========================================================================================*/



/*===========================================================================================
       Name: 
     Inputs: void
    Outputs: void
Description: Interrupt-based version of GPS_UART_Gather_Data. 
===========================================================================================*/
CY_ISR ( RX_Int_Handle )
{
    GPS_UART_Read();
    if( GPS_Data.ASCII == '$' ) beginSave = 1;
    else if( (beginSave == 1) && (GPS_Data.ASCII != '\n') && (GPS_Data.ASCII != '\r') ) NMEASentence[element++] = GPS_Data.ASCII;
    else if( (GPS_Data.ASCII == '\n') || (GPS_Data.ASCII == '\r') )
    {
        beginSave = 0;
        element = 0;

        NMEA_ID_Check();
        
//        placeHolder = NMEA_Checksum();
//        if( placeHolder ) UART_DEBUG_PutString( "Checksum True" );
//        else if( !placeHolder ) UART_DEBUG_PutString( "Checksum False" );
        memset( NMEASentence , '\0' , strlen( NMEASentence ) );
    }
    RX_ISR_ClearPending();
}
/*=========================================================================================*/
/* [] END OF FILE */