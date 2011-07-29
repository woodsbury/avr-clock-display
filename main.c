#include <avr/interrupt.h>

#define LED_ANIMATIONS 3

const unsigned char led_display[LED_ANIMATIONS * 60] = {
	0b00000001,
	0b10000010,
	0b01000100,
	0b00101000,
	0b00010000,
	0b00101000,
	0b01000100,
	0b10000010,
	0b00000001,
	0b00000000,			// 10 seconds
	0b00000001,
	0b10000011,
	0b11000110,
	0b01101100,
	0b00111000,
	0b01101100,
	0b11000110,
	0b10000011,
	0b00000001,
	0b00000000,			// 20 seconds
	0b00000001,
	0b10000011,
	0b11000111,
	0b11101110,
	0b01111100,
	0b11101110,
	0b11000111,
	0b10000011,
	0b00000001,
	0b00000000,			// 30 seconds
	0b00000001,
	0b10000011,
	0b11000111,
	0b11101111,
	0b11111110,
	0b11101111,
	0b11000111,
	0b10000011,
	0b00000001,
	0b00000000,			// 40 seconds
	0b00000001,
	0b10000011,
	0b11000111,
	0b11101111,
	0b11111111,
	0b11101111,
	0b11000111,
	0b10000011,
	0b00000001,
	0b00000000,			// 50 seconds
	0b00000001,
	0b10000011,
	0b11000111,
	0b11101111,
	0b11111111,
	0b11111110,
	0b01111100,
	0b00111000,
	0b00010000,
	0b00000000,			// 60 seconds

	0b00000001,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000,
	0b00000001,
	0b00000000,			// 10 seconds
	0b00000001,
	0b00000011,
	0b00000110,
	0b00001100,
	0b00011000,
	0b00110000,
	0b01100000,
	0b11000000,
	0b10000001,
	0b00000000,			// 20 seconds
	0b00000001,
	0b00000011,
	0b00000111,
	0b00001110,
	0b00011100,
	0b00111000,
	0b01110000,
	0b11100000,
	0b11000001,
	0b00000000,			// 30 seconds
	0b00000001,
	0b00000011,
	0b00000111,
	0b00001111,
	0b00011110,
	0b00111100,
	0b01111000,
	0b11110000,
	0b11100001,
	0b00000000,			// 40 seconds
	0b00000001,
	0b00000011,
	0b00000111,
	0b00001111,
	0b00011111,
	0b00111110,
	0b01111100,
	0b11111000,
	0b11110001,
	0b00000000,			// 50 seconds
	0b00000001,
	0b00000011,
	0b00000111,
	0b00001111,
	0b00011111,
	0b00111111,
	0b01111110,
	0b11111100,
	0b11111001,
	0b00000000,			// 60 seconds

	0b00010000,
	0b00101000,
	0b01000100,
	0b10000010,
	0b00000001,
	0b10000010,
	0b01000100,
	0b00101000,
	0b00010000,
	0b00000000,			// 10 seconds
	0b00010000,
	0b00111000,
	0b01101100,
	0b11000110,
	0b10000011,
	0b11000110,
	0b01101100,
	0b00111000,
	0b00010000,
	0b00000000,			// 20 seconds
	0b00010000,
	0b00111000,
	0b01111100,
	0b11101110,
	0b11000111,
	0b11101110,
	0b01111100,
	0b00111000,
	0b00010000,
	0b00000000,			// 30 seconds
	0b00010000,
	0b00111000,
	0b01111100,
	0b11111110,
	0b11101111,
	0b11111110,
	0b01111100,
	0b00111000,
	0b00010000,
	0b00000000,			// 40 seconds
	0b00010000,
	0b00111000,
	0b01111100,
	0b11111110,
	0b11111111,
	0b11111110,
	0b01111100,
	0b00111000,
	0b00010000,
	0b00000000,			// 50 seconds
	0b00010000,
	0b00111000,
	0b01111100,
	0b11111110,
	0b11111111,
	0b11101111,
	0b11000111,
	0b10000011,
	0b00000001,
	0b00000000			// 60 seconds
};

