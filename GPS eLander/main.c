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
#include <project.h>
#include "MAX_M8Q.h"
#include <stdio.h>

char testString[30];
char testSection1[10];
char testSection2[10];
char testSection3[10];
char testSection4[10];
char testSection5[10];

int main()
{
    CyGlobalIntEnable;
    
    I2C_Start();
    UART_DEBUG_Start();
    CyDelay( 100 );
    
    UART_DEBUG_PutString( "Starting GPS Stream\r\n" );
    
//    RX_ISR_StartEx( RX_Int_Handle );
    
    I2C_MasterClearReadBuf();
    
    for(;;)
    {
//        GPS_UART_Gather_Data();
//        GPS_I2C_Gather_Data();
        
        GPS_I2C_Read();
        if( I2C_MasterGetReadBufSize() > 0 )
        {
            UART_DEBUG_PutChar( GPS_Data.ASCII );
        }
        I2C_MasterClearReadBuf();

        
//        sprintf( testString , "GNGGA,,,,,,0,00,99.99,,,,,,*56" );
//        sprintf( testString , "1,2,3,,5" );
//        sscanf( testString , "%[^','],%[^','],%[^','],%[^','],%[^',']" , testSection1 , testSection2 , testSection3 , testSection4 , testSection5 );
//        UART_DEBUG_PutString( testSection1 );
//        UART_DEBUG_PutChar('\n');
//        UART_DEBUG_PutString( testSection2 );
//        UART_DEBUG_PutChar('\n');
//        UART_DEBUG_PutString( testSection3 );
//        UART_DEBUG_PutChar('\n');
//        UART_DEBUG_PutString( testSection4 );
//        UART_DEBUG_PutChar('\n');
//        UART_DEBUG_PutString( testSection5 );
//        UART_DEBUG_PutString("\n\r");
//        CyDelay( 100 );
        
        
//        sprintf( output , "Fix quality: %d     Number of satellites: %d\n\r" , FixData.quality , FixData.num_sat );
//        sprintf( output , "Position: %f %s , %f %s \n\r" , FixData.lat , FixData.dir_lat , FixData.lon , FixData.dir_lon );
//        UART_DEBUG_PutChar( GPS_Data.ASCII );
//        UART_DEBUG_PutChar( UART_DEBUG_GetChar() );
    }
}

/* [] END OF FILE */
