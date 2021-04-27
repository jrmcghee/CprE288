/*
 * scan.c
 *
 *  Created on: Apr 21, 2021
 *      Author: edbryant
 */

#include "scan.h"
#include "Timer.h"
#include "math.h"
//PING Sensor
volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile unsigned long OVERFLOW = 0;
volatile enum{
    LOW, HIGH, DONE
} STATE = LOW;

void ping_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x2;

    while ((SYSCTL_PRGPIO_R & 0x02) != 0x02){};

    GPIO_PORTB_DEN_R |= 0x8;

    GPIO_PORTB_AFSEL_R |= 0x8;

    GPIO_PORTB_PCTL_R |= 0x7000;

    GPIO_PORTB_DIR_R |= 0x8;

    SYSCTL_RCGCTIMER_R |= 0x8;

    while ((SYSCTL_PRTIMER_R & 0x8) != 0x8){};

    TIMER3_CTL_R &= ~0x100;

    TIMER3_CTL_R |= 0xC00;

    TIMER3_CFG_R = 0x4;

    TIMER3_TBMR_R = 0b00111;

    TIMER3_TBILR_R = 0xFFFF;

    TIMER3_TBPR_R = 0xFF;

    TIMER3_ICR_R |= 0x400;

    TIMER3_IMR_R |= 0x400;

    NVIC_PRI9_R |= 0b00100000;

    NVIC_EN1_R |= 0x10;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    TIMER3_CTL_R |= 0x100;

    STATE = LOW;

}



void ping_trigger(void){

    STATE = LOW;

    TIMER3_CTL_R &= ~0x100;

    TIMER3_IMR_R &= ~0x400;

    GPIO_PORTB_AFSEL_R &= ~0x8;

    GPIO_PORTB_DATA_R &= ~0x8;

    GPIO_PORTB_DATA_R |= 0x8;

    timer_waitMicros(5);

    GPIO_PORTB_DATA_R &= ~0x8;

    TIMER3_ICR_R |= 0x400;

    GPIO_PORTB_AFSEL_R |= 0x08;

    TIMER3_IMR_R |= 0x400;

    TIMER3_CTL_R |= 0x100;
}



void TIMER3B_Handler(void){

    if (STATE == LOW){

        STATE = HIGH;

        START_TIME = TIMER3_TBR_R;
    }

    else if (STATE == HIGH){

        END_TIME = TIMER3_TBR_R;

        STATE = DONE;
    }

    TIMER3_ICR_R |= 0x400;
}

float ping_getDistance(void){

    float distance = 0;

    ping_trigger();

    while (STATE != DONE){}

    if (START_TIME < END_TIME){

        OVERFLOW++;

        return OVERFLOW;
    }

    distance = (START_TIME - END_TIME);

    distance = distance * 34.3 / 32000;

    return distance;
}

//IR Sensor

void adc_init(void){

      SYSCTL_RCGCADC_R |= 0x0001;

      SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

      while((SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R1) == 0){};

      GPIO_PORTB_DIR_R &= ~0x10;

      GPIO_PORTB_AFSEL_R |= 0x10;

      GPIO_PORTB_DEN_R &= ~0x10;

      GPIO_PORTB_AMSEL_R |= 0x10;

      ADC0_ACTSS_R &= ~0x0008;

      ADC0_EMUX_R &= ~0xF000;

      ADC0_SSMUX3_R &= ~0x000F;

      ADC0_SSMUX3_R += 10;

      ADC0_SSCTL3_R = 0x0006;

      ADC0_IM_R &= ~0x0008;

      ADC0_ACTSS_R |= 0x0008;
}

int adc_read(){

    ADC0_PSSI_R = 0x0008;

    while((ADC0_RIS_R&0x08)==0){};

    unsigned long data = ADC0_SSFIFO3_R&0xFFF;

    ADC0_ISC_R = 0x0008;

    return data;
}

double adc_distance(int ir_sensor){

    double result = ir_sensor;

    result = pow(result, -1.176);

    result *= 126106;

    return result;
}

//PWM Motor

void servo_init(){

    SYSCTL_RCGCGPIO_R |= 0x2;     //enable port b clock

    while ((SYSCTL_RCGCGPIO_R & 0x02) == 0x0){};

    GPIO_PORTB_AFSEL_R |= 0x20;   //sets alt function on pin 5

    GPIO_PORTB_PCTL_R |= 0x700000;   //enables GPIO pin 5

    GPIO_PORTB_DIR_R |= 0x20;     //enables output on pin 5

    GPIO_PORTB_DEN_R |= 0x20;     //enables pin 5 as digital

    SYSCTL_RCGCTIMER_R |= 0x2; //starts clock

    TIMER1_CTL_R &= 0xFEFF; //disables clock

    while ((SYSCTL_PRTIMER_R & 0x2) == 0x0){};

    TIMER1_CFG_R |= 0x4;    //configures 16 bit register

    TIMER1_TBMR_R |= 0xA; //changes mode to periodic

    TIMER1_TBPR_R = 0x4;  //timer overflow

    TIMER1_TBILR_R = 0xE200;   //16 bit timer

    TIMER1_TBPMR_R = 0x4; //minimum match value for the time

    TIMER1_TBMATCHR_R = 0xE200; //maximum match value for the timer

    TIMER1_CTL_R |= 0x0100; //enables the timer
}



int servo_move(float degree){

    int calculation = 0xE200 - (90 * (1.6 * degree + 210));

    TIMER1_TBPMR_R = (calculation >> 16) & 0xFF;

    TIMER1_TBMATCHR_R = calculation & 0xFFFF;

    return degree;
}
