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
#include "I2C_EZPZ.h"

uint8 readBuf[7];
uint8 gRange , aRange , mRange , mMode;
float aRes , gRes , mRes;
float gyroBias[3] = {0,0,0};
float accelBias[3] = {0,0,0};
float magCal[3] = {0,0,0};
float magBias[3] = {0,0,0};
float STResult[6];

/*******************************************************************************
* Function Name: imu_self_test
********************************************************************************
*
* Summary:
*  Performs a self-test on the IMU and compares test values against non-test
*  values to verify accuracy. Test results are then stored in an array.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

void imu_self_test( void )
{
    uint8 selfTest[6];
    int16 gAvg[3] , aAvg[3] , aSTAvg[3] , gSTAvg[3];
    float factoryTrim[6];
//    uint8 FS = 0;
    
    uint8 avgcnt;
    
    I2C_WriteByte( MPU9250_ADDR , SMPLRT_DIV , 0x00 );
    I2C_WriteByte( MPU9250_ADDR , CONFIG , 0x02 );
    I2C_WriteByte( MPU9250_ADDR , GYRO_CONFIG , 0x00 );
    I2C_WriteByte( MPU9250_ADDR , ACCEL_CONFIG , 0x00 );
    I2C_WriteByte( MPU9250_ADDR , ACCEL_CONFIG2 , 0x02 );
    
    for( avgcnt = 0 ; avgcnt < 200 ; avgcnt++ )
    {
        I2C_ReadBytes( MPU9250_ADDR , GYRO_XOUT_H , 6 , readBuf );
        gAvg[0] += (int16)(((int16)readBuf[0] << 8) | readBuf[1]);
        gAvg[1] += (int16)(((int16)readBuf[2] << 8) | readBuf[3]);
        gAvg[2] += (int16)(((int16)readBuf[4] << 8) | readBuf[5]);
        
        I2C_ReadBytes( MPU9250_ADDR , ACCEL_XOUT_H , 6 , readBuf );
        aAvg[0] += (int16)(((int16)readBuf[0] << 8) | readBuf[1]);
        aAvg[1] += (int16)(((int16)readBuf[2] << 8) | readBuf[3]);
        aAvg[2] += (int16)(((int16)readBuf[4] << 8) | readBuf[5]);
    }
    
    for( avgcnt = 0 ; avgcnt < 3 ; avgcnt++ )
    {
        gAvg[avgcnt] /= 200;
        aAvg[avgcnt] /= 200;
    }
    
    I2C_WriteByte( MPU9250_ADDR , GYRO_CONFIG , 0xE0 );
    I2C_WriteByte( MPU9250_ADDR , ACCEL_CONFIG , 0xE0 );
    
    CyDelay(50);
    
    for( avgcnt = 0 ; avgcnt < 200 ; avgcnt++ )
    {
        I2C_ReadBytes( MPU9250_ADDR , GYRO_XOUT_H , 6 , readBuf );
        gSTAvg[0] += (int16)(((int16)readBuf[0] << 8) | readBuf[1]);
        gSTAvg[1] += (int16)(((int16)readBuf[2] << 8) | readBuf[3]);
        gSTAvg[2] += (int16)(((int16)readBuf[4] << 8) | readBuf[5]);
        
        I2C_ReadBytes( MPU9250_ADDR , ACCEL_XOUT_H , 6 , readBuf );
        aSTAvg[0] += (int16)(((int16)readBuf[0] << 8) | readBuf[1]);
        aSTAvg[1] += (int16)(((int16)readBuf[2] << 8) | readBuf[3]);
        aSTAvg[2] += (int16)(((int16)readBuf[4] << 8) | readBuf[5]);
    }
    
    for( avgcnt = 0 ; avgcnt < 3 ; avgcnt++ )
    {
        gSTAvg[avgcnt] /= 200;
        aSTAvg[avgcnt] /= 200;
    }
    
    I2C_WriteByte( MPU9250_ADDR , GYRO_CONFIG , 0x00 );
    I2C_WriteByte( MPU9250_ADDR , ACCEL_CONFIG , 0x00 );
    
    CyDelay(50);
    
    selfTest[0] = I2C_ReadByte( MPU9250_ADDR , SELF_TEST_X_GYRO );
    selfTest[1] = I2C_ReadByte( MPU9250_ADDR , SELF_TEST_Y_GYRO );
    selfTest[2] = I2C_ReadByte( MPU9250_ADDR , SELF_TEST_Z_GYRO );
    selfTest[3] = I2C_ReadByte( MPU9250_ADDR , SELF_TEST_X_ACCEL );
    selfTest[4] = I2C_ReadByte( MPU9250_ADDR , SELF_TEST_Y_ACCEL );
    selfTest[5] = I2C_ReadByte( MPU9250_ADDR , SELF_TEST_Z_ACCEL );
    
//    factoryTrim[0] = (float)(2620/1<<FS)*(powf( 1.01 , ((float)selfTest[0] - 1.0) ));
//    factoryTrim[1] = (float)(2620/1<<FS)*(powf( 1.01 , ((float)selfTest[1] - 1.0) ));
//    factoryTrim[2] = (float)(2620/1<<FS)*(powf( 1.01 , ((float)selfTest[2] - 1.0) ));
//    factoryTrim[3] = (float)(2620/1<<FS)*(powf( 1.01 , ((float)selfTest[3] - 1.0) ));
//    factoryTrim[4] = (float)(2620/1<<FS)*(powf( 1.01 , ((float)selfTest[4] - 1.0) ));
//    factoryTrim[5] = (float)(2620/1<<FS)*(powf( 1.01 , ((float)selfTest[5] - 1.0) ));
    
    for( avgcnt = 0 ; avgcnt < 3 ; avgcnt++ )
    {
        STResult[avgcnt] = 100.0 * ((float)(gSTAvg[avgcnt] - gAvg[avgcnt]))/factoryTrim[avgcnt];
        STResult[avgcnt + 3] = 100.0 * ((float)(aSTAvg[avgcnt] - aAvg[avgcnt]))/factoryTrim[avgcnt + 3];
    }
}

/*******************************************************************************
* Function Name: imu_calibrate
********************************************************************************
*
* Summary:
*  Collects samples of data from the IMU to calculate the offset needed for each
*  sensor. The calculated offset values are written in the offset registers for
*  their respective sensor.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

void imu_calibrate( void )
{
    uint8 data[12];
//    uint8 gDiv , aDiv;
    uint16 counter , packetCnt , fifoCnt;
    int32 gyro_bias[3] = {0,0,0} , accel_bias[3] = {0,0,0};
    float gyroSens , accelSens;
    
    I2C_WriteByte( MPU9250_ADDR , PWR_MGMT_1 , 0x80 ); //Reset the internal registers and restores the default settings.
    CyDelay(100);
    
    I2C_WriteByte( MPU9250_ADDR , PWR_MGMT_1 , 0x01 ); //Auto selects the best available clock source – PLL if ready, else use the Internal oscillator
    I2C_WriteByte( MPU9250_ADDR , PWR_MGMT_2 , 0x00 ); //Enables accel and gyro
    CyDelay(200);
    
    I2C_WriteByte( MPU9250_ADDR , INT_ENABLE , 0x00 ); //Disable all interrupts
    I2C_WriteByte( MPU9250_ADDR , FIFO_EN , 0x00 ); //Disable FIFO
    I2C_WriteByte( MPU9250_ADDR , PWR_MGMT_1 , 0x00 ); //Turn on internal clock source (20Mhz)
    I2C_WriteByte( MPU9250_ADDR , I2C_MST_CTRL , 0x00 ); //Disables I2C master
    I2C_WriteByte( MPU9250_ADDR , USER_CTRL , 0x00 ); //Disable FIFO access from serial interface and I2C master modes
    I2C_WriteByte( MPU9250_ADDR , USER_CTRL , 0x0C ); //Reset FIFO
    CyDelay(20);
    
    I2C_WriteByte( MPU9250_ADDR , CONFIG , 0x01 ); //Low-pass filter set to 184 Hz
    I2C_WriteByte( MPU9250_ADDR , SMPLRT_DIV , 0x00 ); //Set sample rate to 1 kHz
    I2C_WriteByte( MPU9250_ADDR , GYRO_CONFIG , 0x00 );
    I2C_WriteByte( MPU9250_ADDR , ACCEL_CONFIG , 0x00 );
    
    gyroSens = 131;
    accelSens = 16384;
    
    I2C_WriteByte( MPU9250_ADDR , USER_CTRL , 0x40 ); //Enable FIFO operation mode.
    I2C_WriteByte( MPU9250_ADDR , FIFO_EN , 0x78 ); //Write gyro and accel data at sample rate.
    CyDelay(40);
    
    I2C_WriteByte( MPU9250_ADDR , FIFO_EN , 0x00 ); //Disable FIFO
    I2C_ReadBytes( MPU9250_ADDR , FIFO_COUNTH , 2 , readBuf );
    fifoCnt = ((uint16)readBuf[0] << 8) | readBuf[1];
    packetCnt = fifoCnt/12;
    
    for( counter = 0 ; counter < packetCnt ; counter++ )
    {
        int16 accel_temp[3] = {0,0,0} , gyro_temp[3] = {0,0,0};
        I2C_ReadBytes( MPU9250_ADDR , FIFO_R_W , 12 , data );
        accel_temp[0] = (int16)(((int16)data[0] << 8) | data[1]);
        accel_temp[1] = (int16)(((int16)data[2] << 8) | data[3]);
        accel_temp[2] = (int16)(((int16)data[4] << 8) | data[5]);
        gyro_temp[0] = (int16)(((int16)data[6] << 8) | data[7]);
        gyro_temp[1] = (int16)(((int16)data[8] << 8) | data[9]);
        gyro_temp[2] = (int16)(((int16)data[10] << 8) | data[11]);
        
        accel_bias[0] += (int32)accel_temp[0];
        accel_bias[1] += (int32)accel_temp[1];
        accel_bias[2] += (int32)accel_temp[2];
        gyro_bias[0] += (int32)gyro_temp[0];
        gyro_bias[1] += (int32)gyro_temp[1];
        gyro_bias[2] += (int32)gyro_temp[2];
    }
    
    accel_bias[0] /= (int32)packetCnt;
    accel_bias[1] /= (int32)packetCnt;
    accel_bias[2] /= (int32)packetCnt;
    gyro_bias[0] /= (int32)packetCnt;
    gyro_bias[1] /= (int32)packetCnt;
    gyro_bias[2] /= (int32)packetCnt;
    
    if( accel_bias[2] > 0 ) {accel_bias[2] -= (int32)accelSens;}
    else {accel_bias[2] += (int32)accelSens;}
    
    data[0] = (-gyro_bias[0]/4 >> 8) & 0xFF;
    data[1] = (-gyro_bias[0]/4) & 0xFF;
    data[2] = (-gyro_bias[1]/4 >> 8) & 0xFF;
    data[3] = (-gyro_bias[1]/4) & 0xFF;
    data[4] = (-gyro_bias[2]/4 >> 8) & 0xFF;
    data[5] = (-gyro_bias[2]/4) & 0xFF;
    
    I2C_WriteByte( MPU9250_ADDR , XG_OFFSET_H , data[0] );
    I2C_WriteByte( MPU9250_ADDR , XG_OFFSET_L , data[1] );
    I2C_WriteByte( MPU9250_ADDR , YG_OFFSET_H , data[2] );
    I2C_WriteByte( MPU9250_ADDR , YG_OFFSET_L , data[3] );
    I2C_WriteByte( MPU9250_ADDR , ZG_OFFSET_H , data[4] );
    I2C_WriteByte( MPU9250_ADDR , ZG_OFFSET_L , data[5] );
    
    gyroBias[0] = (float)gyro_bias[0]/(float)gyroSens;
    gyroBias[1] = (float)gyro_bias[1]/(float)gyroSens;
    gyroBias[2] = (float)gyro_bias[2]/(float)gyroSens;
    
    int32 accel_bias_reg[3] = {0,0,0};
    I2C_ReadBytes( MPU9250_ADDR , XA_OFFSET_H , 2 , data );
    accel_bias_reg[0] = (int32)(((int16)data[0] << 8) | data[1]);
    I2C_ReadBytes( MPU9250_ADDR , YA_OFFSET_H , 2 , data );
    accel_bias_reg[1] = (int32)(((int16)data[0] << 8) | data[1]);
    I2C_ReadBytes( MPU9250_ADDR , ZA_OFFSET_H , 2 , data );
    accel_bias_reg[2] = (int32)(((int16)data[0] << 8) | data[1]);
    
    uint32 mask = 1;
    uint8 mask_bit[3] = {0,0,0};
    
    for( counter = 0 ; counter < 3 ; counter++ )
    {
        if( (accel_bias_reg[counter] & mask) ) mask_bit[counter] = 0x01;
    }
    
    accel_bias_reg[0] -= (accel_bias[0]/8);
    accel_bias_reg[1] -= (accel_bias[1]/8);
    accel_bias_reg[2] -= (accel_bias[2]/8);
    
    data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
    data[1] = (accel_bias_reg[0]) & 0xFF;
    data[1] = data[1] | mask_bit[0];
    data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
    data[3] = (accel_bias_reg[1]) & 0xFF;
    data[3] = data[3] | mask_bit[1];
    data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
    data[5] = (accel_bias_reg[2]) & 0xFF;
    data[5] = data[5] | mask_bit[2];
    
    /*
    I2C_WriteByte( MPU9250_ADDR , XA_OFFSET_H , data[0] );
    I2C_WriteByte( MPU9250_ADDR , XA_OFFSET_L , data[1] );
    I2C_WriteByte( MPU9250_ADDR , YA_OFFSET_H , data[2] );
    I2C_WriteByte( MPU9250_ADDR , YA_OFFSET_L , data[3] );
    I2C_WriteByte( MPU9250_ADDR , ZA_OFFSET_H , data[4] );
    I2C_WriteByte( MPU9250_ADDR , ZA_OFFSET_L , data[5] );
    */
    
    accelBias[0] = (float)accel_bias[0]/(float)accelSens;
    accelBias[1] = (float)accel_bias[1]/(float)accelSens;
    accelBias[2] = (float)accel_bias[2]/(float)accelSens;
}

