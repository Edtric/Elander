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
uint8 element;            // Element of NMEA sentence
uint8 endSentence;        // End of NMEA sentence condition
char read;

uint8 i;
uint8 j;
int CHECKSUM;             // Final checksum value (not ASCII)

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
        
        if( i == 1 )                                            // Time
        {
            FixData.time = atof( token );
            Time_Parse();
        }
        else if( i == 2 ) FixData.lat = atof( token );          // Latitude
        else if( i == 3 ) FixData.dir_lat = token;              // N or S
        else if( i == 4 ) FixData.lon = atof( token );          // Longitude
        else if( i == 5 ) FixData.dir_lon = token;              // E or W
        else if( i == 6 ) FixData.quality = atoi( token );      // Fix quality
        else if( i == 7 ) FixData.num_sat = atoi( token );      // Number of satellites in view
        else if( i == 8 ) FixData.HDOP = atof( token );         // Horizontal Dilution of Precision
        else if( i == 9 ) FixData.altitude = atof( token );     // Altitude in meters above mean-sea-level
        else if( i == 11 ) FixData.geoidal_sep = atof( token ); // Geoidal separation in meters
        else if( i == 13 ) FixData.data_age = atof( token );    // Time since last differential GPS data update
        else if( i == 14 ) FixData.ref_ID = atoi( token );      // Reference station ID
        
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
Description: Checks the first 6 elements for the NMEA sentence identifier, i.e. '$GPGGA...'
===========================================================================================*/
void NMEA_ID_Check( void )
{
    // If it's $GPGGA.
    if( NMEASentence[1] == 'P' && NMEASentence[2] == 'G' && NMEASentence[3] == 'G' && NMEASentence[4] == 'A' )
    {
        // Use GPGGA parsing function
        GPGGA_Parse();
    }
    // If it's $GPRMC.
    else if( NMEASentence[1] == 'P' && NMEASentence[2] == 'R' && NMEASentence[3] == 'M' && NMEASentence[4] == 'C' )
    {
        // Use GPRMC parsing function
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
    GPS_Data.gps_itoa = GPS_Data.gps_stream;
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
void NMEA_Checksum( void )
{
    j = 0;
    
    while( j < 2 )
    {
        if( (Check_Sum[j] > 47) && Check_Sum[j] < 58 ) Check_Sum[j] = Check_Sum[j] - 48;
        if( Check_Sum[j] > 64 ) Check_Sum[j] = Check_Sum[j] - 55;
        
        CHECKSUM = ( (Check_Sum[0] << 4) + Check_Sum[1] );
    }
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
        if( GPS_Data.gps_itoa == '$' )
        {
            // Save the string to an array
            while( (GPS_Data.gps_itoa != '\n') && (GPS_Data.gps_itoa != '\r') && (GPS_Data.gps_itoa != '*') )
            {
                GPS_I2C_Read();
                NMEASentence[element] = GPS_Data.gps_itoa;
                element++;
            }
            
            if( GPS_Data.gps_itoa == '*' )
            {
                while( (GPS_Data.gps_itoa != '\n') && (GPS_Data.gps_itoa != '\r') )
                {
                    Check_Sum[j] = GPS_Data.gps_itoa;
                    j++;
                }
            }
            // Check the next 5 elements for the NMEA sentence identifier, i.e. 'GPGGA'
            NMEA_ID_Check();
            
            NMEA_Checksum();
            
            endSentence = 1; // End of NMEA sentence
        }
    }
}
/* [] END OF FILE */