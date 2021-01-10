#include <Adafruit_NeoPixel.h>

#define PIN_AX 0
#define PIN_AY 1
#define PIN_AZ 2

int ax, ay, az;
int ax_prev, ay_prev, az_prev;
int dax, day, daz;
int dax_prev, day_prev, daz_prev;


#define PIN            6
#define NUMPIXELS      2
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

struct Color {
  int r;
  int g;
  int b;
} color;

#define SLEEP_THRESHOLD 10
#define ACTIVATE_THRESHOLD 100
#define K_VARIABLE 1000

boolean flg_sleep;
float k = K_VARIABLE;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pixels.begin();
  flg_sleep = false;
  ax = ay = az = 0;
  ax_prev = ay_prev = az_prev = 0;
  dax = day = daz = 0;
  dax_prev = day_prev = daz_prev = 0;

}

unsigned long time_start = 0;
boolean checkStatic(int _ms)
{
  float d = sqrt( pow(dax, 2) + pow(day, 2) + pow(daz, 2) );

  // if got some motions.
  if ( d > SLEEP_THRESHOLD ) {
    time_start = millis();
  }
  // static motions
  else {
    if ( millis() - time_start > _ms ) {
      return true;
    }
  }
  return false;
}

void updateParameters()
{
  float d = sqrt( pow(dax, 2) + pow(day, 2) + pow(daz, 2) );

  if ( d > ACTIVATE_THRESHOLD ) {

  }
  else {
    dax = ((dax - dax_prev) / k) + dax_prev;
    day = ((day - day_prev) / k) + day_prev;
    daz = ((daz - daz_prev) / k) + daz_prev;
  }

  /*
    pixels.setPixelColor(0, pixels.Color(colx, coly, colz));
    pixels.setPixelColor(1, pixels.Color(colx, coly, colz));
    pixels.show();
  */
}

void loop() {
  // put your main code here, to run repeatedly:
  ax = analogRead(PIN_AX);
  ay = analogRead(PIN_AY);
  az = analogRead(PIN_AZ);

  dax = abs(ax - ax_prev);
  day = abs(ay - ay_prev);
  daz = abs(az - az_prev);

  updateParameters();

  Serial.print(dax);
  Serial.print(",");
  Serial.print(day);
  Serial.print(",");
  Serial.println(daz);

  if ( checkStatic(5000) == true ) {
    // go to sleep
    if ( flg_sleep == false )//Serial.println("Go to Sleep");
      flg_sleep = true;
  }
  else {
    if ( flg_sleep == true)//Serial.println("Woke up!!");
      flg_sleep = false;
  }

  // Sleep transaction
  {
    if ( flg_sleep == true ) {

    }
  }

  // Normal transaction
  {
    if ( flg_sleep == false ) {
      pixels.setPixelColor(0, pixels.Color(dax, day, daz));
      pixels.setPixelColor(1, pixels.Color(dax, day, daz));
      pixels.show();
    }
  }
  // update Accerometer
  ax_prev = ax;
  ay_prev = ay;
  az_prev = az;
  dax_prev = dax;
  day_prev = day;
  daz_prev = daz;
  delay(1);
}