/*******************************************************************************
* Function Name: imu_initialize
********************************************************************************
*
* Summary:
*  Initializes and configures the gyroscope and accelerometer. Also configures
*  the IMU to enable a bypass to the magnetometer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

void imu_initialize( void )
{
    float gyroSens , accelSens;
    
    I2C_WriteByte( MPU9250_ADDR , PWR_MGMT_1 , 0x80 ); //Reset the internal registers and restores the default settings.
    CyDelay(100);
    
//    I2C_WriteByte( MPU9250_ADDR , PWR_MGMT_1 , 0x00 );
//    CyDelay(100);
    
    I2C_WriteByte( MPU9250_ADDR , PWR_MGMT_1 , 0x01 );
    CyDelay(200);
    
    I2C_WriteByte( MPU9250_ADDR , SMPLRT_DIV , 0x07 );//0x04
    
    I2C_WriteByte( MPU9250_ADDR , CONFIG , 0x03 );
    
    switch (gRange)
    {
        case GYRO_RANGE_250DPS:
            I2C_WriteByte( MPU9250_ADDR , GYRO_CONFIG , 0x00 );
            gyroSens = 131;
            gRes = GYRO_RES_250DPS;
            break;
        case GYRO_RANGE_500DPS:
            I2C_WriteByte( MPU9250_ADDR , GYRO_CONFIG , 0x08 );
            gyroSens = 65.5;
            gRes = GYRO_RES_500DPS;
            break;
        case GYRO_RANGE_1000DPS:
            I2C_WriteByte( MPU9250_ADDR , GYRO_CONFIG , 0x10 );
            gyroSens = 32.8;
            gRes = GYRO_RES_1000DPS;
            break;
        case GYRO_RANGE_2000DPS:
            I2C_WriteByte( MPU9250_ADDR , GYRO_CONFIG , 0x18 );
            gyroSens = 16.4;
            gRes = GYRO_RES_2000DPS;
            break;
    }
    
    switch (aRange)
    {
        case ACCEL_RANGE_2G:
            I2C_WriteByte( MPU9250_ADDR , ACCEL_CONFIG , 0x00 );
            accelSens = 16384;
            aRes = ACCEL_RES_2G;
            break;
        case ACCEL_RANGE_4G:
            I2C_WriteByte( MPU9250_ADDR , ACCEL_CONFIG , 0x08 );
            accelSens = 8192;
            aRes = ACCEL_RES_4G;
            break;
        case ACCEL_RANGE_8G:
            I2C_WriteByte( MPU9250_ADDR , ACCEL_CONFIG , 0x10 );
            accelSens = 4096;
            aRes = ACCEL_RES_8G;
            break;
        case ACCEL_RANGE_16G:
            I2C_WriteByte( MPU9250_ADDR , ACCEL_CONFIG , 0x18 );
            accelSens = 2048;
            aRes = ACCEL_RES_16G;
            break;
    }    
    
    I2C_WriteByte( MPU9250_ADDR , ACCEL_CONFIG2 , 0x03 );
    
    I2C_WriteByte( MPU9250_ADDR , INT_PIN_CFG , 0x02 );
//    I2C_WriteByte( MPU9250_ADDR , INT_ENABLE , 0x01 );
    CyDelay(100);
}

/*******************************************************************************
* Function Name: mag_initialize
********************************************************************************
*
* Summary:
*  Initializes and configures the magnetometer. Also calculates the offset value.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

void mag_initialize( void )
{
    uint8 rawData[3];
    
    switch (mRange)
    {
        case MAG_RANGE_14BITS:
            mRes = MAG_RES_14BITS;
            break;
        case MAG_RANGE_16BITS:
            mRes = MAG_RES_16BITS;
            break;
    }
    
    I2C_WriteByte( AK8963_ADDR , AK8963_CNTL , 0x00 ); //Power down mag
    CyDelay(10);
    I2C_WriteByte( AK8963_ADDR , AK8963_CNTL , 0x0F ); //Enter Fuse ROM access mode
    CyDelay(10);
    
    I2C_ReadBytes( AK8963_ADDR , AK8963_ASAX , 3 , rawData );
    magCal[0] = (float)(rawData[0] - 128) / 256.0 + 1.0;
    magCal[1] = (float)(rawData[1] - 128) / 256.0 + 1.0;
    magCal[2] = (float)(rawData[2] - 128) / 256.0 + 1.0;
    
    I2C_WriteByte( AK8963_ADDR , AK8963_CNTL , 0x00 ); //Power down mag
    CyDelay(10);
    I2C_WriteByte( AK8963_ADDR , AK8963_CNTL , (mRange << 4) | mMode );
    CyDelay(10);
}

/*******************************************************************************
* Function Name: IMU_Configure
********************************************************************************
*
* Summary:
*  Sets the range for the gyroscope and the accelerometer, and calls the
*  imu_initialize and imu_calibrate functions to perform an initial setup.
*
* Parameters:
*  gRng: Range of gyroscope.
*  aRng: Range of accelerometer.
*
* Return:
*  None.
*
*******************************************************************************/

