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

#ifndef MPU9250_H
#define MPU9250_H
    
    #include <project.h>
    
    #define MPU9250_ADDR        0x68	 /**< IMU device address */
    #define AK8963_ADDR         0x0C	 /**< Magnetometer device address */
    
    #define GYRO_RES_250DPS     0.00763F /**< Gyroscope data resolution - 250 degrees/sec */
    #define GYRO_RES_500DPS     0.01526F /**< Gyroscope data resolution - 500 degrees/sec */
    #define GYRO_RES_1000DPS    0.03052F /**< Gyroscope data resolution - 1000 degrees/sec */
    #define GYRO_RES_2000DPS    0.06104F /**< Gyroscope data resolution - 2000 degrees/sec */
    
    #define ACCEL_RES_2G        0.00006F /**< Accelerometer data resolution - 2 G */
    #define ACCEL_RES_4G        0.00012F /**< Accelerometer data resolution - 4 G */
    #define ACCEL_RES_8G        0.00024F /**< Accelerometer data resolution - 8 G */
    #define ACCEL_RES_16G       0.00049F /**< Accelerometer data resolution - 16 G */
    
    #define MAG_RES_14BITS      5.99756F /**< Magnetometer data resolution - 14 bits */
    #define MAG_RES_16BITS      1.49939F /**< Magnetometer data resolution - 16 bits */
    
    #define WHO_AM_I_AK8963     0x00  /**< Should return 0x48 */
    #define AK8963_ST1          0x02  /**< Data ready status bit 0 */
    #define AK8963_XOUT_L	    0x03  /**< Magnetometer x-axis low-byte */
    #define AK8963_XOUT_H	    0x04  /**< Magnetometer x-axis hi-byte */
    #define AK8963_YOUT_L	    0x05  /**< Magnetometer y-axis low-byte */
    #define AK8963_YOUT_H	    0x06  /**< Magnetometer y-axis hi-byte */
    #define AK8963_ZOUT_L	    0x07  /**< Magnetometer z-axis low-byte */
    #define AK8963_ZOUT_H	    0x08  /**< Magnetometer z-axis hi-byte */
    #define AK8963_ST2          0x09  /**< Data overflow bit 3 and data read error status bit 2 */
    #define AK8963_CNTL         0x0A  /**< Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0 */
    #define AK8963_ASTC         0x0C  /**< Self test control */
    #define AK8963_I2CDIS       0x0F  /**< I2C disable */
    #define AK8963_ASAX         0x10  /**< Fuse ROM x-axis sensitivity adjustment value */
    #define AK8963_ASAY         0x11  /**< Fuse ROM y-axis sensitivity adjustment value */
    #define AK8963_ASAZ         0x12  /**< Fuse ROM y-axis sensitivity adjustment value */
    
    #define SELF_TEST_X_GYRO    0x00  /**< Gyroscope x-axis self-test value */
    #define SELF_TEST_Y_GYRO    0x01  /**< Gyroscope y-axis self-test value */
    #define SELF_TEST_Z_GYRO    0x02  /**< Gyroscope z-axis self-test value */
    #define SELF_TEST_X_ACCEL   0x0D  /**< Accelerometer x-axis self-test value */
    #define SELF_TEST_Y_ACCEL   0x0E  /**< Accelerometer y-axis self-test value */
    #define SELF_TEST_Z_ACCEL   0x0F  /**< Accelerometer z-axis self-test value */
    
    #define XG_OFFSET_H         0x13  /**< Gyroscope x-axis trim value hi-byte */
    #define XG_OFFSET_L         0x14  /**< Gyroscope x-axis trim value low-byte */
    #define YG_OFFSET_H         0x15  /**< Gyroscope y-axis trim value hi-byte */
    #define YG_OFFSET_L         0x16  /**< Gyroscope y-axis trim value low-byte */
    #define ZG_OFFSET_H         0x17  /**< Gyroscope z-axis trim value hi-byte */
    #define ZG_OFFSET_L         0x18  /**< Gyroscope z-axis trim value low-byte */
    #define SMPLRT_DIV          0x19  /**< Divides the internal sample rate (see register CONFIG) */
    #define CONFIG              0x1A  /**< FIFO mode bit 6, FSYNC pin data sample enable bit 5-3, DLPF config bit 2-0 */
    #define GYRO_CONFIG         0x1B  /**< Gyroscope configuration */
    #define ACCEL_CONFIG        0x1C  /**< Acceleration configuration */
    #define ACCEL_CONFIG2       0x1D  /**< Acceleration configuration (FCHOICE and DLPF config) */
    #define LP_ACCEL_ODR        0x1E  /**< Low Power Accelerometer ODR Control */ 
    #define WOM_THR             0x1F  /**< Wake-on Motion Threshold */
    
    #define FIFO_EN             0x23  /**< FIFO enable */
    #define I2C_MST_CTRL        0x24  /**< I2C master control register */
    #define INT_PIN_CFG         0x37  /**< INT Pin / Bypass Enable Configuration */
    #define INT_ENABLE          0x38  /**< Interrupt Enable */
    #define INT_STATUS          0x3A  /**< Interrupt Status */
    #define ACCEL_XOUT_H        0x3B  /**< Accelerometer x-axis hi-byte */
    #define ACCEL_XOUT_L        0x3C  /**< Accelerometer x-axis low-byte */
    #define ACCEL_YOUT_H        0x3D  /**< Accelerometer y-axis hi-byte */
    #define ACCEL_YOUT_L        0x3E  /**< Accelerometer y-axis low-byte */
    #define ACCEL_ZOUT_H        0x3F  /**< Accelerometer z-axis hi-byte */
    #define ACCEL_ZOUT_L        0x40  /**< Accelerometer z-axis low-byte */
    #define TEMP_OUT_H          0x41  /**< Temperature data hi-byte */
    #define TEMP_OUT_L          0x42  /**< Temperature data low-byte */
    #define GYRO_XOUT_H         0x43  /**< Gyroscope x-axis hi-byte */
    #define GYRO_XOUT_L         0x44  /**< Gyroscope x-axis low-byte */
    #define GYRO_YOUT_H         0x45  /**< Gyroscope y-axis hi-byte */
    #define GYRO_YOUT_L         0x46  /**< Gyroscope y-axis low-byte */
    #define GYRO_ZOUT_H         0x47  /**< Gyroscope z-axis hi-byte */
    #define GYRO_ZOUT_L         0x48  /**< Gyroscope z-axis low-byte */
    #define USER_CTRL           0x6A  /**< Bit 7 enable DMP, bit 3 reset DMP */
    #define PWR_MGMT_1          0x6B  /**< Device defaults to the SLEEP mode */
    #define PWR_MGMT_2          0x6C  /**< Enables accel and gyro (on by default) */
    #define FIFO_COUNTH         0x72  /**< FIFO count hi-byte */
    #define FIFO_COUNTL         0x73  /**< FIFO count low-byte */
    #define FIFO_R_W            0x74  /**< FIFO read/write */
    #define WHO_AM_I_MPU9250    0x75  /**< Should return 0x71 */
    #define XA_OFFSET_H         0x77  /**< Accelerometer x-axis trim value hi-byte */
    #define XA_OFFSET_L         0x78  /**< Accelerometer x-axis trim value low-byte */
    #define YA_OFFSET_H         0x7A  /**< Accelerometer y-axis trim value hi-byte */
    #define YA_OFFSET_L         0x7B  /**< Accelerometer y-axis trim value low-byte */
    #define ZA_OFFSET_H         0x7D  /**< Accelerometer z-axis trim value hi-byte */
    #define ZA_OFFSET_L         0x7E  /**< Accelerometer z-axis trim value low-byte */
    
    typedef enum
    {
        GYRO_RANGE_250DPS, /**< 250 degrees/sec for gyroscope configuration */
        GYRO_RANGE_500DPS, /**< 500 degrees/sec for gyroscope configuration */
        GYRO_RANGE_1000DPS, /**< 1000 degrees/sec</B> for gyroscope configuration */
        GYRO_RANGE_2000DPS /**< 2000 degrees/sec</B> for gyroscope configuration */
    }gyroRange_t;
    
    typedef enum
    {
        ACCEL_RANGE_2G, /**< +/- 2G for accelerometer configuration */
        ACCEL_RANGE_4G, /**< +/- 4G for accelerometer configuration */
        ACCEL_RANGE_8G, /**< +/- 8G for accelerometer configuration */
        ACCEL_RANGE_16G /**< +/- 16G for accelerometer configuration */
    }accelRange_t;
    
    typedef enum
    {
        MAG_RANGE_14BITS, /**< 14-bit resolution for magnetometer configuration */
        MAG_RANGE_16BITS /**< 16-bit resolution for magnetometer configuration */
    }magRange_t;
    
    typedef enum
    {
        MAG_MODE_8HZ    = 0x02, /**< 8 Hz sample frequency for magnetometer configuration */
        MAG_MODE_100HZ  = 0x06 /**< 100 Hz sample frequency for magnetometer configuration */
    }magMode_t;        
    
	
	/**
    * Structure to store read data from sensors.
    */
    typedef struct MPU9250_Data
    {
        int x;
        int y;
        int z;
    }IMU_Data;
    
    IMU_Data gyro;
    IMU_Data accel;
    IMU_Data mag;

    void IMU_Start( gyroRange_t gRng , accelRange_t aRng );
    void Mag_Start( magRange_t mRng , magMode_t mMde );  
    void IMU_Read( void );
    
#endif

/* [] END OF FILE */
