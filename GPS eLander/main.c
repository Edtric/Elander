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

int main()
{
    CyGlobalIntEnable;
    
    char debug_message[100];
    
    I2C_Start();
    UART_DEBUG_Start();
    CyDelay( 100 );
    
    UART_DEBUG_PutString( "Starting GPS Stream\r\n" );
    
    for(;;)
    {
        GPS_I2C_Read();
        
        UART_DEBUG_PutChar( GPS_Data.gps_itoa );
        
        CyDelay( 10 );
    }
}

/* [] END OF FILE */
