#include "hx711_raw.h"


void Hx711Raw::begin(uint8_t dt_pin, uint8_t sck_pin)
{
	_dt_pin = dt_pin;
	_sck_pin = sck_pin;

	pinMode(_dt_pin, INPUT);
	pinMode(_sck_pin, OUTPUT);
}

void Hx711Raw::update(void)
{
	// wait for sample
	while (digitalRead(_dt_pin))
		delay(0);
	// disable interrupts because hx711 is sensitive
	noInterrupts();
	// clock in 24 sample bits
	value.as_buff[2] = shiftIn(_dt_pin, _sck_pin, MSBFIRST);
	value.as_buff[1] = shiftIn(_dt_pin, _sck_pin, MSBFIRST);
	value.as_buff[0] = shiftIn(_dt_pin, _sck_pin, MSBFIRST);
	// set channel and 128 gain factor for the next reading
	digitalWrite(_sck_pin, HIGH);
	digitalWrite(_sck_pin, LOW);
	// re-enable interrupts
	interrupts();
	// fix hx711 offset
	value.as_buff[3] = 0;
	value.as_uint32 += HX711_OFFSET;
	value.as_buff[3] = 0;
}
