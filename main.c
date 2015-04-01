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

volatile unsigned char eledValue;    // EX_LED ���� ��������
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
	eledValue   = 0;    // EX_LED�� �����ϱ� ���� �������� eledValue�� 0���� �ʱ�ȭ�Ѵ�.
	DDRB		= 0x00;	// PORTB�� �Է�(0)���� �����.
	PORTB		= 0x00;	// ???
	MCUCR		= 0x80; // MCU ���� �������� MCUCR�� Bit7(SRE: External SRAM Enable)�� 1(Enable)�� �����.
}

// EX_LED�� �Ѱų� ����.
void eled_control(unsigned char ledN, unsigned sw) {
	
    unsigned char led;
	
    led = 0;    // led�� 0���� �ʱ�ȭ
	
    // led��ȣ ���� 7���� ũ�ų� 0���� ������ �Լ��� �����Ų��.
    if(ledN > 7 || ledN < 0)
		return;
	
    led = (1<<ledN);    // led�� led��ȣ�� �ش�Ǵ� Bit�� 1�� �����.
    
	if(sw) {
        // sw���� 0�� �ƴϸ�(1�̸�) �������� eledValue�� led��ȣ�� �ش��ϴ� Bit�� led������ ����� 1�� �����.
        eledValue	|= led;
	}
	else {
        // sw���� 0�̸� �������� eledValue�� led��ȣ�� �ش��ϴ� Bit�� led������ ����� 0���� �����.
        eledValue	&= ~(led);
	}

    // EX_LED���� �������� eledValue�� ������ led��ȣ�� �ش��ϴ� led�� �Ѱų� ����.
    // EX_LED�� WriteOnly�̱� ������ ���� �о�� �� �����Ƿ� ���������� ���� �����Ѵ�.
    EX_LED	= eledValue;
}

void eled_toggle(unsigned char ledN) {
	unsigned char led;
	
    led = 0;    // led�� 0���� �ʱ�ȭ
	
    // led��ȣ ���� 7���� ũ�ų� 0���� ������ �Լ��� �����Ų��.
    if(ledN > 7 || ledN < 0) { return; }
		
	led = 1<<ledN;
	
	eledValue ^= led;
	
	EX_LED	= eledValue;
}

// Ÿ�̸Ӹ� �̿����� �ʴ� delay�Լ�, ��Ȯ�� �����ð��� �� �� ����.
void delay(unsigned short td) {
	volatile unsigned short i, j;
	for(i = 0; i < td; i++)
		for(j = 0; j < 1000; j++);
}