void IMU_Start( gyroRange_t gRng , accelRange_t aRng )
{
    gRange = gRng;
    aRange = aRng;
    
//    imu_self_test();
    imu_initialize();

//    imu_calibrate();
}

/*******************************************************************************
* Function Name: Mag_Configure
********************************************************************************
*
* Summary:
*  Sets the range and the mode for the magnetometer, and calls the mag_initialize
*  function to perform and initial setup.
*
* Parameters:
*  mRng: Range of magnetometer.
*  mMde: Mode of magnetometer.
*
* Return:
*  None.
*
*******************************************************************************/

void Mag_Start( magRange_t mRng , magMode_t mMde )
{
    mRange = mRng;
    mMode = mMde;
    
    mag_initialize();
}

/*******************************************************************************
* Function Name: IMU_Read
********************************************************************************
*
* Summary:
*  Reads the IMU data from the sensor registers and stores them in the sensor
*  structures.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

void IMU_Read( void )
{

    if( I2C_ReadByte( MPU9250_ADDR , INT_STATUS ) & 0x01 )
    {
        I2C_ReadBytes( MPU9250_ADDR , GYRO_XOUT_H , 6 , readBuf );
        gyro.x = (int16)((readBuf[0] << 8) | readBuf[1]);
        gyro.y = (int16)((readBuf[2] << 8) | readBuf[3]);
        gyro.z = (int16)((readBuf[4] << 8) | readBuf[5]);
        
        I2C_ReadBytes( MPU9250_ADDR , ACCEL_XOUT_H , 6 , readBuf );
        accel.x = (int16)((readBuf[0] << 8) | readBuf[1]);
        accel.y = (int16)((readBuf[2] << 8) | readBuf[3]);
        accel.z = (int16)((readBuf[4] << 8) | readBuf[5]);
        
//        gyro.x *= gRes;
//        gyro.y *= gRes;
//        gyro.z *= gRes;
//        
//        accel.x *= aRes;
//        accel.y *= aRes;
//        accel.z *= aRes;
    }
    
    if( I2C_ReadByte( AK8963_ADDR , AK8963_ST1 ) & 0x01 )
    {
        I2C_ReadBytes( AK8963_ADDR , AK8963_XOUT_L , 7 , readBuf );
        uint8 c = readBuf[6];
        if( !(c & 0x08) )
        {
            mag.x = (int16)((readBuf[1] << 8) | readBuf[0]);
            mag.y = (int16)((readBuf[3] << 8) | readBuf[2]);
            mag.z = (int16)((readBuf[5] << 8) | readBuf[4]);
            
//            mag.x *= mRes;
//            mag.y *= mRes;
//            mag.z *= mRes;
        }
    }
}

/* [] END OF FILE */
