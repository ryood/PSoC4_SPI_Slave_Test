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
#define UART_TRACE  (1)

#include <project.h>
#include <stdio.h>
#define RX_PACKET_SIZE      (5)
#define RX_PACKET_HEADER    (0x55)

int main()
{
    uint8 rxBuffer[RX_PACKET_SIZE];
    char strBuffer[100];
    int i;
    uint16 frequency10;
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_Start();
    UART_UartPutString("PSoC 4 SPI Slave Test\r\n");
    
    SPIS_Start();
    PWM_Start();

    for(;;)
    {
        // Check Packet Header
        if (RX_PACKET_SIZE <= SPIS_SpiUartGetRxBufferSize()) {
            rxBuffer[0] = SPIS_SpiUartReadRxData();
            if (RX_PACKET_HEADER != rxBuffer[0]) {
                break;
            }
            for (i = 1; i < RX_PACKET_SIZE; i++) {
                rxBuffer[i] = SPIS_SpiUartReadRxData();
            }
            frequency10 = ((uint16)rxBuffer[3] << 8) | rxBuffer[4];
            PWM_WriteCompare(frequency10);
#if(UART_TRACE)
            sprintf(strBuffer, "%d\t%d\t%d\t%d\t%d\t%d\r\n",
                rxBuffer[0], rxBuffer[1], rxBuffer[2], rxBuffer[3], rxBuffer[4],
                frequency10
            );
            UART_UartPutString(strBuffer);
#endif     
        }
    }
}

/* [] END OF FILE */
