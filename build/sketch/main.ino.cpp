#include <Arduino.h>
#line 1 "c:\\Users\\HDP\\Desktop\\temp\\projects\\mhamad feris\\main.ino"
#include "src/config.h"
#include "src/lcd.h"
#include "src/control.h"

int button_speed = 1;
int button_times = 0;
int button_value = 100;
int button_value_max = 2000;

#line 10 "c:\\Users\\HDP\\Desktop\\temp\\projects\\mhamad feris\\main.ino"
void setup();
#line 18 "c:\\Users\\HDP\\Desktop\\temp\\projects\\mhamad feris\\main.ino"
void loop();
#line 29 "c:\\Users\\HDP\\Desktop\\temp\\projects\\mhamad feris\\main.ino"
void loop_button();
#line 10 "c:\\Users\\HDP\\Desktop\\temp\\projects\\mhamad feris\\main.ino"
void setup()
{
	Serial.begin(9600);
	init_pins();
	lcd_init();
	control_init();
}

void loop()
{
	int analog_read = analogRead(Temp_Read);
	int termometer = analog_read / TEMP_SENSOR_VOLT_DEGRE * 5 / 1024;

	loop_button();
	control_loop();

	delay(REFRESH_TIME);
}

void loop_button()
{
	if (digitalRead(BUTTON_OK))
	{
		control_ok();
	}
	if (digitalRead(BUTTON_RESET))
	{
		control_reset();
	}

	bool button_pressed = false;
	if (digitalRead(BUTTON_UP) == LOW)
	{
		button_value += button_speed;
		button_times++;
		button_pressed = true;
		button_value = min(button_value, button_value_max);
		control_setup(button_value);
	}
	if (digitalRead(BUTTON_DOWN) == LOW)
	{
		button_value -= button_speed;
		button_times++;
		button_pressed = true;
		button_value = max(button_value, 1);
		control_setup(button_value);
	}

	if (button_pressed)
	{
		if (button_times > 9)
		{
			button_speed = button_speed * 5;
			button_times = 0;
		}

		if (button_speed > 100)
		{
			button_speed = 100;
		}
	}
	else
	{
		button_speed = 1;
		button_times = 0;
	}
}
