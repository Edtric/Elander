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

#include "I2C_EZPZ.h"

/*******************************************************************************
* Function Name: I2C_Write2Reg
********************************************************************************
*
* Summary:
*  Writes a single byte to the slave device (slave register).
*
* Parameters:
*  address: 7-bit address of the slave device.
*  reg:     8-bit register to be written to.
*
* Return:
*  None.
*
*******************************************************************************/

void I2C_Write2Reg( uint8 address , uint8 reg )
{
    I2C_MasterSendStart( address , WRITE );
    
    I2C_MasterWriteByte( reg );
    
    I2C_MasterSendStop();
}

/*******************************************************************************
* Function Name: I2C_WriteByte
********************************************************************************
*
* Summary:
*  Writes two bytes to the slave device.
*
* Parameters:
*  address: 7-bit address of the slave device.
*  reg:     8-bit register to be written to.
*  data:    Byte to be written to register.
*
* Return:
*  None.
*
*******************************************************************************/

void I2C_WriteByte( uint8 address , uint8 reg , uint8 data )
{
    I2C_MasterSendStart( address , WRITE );
    
    I2C_MasterWriteByte( reg );
    
    I2C_MasterWriteByte( data );
    
    I2C_MasterSendStop();
}

/*******************************************************************************
* Function Name: I2C_ReadByteNW
********************************************************************************
*
* Summary:
*  Reads a single byte from slave device. This function should be used if writing 
*  to the device first is not necessary.
*
* Parameters:
*  address: 7-bit address of the slave device.
*
* Return:
*  value:   Single byte value that was read from the slave device.
*
*******************************************************************************/

uint8 I2C_ReadByteNW( uint8 address )
{
    uint8 value;
    
    I2C_MasterSendStart( address , READ );
    
    value = I2C_MasterReadByte( I2C_NAK_DATA );
    
    I2C_MasterSendStop();
    
    return value;
}

/*******************************************************************************
* Function Name: I2C_ReadBytesNW
********************************************************************************
*
* Summary:
*  Reads multiple bytes from slave device. This function should be used if writing 
*  to the device first is not necessary.
*
* Parameters:
*  address:     7-bit address of the slave device.
*  bytesToRead: Number of bytes to be read from the slave address.
*  data:        Data buffer to store received bytes.
*
* Return:
*  None.
*
*******************************************************************************/

void I2C_ReadBytesNW( uint8 address , uint8 bytesToRead , uint8 * data )
{
    uint8 counter;
    
    I2C_MasterSendStart( address , READ );
    
    for( counter = 0 ; counter < bytesToRead ; counter++ )
    {
        if( counter == (bytesToRead - 1) )
        {
            data[counter] = I2C_MasterReadByte( I2C_NAK_DATA );
        }
        else
        {
            data[counter] = I2C_MasterReadByte( I2C_ACK_DATA );
        }
    }
    
    I2C_MasterSendStop();
}

/*******************************************************************************
* Function Name: I2C_ReadByte
********************************************************************************
*
* Summary:
*  Reads a single byte from slave device.
*
* Parameters:
*  address: 7-bit address of the slave device.
*  reg:     8-bit register to be written to.
*
* Return:
*  value:   Single byte value that was read from the slave device.
*
*******************************************************************************/

uint8 I2C_ReadByte( uint8 address , uint8 reg )
{
    uint8 value;
    
    I2C_MasterSendStart( address , WRITE );
    
    I2C_MasterWriteByte( reg );
    
    I2C_MasterSendRestart( address , READ );
    
    value = I2C_MasterReadByte( I2C_NAK_DATA );
    
    I2C_MasterSendStop();
    
    return value;
}

/*******************************************************************************
* Function Name: I2C_ReadBytes
********************************************************************************
*
* Summary:
*  Reads multiple bytes from slave device.
*
* Parameters:
*  address:     7-bit address of the slave device.
*  reg:         8-bit register to be written to.
*  bytesToRead: Number of bytes to be read from the slave address.
*  data:        Data buffer to store received bytes.
*
* Return:
*  None.
*
*******************************************************************************/

void I2C_ReadBytes( uint8 address , uint8 reg , uint8 bytesToRead , uint8 * data )
{
    uint8 counter;
    
    I2C_MasterSendStart( address , WRITE );
    
    I2C_MasterWriteByte( reg );
    
    I2C_MasterSendRestart( address , READ );
    
    for( counter = 0 ; counter < bytesToRead ; counter++ )
    {
        if( counter == (bytesToRead - 1) )
        {
            data[counter] = I2C_MasterReadByte( I2C_NAK_DATA );
        }
        else
        {
            data[counter] = I2C_MasterReadByte( I2C_ACK_DATA );
        }
    }
    
    I2C_MasterSendStop();
}

/* [] END OF FILE */
