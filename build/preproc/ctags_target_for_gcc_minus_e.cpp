# 1 "c:\\Users\\HDP\\Desktop\\temp\\projects\\mhamad feris\\main.ino"
# 2 "c:\\Users\\HDP\\Desktop\\temp\\projects\\mhamad feris\\main.ino" 2
# 3 "c:\\Users\\HDP\\Desktop\\temp\\projects\\mhamad feris\\main.ino" 2
# 4 "c:\\Users\\HDP\\Desktop\\temp\\projects\\mhamad feris\\main.ino" 2

int button_speed = 1;
int button_times = 0;
int button_value = 100;
int button_value_max = 2000;

void setup()
{
 Serial.begin(9600);
 init_pins();
 lcd_init();
 control_init();
}

void loop()
{
 int analog_read = analogRead(A0);
 int termometer = analog_read / 0.0048828125 * 5 / 1024;

 loop_button();
 control_loop();

 delay(200);
}

void loop_button()
{
 if (digitalRead(6))
 {
  control_ok();
 }
 if (digitalRead(7))
 {
  control_reset();
 }

 bool button_pressed = false;
 if (digitalRead(8) == 0x0)
 {
  button_value += button_speed;
  button_times++;
  button_pressed = true;
  button_value = min(button_value, button_value_max);
  control_setup(button_value);
 }
 if (digitalRead(9) == 0x0)
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
