#pragma once

#include "config.h"
#include "lcd.h"
#include <EEPROM.h>

#define CONTROL_STATE_STANDBY 0
#define CONTROL_STATE_SETUP_TEMP1 1
#define CONTROL_STATE_SETUP_TIME1 2
#define CONTROL_STATE_SETUP_HOLD1 3
#define CONTROL_STATE_SETUP_TEMP2 4
#define CONTROL_STATE_SETUP_TIME2 5
#define CONTROL_STATE_SETUP_HOLD2 6
#define CONTROL_STATE_RUN_RAISEUP_1 7
#define CONTROL_STATE_RUN_HOLD_1 8
#define CONTROL_STATE_RUN_RAISEUP_2 9
#define CONTROL_STATE_RUN_HOLD_2 10
#define CONTROL_STATE_END 11

#define EEPROM_TEMP_1 10
#define EEPROM_TIME_1 12
#define EEPROM_HOLD_1 14
#define EEPROM_TEMP_2 16
#define EEPROM_TIME_2 18
#define EEPROM_HOLD_2 20

int control_state = CONTROL_STATE_STANDBY;
int control_count_down = 0;
int control_count_down_sec = 0;
float control_a = 0;
float control_b = 0;
float termometer = 0;

struct Control_Data
{
	int temp1;
	int time1;
	int hold1;
	int temp2;
	int time2;
	int hold2;
};

Control_Data control_data;
void eeprom_write_int(int address, int value)
{
	EEPROM.write(address, value & 0xFF);
	EEPROM.write(address + 1, (value >> 8) & 0xFF);
}

int eeprom_read_int(int address)
{
	int value = EEPROM.read(address);
	value = value + (EEPROM.read(address + 1) << 8);
	return value;
}

void control_temp_relay_off()
{
	digitalWrite(Relay, LOW);
}

void control_temp_relay_on()
{
	digitalWrite(Relay, LOW);
}

void control_init()
{
	control_state = CONTROL_STATE_STANDBY;

	control_data.temp1 = eeprom_read_int(EEPROM_TEMP_1);
	control_data.time1 = eeprom_read_int(EEPROM_TIME_1);
	control_data.hold1 = eeprom_read_int(EEPROM_HOLD_1);
	control_data.temp2 = eeprom_read_int(EEPROM_TEMP_2);
	control_data.time2 = eeprom_read_int(EEPROM_TIME_2);
	control_data.hold2 = eeprom_read_int(EEPROM_HOLD_2);
}

void control_temp_target(int temp)
{
	if (temp >= termometer)
		return;

	int step = control_count_down % RELAY_TIME_TOTAL;
	if (step > RELAY_TIME_OFF)
	{
		control_temp_relay_on();
	}
	else
	{
		control_temp_relay_off();
	}
}

void control_init_temp1()
{
	int time = control_data.time1;
	float temp = control_data.temp1;
	control_a = temp / time;
	control_b = control_data.temp1 - control_a * time;
}

void control_init_temp2()
{
	int time = control_data.time2;
	float temp = control_data.temp2 - control_data.temp1;
	control_a = temp / time;
	control_b = control_data.temp2 - control_a * time;
}

void control_setup_temp1(int temp)
{
	lcd_clean();
	lcd_print("Enter Tempre. 1:");
	lcd_set_cursor(0, 1);
	lcd_print("Tempre. 1 = ");
	lcd_print(temp);
	lcd_flush();
}

void control_setup_time1(int time)
{
	lcd_clean();
	lcd_print("Enter Time 1:");
	lcd_set_cursor(0, 1);
	lcd_print("Time 1 =    ");
	lcd_print(time);
	lcd_flush();
}

void control_setup_hold1(int time)
{
	lcd_clean();
	lcd_print("Enter H.Time 1:");
	lcd_set_cursor(0, 1);
	lcd_print("H.Time 1 =  ");
	lcd_print(time);
	lcd_flush();
}

void control_setup_temp2(int temp)
{
	lcd_clean();
	lcd_print("Enter Tempre. 2:");
	lcd_set_cursor(0, 1);
	lcd_print("Tempre. 2 = ");
	lcd_print(temp);
	lcd_flush();
}

void control_setup_time2(int time)
{
	lcd_clean();
	lcd_print("Enter Time 2:");
	lcd_set_cursor(0, 1);
	lcd_print("Time 2 =    ");
	lcd_print(time);
	lcd_flush();
}

void control_setup_hold2(int time)
{
	lcd_clean();
	lcd_print("Enter H.Time 2:");
	lcd_set_cursor(0, 1);
	lcd_print("H.Time 2 =  ");
	lcd_print(time);
	lcd_flush();
}

void control_state_show(int target, char *str)
{
	lcd_clean();
	lcd_print("PrevTemp = ");
	lcd_print(termometer);
	lcd_set_cursor(0, 1);
	lcd_print("SV(");
	lcd_print(str);
	lcd_print(") = ");
	lcd_print(target);
	lcd_flush();
}

void control_show_value(int value)
{
	lcd_set_cursor(12, 1);
	lcd_print("    ");
	lcd_print(value);
	lcd_flush();
}

void control_show_temp()
{
	lcd_set_cursor(11, 0);
	lcd_print("     ");
	lcd_print(termometer);
	lcd_flush();
}

