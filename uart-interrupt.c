/*
*
*   uart-interrupt.c
*
*
*
*   @author
*   @date
*/

// The "???" placeholders should be the same as in your uart.c file.
// The "?????" placeholders are new in this file and must be replaced.

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "uart-interrupt.h"
#include "cyBot_Scan.h"

// These variables are declared as examples for your use in the interrupt handler.
volatile char command_byte = -1; // byte value for special character used as a command
volatile int command_flag = 0; // flag to tell the main program a special command was received

void uart_interrupt_init(void){
    //TODO
    //enable clock to GPIO port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //enable clock to UART1
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
    //wait for GPIOB and UART1 peripherals to be ready
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1 ) == 0 ){};
    while ((SYSCTL_PRUART_R & SYSCTL_PRUART_R1 ) == 0 ){};
    //enable alternate functions on port B pins
    GPIO_PORTB_AFSEL_R |= 0x03;
    //enable digital functionality on port B pins
    GPIO_PORTB_DEN_R |= 0xFF;
    //enable UART1 Rx and Tx on port B pins
    GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB1_U1TX;
    GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB0_U1RX;
    //calculate baud rate
    uint16_t iBRD = 0x08; //use equations
    uint16_t fBRD = 0x2C; //use equations
    //turn off UART1 while setting it up
    UART1_CTL_R &= ~UART_CTL_UARTEN;

    //set baud rate
    //note: to take effect, there must be a write to LCRH after these assignments
    UART1_IBRD_R = iBRD;
    UART1_FBRD_R = fBRD;

    //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
    //note: this write to LCRH must be after the BRD assignments
    //UART1_LCRH_R = 0b0110001;
    UART1_LCRH_R |= UART_LCRH_WLEN_8;

    //use system clock as source
    //note from the datasheet UARTCCC register description:
    //field is 0 (system clock) by default on reset
    //Good to be explicit in your code
    UART1_CC_R = 0x0;

    //first clear RX interrupt flag (clear by writing 1 to ICR)
      UART1_ICR_R |= 0b00010000;

      //enable RX raw interrupts in interrupt mask register
      UART1_IM_R |= 0x10;

      //NVIC setup: set priority of UART1 interrupt to 1 in bits 21-23
      NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF) | 0x00200000;

      //NVIC setup: enable interrupt for UART1, IRQ #6, set bit 6
      NVIC_EN0_R |= 0x40;

      //tell CPU to use ISR handler for UART1 (see interrupt.h file)
      //from system header file: #define INT_UART1 22
      IntRegister(INT_UART1, UART1_Handler);

      //globally allow CPU to service interrupts (see interrupt.h file)
      IntMasterEnable();

      //re-enable UART1 and also enable RX, TX (three bits)
      //note from the datasheet UARTCTL register description:
      //RX and TX are enabled by default on reset
      //Good to be explicit in your code
      UART1_CTL_R |= UART_CTL_UARTEN;
    }

void uart_sendChar(char data){
//Blocking call that sends 1 char over UART1
//wait until there is room to send data
while(UART1_FR_R & 0x20)
{
}

//send data
UART1_DR_R = data;

}

char uart_receive(void){

    char data = 0;
    //wait to receive
    while(UART1_FR_R & UART_FR_RXFE)
    {
    }
    //mask the 4 error bits and grab only 8 data bits
    data = (char)(UART1_DR_R & 0xFF);
    return data;
    }

char* uart_receiveStr(){
            buf[20] = '\0';
            idx = 0;

            while(buf[idx] != '\0'){
                buf[idx++] = ' ';
            }

            idx = 0;

            do {
               buf[idx] = uart_receive();

            } while((buf[idx] != '\n') && idx++ < 19);

            //idx--;
            buf[idx] = '\0';
            return buf;
}

void uart_sendStr(const char *data){
    //TODO for reference see lcd_puts from lcd.c file
    do{

        uart_sendChar(*data);

    } while( *(data++) != '\0');
}

unsigned int** map;

// Interrupt handler for receive interrupts
void UART1_Handler(void)
{
    char* byte_received;
    //check if handler called due to RX event
    if (UART1_MIS_R & 0x10)
    {
        //byte was received in the UART data register
        //clear the RX trigger flag (clear by writing 1 to ICR)
        UART1_ICR_R |= 0b00010000;

        //read the byte received from UART1_DR_R and echo it back to PuTTY
        //ignore the error bits in UART1_DR_R
        byte_received = uart_receiveStr();

        if (strstr(byte_received, "go ") == 0){

            char* coord = strtok(byte_received[2], ',');
            int x = atoi(coord[0]);
            int y = atoi(coord[1]);
        }

        //if byte received is a carriage return
        if (strcmp(byte_received, "getMap") == 0)
        {
            //send a newline character back to PuTTY
            uart_sendChar('\n');
            map = getMap();
            int i = 0;
            for( i = 0; i < mapSideLength; i++){
                map[i] = malloc(mapSideLength * sizeof(unsigned int));
                int j = 0;
                for(j = 0; j < mapSideLength; j++){
                    char sendChar;
                    swtich(map[i][j]){
                        case UNDISCOVERED:
                            uart_sendChar('?');
                            break;
                        case EMPTY:
                            uart_sendChar('-');
                            break;
                        case TALL_OBJ:
                            uart_sendChar('O');
                            break;
                        case SHORT_OBJ:
                            uart_sendChar('o');
                            break;
                        case PIT:
                            uart_sendChar('|');
                            break;
                        case ROOM:
                            uart_sendChar('*');
                            break;
                        case ROBOT:
                            uart_sendChar('R');
                            break;
                    }
                }
                uart_sendChar('\r');
                uart_sendChar('\n');
            }
        }
        else
        {
            //AS NEEDED
            //code to handle any other special characters
            //code to update global shared variables
            //DO NOT PUT TIME-CONSUMING CODE IN AN ISR

            if (byte_received == 'g')
            {
              command_flag = 1;
            }
            if (byte_received == 's')
            {
              command_flag = 0;
            }
        }
    }
}
