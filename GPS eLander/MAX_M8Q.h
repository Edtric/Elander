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

#ifndef MAX_M8Q_H
#define MAX_M8Q_H
    
    #include <project.h>
    #include <stdlib.h>
    #include <string.h>
    #include "I2C_EZPZ.h"

    #define MAX_M8Q_ADDRESS     0x42
    #define NUM_BYTES_HI        0xFD
    #define NUM_BYTES_LO        0xFE
    #define DATA_STREAM_REG     0xFF

    #define GPS_SUBFRAME_SIZE   10
    
    uint8 rStat;
    uint8 wStat;
    uint8 reg[2];
    uint8 *size;
    uint32 GPS_Subframe[10];
    uint32 GLONASS_Subframe[4];
    
    CY_ISR ( RX_Int_Handle );
    void GPS_I2C_Output( void );
    void GPS_UART_Output( void );
    void GPS_I2C_Read( void );
    void GPS_UART_Read( void );
    void MAX_M8Q_Get_Byte_Size( void );
    
    typedef struct MAX_M8Q_Data
    {
        uint8 gps_stream;
        char ASCII;
    } GPS;
    
    typedef struct NMEA_Data
    {
        float time;
        int lat;
        char *dir_lat;
        int lon;
        char *dir_lon;
        uint8 quality;
        uint8 num_sat;
        float HDOP;
        float altitude;
        float geoidal_sep;
        float data_age;
        uint8 ref_ID;
    } NMEAData;
    
    GPS GPS_Data;
    NMEAData FixData;
    
#endif

/* [] END OF FILE */