void control_setup(int value)
{
	switch (control_state)
	{
	case CONTROL_STATE_SETUP_TEMP1:
		control_show_value(value);
		break;
	case CONTROL_STATE_SETUP_TIME1:
		control_show_value(value);
		break;
	case CONTROL_STATE_SETUP_HOLD1:
		control_show_value(value);
		break;
	case CONTROL_STATE_SETUP_TEMP2:
		control_show_value(value);
		break;
	case CONTROL_STATE_SETUP_TIME2:
		control_show_value(value);
		break;
	case CONTROL_STATE_SETUP_HOLD2:
		control_show_value(value);
		break;
	}
}

void control_goto_time1()
{
	control_count_down = control_data.time1;
	control_state = CONTROL_STATE_RUN_RAISEUP_1;
	control_state_show(control_data.temp1, "Temp 1");
}

void control_goto_time2()
{
	control_count_down = control_data.time2;
	control_state = CONTROL_STATE_RUN_RAISEUP_2;
	control_state_show(control_data.temp2, "Temp 2");
}

void control_goto_hold1()
{
	control_count_down = control_data.hold1;
	control_state = CONTROL_STATE_RUN_HOLD_1;
	control_state_show(control_data.temp1, "Hold 1");
}

void control_goto_hold2()
{
	control_count_down = control_data.hold2;
	control_state = CONTROL_STATE_RUN_HOLD_2;
	control_state_show(control_data.temp2, "Hold 2");
}

void control_goto_end()
{
	control_state = CONTROL_STATE_END;
	lcd_clean();
	lcd_print("END");
	lcd_flush();

	control_temp_relay_off();
}

void control_ok(int value = 0)
{
	switch (control_state)
	{
	case CONTROL_STATE_STANDBY:
	{
		control_state = CONTROL_STATE_SETUP_TEMP1;
		control_setup_temp1(control_data.temp1);
		break;
	}

	case CONTROL_STATE_SETUP_TEMP1:
	{
		control_state = CONTROL_STATE_SETUP_TIME1;
		control_setup_time1(control_data.time1);

		if (value != control_data.temp1)
		{
			control_data.temp1 = value;
			eeprom_write_int(EEPROM_TEMP_1, value);
		}
		break;
	}
	case CONTROL_STATE_SETUP_TIME1:
	{
		control_state = CONTROL_STATE_SETUP_HOLD1;
		control_setup_hold1(control_data.hold1);

		if (value != control_data.time1)
		{
			control_data.time1 = value;
			eeprom_write_int(EEPROM_TIME_1, value);
		}
		break;
	}
	case CONTROL_STATE_SETUP_HOLD1:
	{
		control_state = CONTROL_STATE_SETUP_TEMP2;
		control_setup_temp2(control_data.temp2);

		if (value != control_data.hold1)
		{
			control_data.hold1 = value;
			eeprom_write_int(EEPROM_HOLD_1, value);
		}
		break;
	}
	case CONTROL_STATE_SETUP_TEMP2:
	{
		control_state = CONTROL_STATE_SETUP_TIME2;
		control_setup_time2(control_data.time2);

		if (value != control_data.temp2)
		{
			control_data.temp2 = value;
			eeprom_write_int(EEPROM_TEMP_2, value);
		}
		break;
	}
	case CONTROL_STATE_SETUP_TIME2:
	{
		control_state = CONTROL_STATE_SETUP_HOLD2;
		control_setup_hold2(control_data.hold2);

		if (value != control_data.time2)
		{
			control_data.time2 = value;
			eeprom_write_int(EEPROM_TIME_2, value);
		}
		break;
	}
	case CONTROL_STATE_SETUP_HOLD2:
	{
		if (value != control_data.hold2)
		{
			control_data.hold2 = value;
			eeprom_write_int(EEPROM_HOLD_2, value);
		}

		control_init_temp1();
		control_init_temp2();

		control_goto_time1();

		break;
	}
	}
}

void control_reset()
{
	control_state = 0;
}

void control_loop()
{
	control_show_temp();

	control_count_down_sec--;
	if (control_count_down_sec > 0)
		return;

	float temp = control_a * control_count_down + control_b;

	switch (control_state)
	{
	case CONTROL_STATE_RUN_RAISEUP_1:
		control_temp_target(temp);
		break;
	case CONTROL_STATE_RUN_HOLD_1:
		control_temp_target(control_data.temp1);
		break;
	case CONTROL_STATE_RUN_RAISEUP_2:
		control_temp_target(temp);
		break;
	case CONTROL_STATE_RUN_HOLD_2:
		control_temp_target(control_data.temp2);
		break;
	default:
		control_temp_relay_off();
		break;
	}

	control_count_down--;
	control_count_down_sec = 60 * 1000 / REFRESH_TIME;

	if (control_count_down <= 0)
	{
		switch (control_state)
		{
		case CONTROL_STATE_RUN_RAISEUP_1:
			control_goto_hold1();
			break;
		case CONTROL_STATE_RUN_HOLD_1:
			control_goto_time2();
			break;
		case CONTROL_STATE_RUN_RAISEUP_2:
			control_goto_hold2();
			break;
		case CONTROL_STATE_RUN_HOLD_2:
			control_goto_end();
			break;
		}
	}
}
