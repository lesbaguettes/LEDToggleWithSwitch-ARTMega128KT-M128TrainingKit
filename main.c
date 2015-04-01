//
// MicroProcessor Applicate
//
// main.c
// LEDToggleWithSwitch
// 
// 2015. 4. 1.. write by Choe Wantaek
// with ARTMega 128 KT-M128 Training Kit (No.8)
//

#include <avr/io.h>
#include <avr/interrupt.h>

#define EX_LED	(*(volatile unsigned int *)0x8008)
#define	ON		1
#define OFF		0

volatile unsigned char eledValue;    // EX_LED 제어 전역변수
volatile unsigned char sw[8];

void hw_init (void);
void eled_control(unsigned char, unsigned);
void eled_toggle(unsigned char);
void delay(unsigned short td);

int main(void) {
	unsigned char key;
	
	hw_init ();

	while(1) {
		eledValue	^= PINB;
		EX_LED		= eledValue;
		delay(500);
	}

	return 0;
}

void hw_init (void) {
	eledValue   = 0;    // EX_LED를 제어하기 위한 전역변수 eledValue를 0으로 초기화한다.
	DDRB		= 0x00;	// PORTB를 입력(0)으로 만든다.
	PORTB		= 0x00;	// ???
	MCUCR		= 0x80; // MCU 제어 레지스터 MCUCR의 Bit7(SRE: External SRAM Enable)을 1(Enable)로 만든다.
}

// EX_LED를 켜거나 끈다.
void eled_control(unsigned char ledN, unsigned sw) {
	
    unsigned char led;
	
    led = 0;    // led를 0으로 초기화
	
    // led번호 값이 7보다 크거나 0보다 작으면 함수를 종료시킨다.
    if(ledN > 7 || ledN < 0)
		return;
	
    led = (1<<ledN);    // led에 led번호에 해당되는 Bit를 1로 만든다.
    
	if(sw) {
        // sw값이 0이 아니면(1이면) 전역변수 eledValue에 led번호에 해당하는 Bit를 led변수를 사용해 1로 만든다.
        eledValue	|= led;
	}
	else {
        // sw값이 0이면 전역변수 eledValue에 led번호에 해당하는 Bit를 led변수를 사용해 0으로 만든다.
        eledValue	&= ~(led);
	}

    // EX_LED값에 전역변수 eledValue를 대입해 led번호에 해당하는 led를 켜거나 끈다.
    // EX_LED는 WriteOnly이기 때문에 값을 읽어올 수 없으므로 전역변수를 통해 제어한다.
    EX_LED	= eledValue;
}

void eled_toggle(unsigned char ledN) {
	unsigned char led;
	
    led = 0;    // led를 0으로 초기화
	
    // led번호 값이 7보다 크거나 0보다 작으면 함수를 종료시킨다.
    if(ledN > 7 || ledN < 0) { return; }
		
	led = 1<<ledN;
	
	eledValue ^= led;
	
	EX_LED	= eledValue;
}

// 타이머를 이용하지 않는 delay함수, 정확한 지연시간을 알 수 없다.
void delay(unsigned short td) {
	volatile unsigned short i, j;
	for(i = 0; i < td; i++)
		for(j = 0; j < 1000; j++);
}