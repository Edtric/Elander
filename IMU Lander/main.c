/* ========================================
 *
 * Copyright EDTRIC CORP., 2016
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF EDTRIC CORP.
 *
 * ========================================
*/

#include "MPU9250.h"
#include <stdio.h>

int main()
{
    CyGlobalIntEnable;

    I2C_Start();
    UART_DEBUG_Start();
    
    char debug_message[100];
    
    IMU_Start(GYRO_RANGE_250DPS,ACCEL_RANGE_2G);
    Mag_Start(MAG_RANGE_14BITS,MAG_MODE_8HZ);

    for(;;)
    {
        IMU_Read();
        sprintf( debug_message , "GX: %d    GY: %d    GZ: %d\r\nAX: %d    AY: %d    AZ: %d\r\nMX: %d    MY: %d    MZ: %d\r\n" , 
            gyro.x , gyro.y , gyro.z , accel.x , accel.y , accel.z , mag.x , mag.y , mag.z );
        UART_DEBUG_PutString( debug_message );
        CyDelay(50);
    }
}

/* [] END OF FILE */