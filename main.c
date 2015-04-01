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

volatile unsigned char eledValue;    // EX_LED ¡¶æÓ ¿¸ø™∫Øºˆ
volatile unsigned char swFlag;
volatile unsigned char swIndex;
volatile unsigned char swCnt[8];

void hw_init (void);
void eled_control(unsigned char, unsigned);
void eled_toggle(unsigned char);
void delay(unsigned short td);

int main(void) {
	hw_init ();
    
    while(1) {
        for( swIndex = 0; swIndex < 8; swIndex++ ) {
            if( !(PINB ^ (1<<swIndex)) ) {
                swCnt[swIndex]++;
            }
            else {
                swCnt[swIndex] = 0;
            }
            if( swCnt[swIndex] >= 5 ) {
                eledValue   ^= (1<<swIndex);
                EX_LED      = eledValue;
            }
        }
    }
	return 0;
}

void hw_init (void) {
	eledValue   = 0;    // EX_LED∏¶ ¡¶æÓ«œ±‚ ¿ß«— ¿¸ø™∫Øºˆ eledValue∏¶ 0¿∏∑Œ √ ±‚»≠«—¥Ÿ.
	DDRB		= 0x00;	// PORTB∏¶ ¿‘∑¬(0)¿∏∑Œ ∏∏µÁ¥Ÿ.
	PORTB		= 0x00;	// ???
	MCUCR		= 0x80; // MCU ¡¶æÓ ∑π¡ˆΩ∫≈Õ MCUCR¿« Bit7(SRE: External SRAM Enable)¿ª 1(Enable)∑Œ ∏∏µÁ¥Ÿ.
}

// EX_LED∏¶ ƒ—∞≈≥™ ≤ˆ¥Ÿ.
void eled_control(unsigned char ledN, unsigned sw) {
	
    unsigned char led;
	
    led = 0;    // led∏¶ 0¿∏∑Œ √ ±‚»≠
	
    // ledπ¯»£ ∞™¿Ã 7∫∏¥Ÿ ≈©∞≈≥™ 0∫∏¥Ÿ ¿€¿∏∏È «‘ºˆ∏¶ ¡æ∑·Ω√≈≤¥Ÿ.
    if(ledN > 7 || ledN < 0)
		return;
	
    led = (1<<ledN);    // ledø° ledπ¯»£ø° «ÿ¥Áµ«¥¬ Bit∏¶ 1∑Œ ∏∏µÁ¥Ÿ.
    
	if(sw) {
        // sw∞™¿Ã 0¿Ã æ∆¥œ∏È(1¿Ã∏È) ¿¸ø™∫Øºˆ eledValueø° ledπ¯»£ø° «ÿ¥Á«œ¥¬ Bit∏¶ led∫Øºˆ∏¶ ªÁøÎ«ÿ 1∑Œ ∏∏µÁ¥Ÿ.
        eledValue	|= led;
	}
	else {
        // sw∞™¿Ã 0¿Ã∏È ¿¸ø™∫Øºˆ eledValueø° ledπ¯»£ø° «ÿ¥Á«œ¥¬ Bit∏¶ led∫Øºˆ∏¶ ªÁøÎ«ÿ 0¿∏∑Œ ∏∏µÁ¥Ÿ.
        eledValue	&= ~(led);
	}

    // EX_LED∞™ø° ¿¸ø™∫Øºˆ eledValue∏¶ ¥Î¿‘«ÿ ledπ¯»£ø° «ÿ¥Á«œ¥¬ led∏¶ ƒ—∞≈≥™ ≤ˆ¥Ÿ.
    // EX_LED¥¬ WriteOnly¿Ã±‚ ∂ßπÆø° ∞™¿ª ¿–æÓø√ ºˆ æ¯¿∏π«∑Œ ¿¸ø™∫Øºˆ∏¶ ≈Î«ÿ ¡¶æÓ«—¥Ÿ.
    EX_LED	= eledValue;
}

void eled_toggle(unsigned char ledN) {
	unsigned char led;
	
    led = 0;    // led∏¶ 0¿∏∑Œ √ ±‚»≠
	
    // ledπ¯»£ ∞™¿Ã 7∫∏¥Ÿ ≈©∞≈≥™ 0∫∏¥Ÿ ¿€¿∏∏È «‘ºˆ∏¶ ¡æ∑·Ω√≈≤¥Ÿ.
    if(ledN > 7 || ledN < 0) { return; }
		
	led = 1<<ledN;
	
	eledValue ^= led;
	
	EX_LED	= eledValue;
}

// ≈∏¿Ã∏”∏¶ ¿ÃøÎ«œ¡ˆ æ ¥¬ delay«‘ºˆ, ¡§»Æ«— ¡ˆø¨Ω√∞£¿ª æÀ ºˆ æ¯¥Ÿ.
void delay(unsigned short td) {
	volatile unsigned short i, j;
	for(i = 0; i < td; i++)
		for(j = 0; j < 1000; j++);
}