const unsigned char seven_segment[10] = {
	0b00111111,
	0b00000110,
	0b01011011,
	0b01001111,
	0b01100110,
	0b01101101,
	0b01111101,
	0b00000111,
	0b01111111,
	0b01101111
};

// state = 0xFF, display hours, state = 0x00, display minutes
volatile uint8_t state;
volatile uint8_t buttons;

volatile uint8_t seconds;
volatile uint8_t minutes;
volatile uint8_t hours;

volatile uint8_t display_counter;

ISR(TIMER1_COMPA_vect)
{
	static uint8_t counter;

	if (seconds > 58)
	{
		seconds = 0;
		if (minutes > 58)
		{
			minutes = 0;
			if (hours > 22)
			{
				hours = 0;
			}
			else
			{
				++hours;
			}
		}
		else
		{
			++minutes;
			display_counter = (display_counter + 1) % LED_ANIMATIONS;
		}
	}
	else
	{
		++seconds;
	}

	if (counter == 1)
	{
		counter = 0;
		state = ~state;
	}
	else
	{
		++counter;
	}
}

ISR(TIMER2_OVF_vect)
{
	static uint8_t second_counter; // Display second LEDs longer as they are dimmer

	if (bit_is_set(PINC, PC3) && second_counter)
	{
		PORTD = led_display[seconds + display_counter * 60];
		second_counter = ~second_counter;
	}
	else if (bit_is_set(PINC, PC3))
	{
		PORTC = _BV(4);
		if (state)
		{
			PORTD = seven_segment[hours % 10];
		}
		else
		{
			PORTD = seven_segment[minutes % 10];
			PORTD |= _BV(7);
		}
	}
	else if (bit_is_set(PINC, PC4))
	{
		PORTC = _BV(5);
		if (state)
		{
			PORTD = seven_segment[hours / 10];
		}
		else
		{
			PORTD = seven_segment[minutes / 10];
		}
	}
	else
	{
		PORTC = _BV(3);
		PORTD = led_display[seconds + display_counter * 60];
		second_counter = ~second_counter;
	}
}

ISR(TIMER2_COMP_vect)
{
	static uint8_t buttons_old, buttons_new, counter;

	PORTD = 0;

	buttons_old = buttons_new;
	buttons_new = PINB;

	if (buttons_old == buttons_new)
	{
		++counter;
	}
	else
	{
		counter = 0;
	}

	if (counter > 5)
	{
		buttons = buttons_new;
		return;
	}
}

int main(void)
{
	uint8_t button_press = 0;

	PORTB = 0b00000111;
	PORTC = 0b00000000;
	PORTD = 0b00000000;

	DDRB = 0b00000000;
	DDRC = 0b00111000;
	DDRD = 0b11111111;

	TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);
	OCR1A = 15624;
	TCCR2 = _BV(CS22) | _BV(CS21);
	OCR2 = 0x21;
	TIMSK = _BV(OCIE2) | _BV(TOIE2) | _BV(OCIE1A);

	PORTC = _BV(3);

	sei();

	while (1)
	{
		if (bit_is_clear(buttons, 0) && bit_is_clear(buttons, 2) && button_press)
		{
			cli();

			state = 0x00;

			if (minutes > 58)
			{
				minutes = 0;
				if (hours > 22)
				{
					hours = 0;
				}
				else
				{
					++hours;
				}
			}
			else
			{
				++minutes;
			}

			sei();

			button_press = 0;
		}
		else if (bit_is_clear(buttons, 1) && bit_is_clear(buttons, 2) && button_press)
		{
			cli();

			state = 0xFF;

			if (hours > 22)
			{
				hours = 0;
			}
			else
			{
				++hours;
			}

			sei();

			button_press = 0;
		}
		else if (bit_is_clear(buttons, 0) && button_press)
		{
			if (OCR2 < 0x60)
			{
				OCR2 += 0x08;
			}

			button_press = 0;
		}
		else if (bit_is_clear(buttons, 1) && button_press)
		{
			if (OCR2 > 0x08)
			{
				OCR2 -= 0x08;
			}

			button_press = 0;
		}
		else if (bit_is_set(buttons, 0) && bit_is_set(buttons, 1))
		{
			button_press = 1;
		}
	}
}

