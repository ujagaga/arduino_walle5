#ifndef UART_CMD_H
#define UART_CMD_H

/* Uart commands. Receiving command is expected to start with UCMD_START_0, UCMD_START_1 and then 
*  the third byte can be any value. Some predefined commands are below.
*  Any routinre sending data back should specify the command to which it is responding and then 
*  send the data.
*/

#define UCMD_START_0   0xfe
#define UCMD_START_1   0xff

#define UCMD_FAIL             -1    
#define UCMD_NONE             0
#define UCMD_CAM_SNAPSHOT     1
#define UCMD_FLASH_ON         2
#define UCMD_FLASH_OFF        3
#define UCMD_IR_ON            4
#define UCMD_IR_OFF           5
#define UCMD_TV               6
#define UCMD_GET_RANGE_FRONT  7
#define UCMD_GET_RANGE_BACK   8
#define UCMD_GET_ROTATION     9
#define UCMD_PRINT_TEXT       10
#define UCMD_PRINT_GRAPHIC    11

uint8_t UART_checkCmd(void);
void UART_sendResponse(int cmd, uint8_t* buf, uint32_t len);
void UART_process(void);
bool UART_copyMgs(uint8_t* outBuf, uint16_t len);
uint16_t UART_getRxLen(void);

#endif