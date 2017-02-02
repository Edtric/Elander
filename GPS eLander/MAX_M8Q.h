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
    #include <stdio.h>
    #include <stdbool.h>
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
    
//    CY_ISR ( RX_Int_Handle );
    void GPS_I2C_Output( void );
    void GPS_UART_Output( void );
    void GPS_I2C_Read( void );
    void GPS_I2C_Gather_Data( void );
    void GPS_UART_Gather_Data( void );
    
    typedef struct MAX_M8Q_Data
    {
        uint8 gps_stream;
        char ASCII;
    } GPS;
    
    typedef struct NMEA_Data
    {
        uint8 time_hr;
        uint8 time_min;
        float time_sec;
        float lat;
        float lon;
        uint8 quality;
        uint8 num_sat;
        float HDOP;
        float altitude;
        float geoidal_sep;
        float data_age;
        uint8 ref_ID;
    } NMEAData;
    
    typedef struct NMEA_String
    {
        char sentenceID[6];
        char time[10];
        char lat[11];
        char dir_lat[3];
        char lon[12];
        char dir_lon[3];
        char quality[3];
        char num_sat[3];
        char HDOP[5];
        char altitude[10];
        char geoidal_sep[5];
        char data_age[5];
        char ref_ID[5];
        char M1[3];
        char M2[3];
    } NMEAString;
    
    typedef struct Time_String
    {
        char hours[3];
        char minutes[3];
        char seconds[6];
    } TimeString;
    
    GPS GPS_Data;
    NMEAData FixData;
    NMEAString FixString;
    TimeString StringTime;
    
#endif

/* [] END OF FILE */
