/* ========================================
 *
 * SPI Slaveの送受信テスト
 * 
 * 2016.05.09
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>

#define RX_PACKET_SIZE  3
#define TX_PACKET_SIZE  3

int main()
{
    int i;
    uint8 rxBuffer[RX_PACKET_SIZE];
    uint8 txBuffer[TX_PACKET_SIZE];
    
    char strBuffer[80];
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_Start();
    UART_UartPutString("SPI Slave test.\r\n");

    SPIS_Start();

    for(;;)
    {
        // RX
        while (RX_PACKET_SIZE > SPIS_SpiUartGetRxBufferSize())
        {
        }
        for (i = 0; i < RX_PACKET_SIZE; i++) {
            rxBuffer[i] = SPIS_SpiUartReadRxData();
        }

        // TX
        memcpy(txBuffer, rxBuffer, TX_PACKET_SIZE);
        SPIS_SpiUartPutArray(txBuffer, TX_PACKET_SIZE);
        
        //sprintf(strBuffer, "RX:\t%d\t%d\t%d\r\n", rxBuffer[0], rxBuffer[1], rxBuffer[2]);
        sprintf(strBuffer, "%d\t%d\t%d\r\n", rxBuffer[0], rxBuffer[1], rxBuffer[2]);
        UART_UartPutString(strBuffer);      
        
        //CyDelay(1);
    }
}

/* [] END OF FILE */
