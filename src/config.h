#pragma once

// sendors
#define TEMP_SENSOR_VOLT_DEGRE 0.0048828125

// lcd config
#define LCD_ROWS 2
#define LCD_COLS 16
#define LCD_ADDRESS 0x27

// LCD pins
#define LCD_RS 12
#define LCD_EN 11
#define LCD_NSS 5
#define LCD_MOSI 4
#define LCD_MISO 3
#define LCD_SCK 2

// Buttons Pins
#define BUTTON_OK 6
#define BUTTON_RESET 7
#define BUTTON_UP 8
#define BUTTON_DOWN 9

// Other Pins
#define Relay 10
#define Temp_Read A0
#define Buzzer 13

// Relay ON/OFF in minute
// on raise temp up
//	- turn on for RELAY_TIME_ON minutes
//	- then turn off for RELAY_TIME_OFF minutes
#define RELAY_TIME_ON 1
#define RELAY_TIME_OFF 1
#define RELAY_TIME_TOTAL (RELAY_TIME_ON + RELAY_TIME_OFF)

// refrech time in mili second
#define REFRESH_TIME 200

void init_pins()
{
	pinMode(Relay, OUTPUT);
	pinMode(Buzzer, OUTPUT);
	pinMode(Temp_Read, INPUT);
	pinMode(BUTTON_OK, INPUT_PULLUP);
	pinMode(BUTTON_RESET, INPUT_PULLUP);
	pinMode(BUTTON_UP, INPUT_PULLUP);
	pinMode(BUTTON_DOWN, INPUT_PULLUP);

	digitalWrite(Relay, LOW);
	digitalWrite(Buzzer, LOW);
}