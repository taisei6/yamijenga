#include <Adafruit_NeoPixel.h>

#define PIN_AX 0
#define PIN_AY 1
#define PIN_AZ 2

int ax, ay, az;
int ax_prev, ay_prev, az_prev;
int dax, day, daz;
int dax_prev, day_prev, daz_prev;
float l_dax, l_day, l_daz;
float l_dax_prev, l_day_prev, l_daz_prev;
int colx, coly, colz;
int i;


#define PIN            6
#define NUMPIXELS      2
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define SLEEP_THRESHOLD 8
#define ACTIVATE_THRESHOLD 210
#define K_VARIABLE 15

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
  randomSeed(analogRead(5));

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
  // put your main code here, to run repeatedly:
  ax = analogRead(PIN_AX);
  ay = analogRead(PIN_AY);
  az = analogRead(PIN_AZ);

  dax = abs(ax - ax_prev);
  day = abs(ay - ay_prev);
  daz = abs(az - az_prev);
  /*
    dax++;
    day++;
    daz++;
  */

  if ( dax <  1 )dax = 1;
  if ( day <  1 )day = 1;
  if ( daz <  1 )daz = 1;
  
    Serial.print(dax);
    Serial.print(" ");
    Serial.print(day);
    Serial.print(" ");
    Serial.print(daz);
    Serial.print("   ");
    
  
  l_dax = log(dax) * 50;
  l_day = log(day) * 50;
  l_daz = log(daz) * 50;


  float d = sqrt( pow(dax, 2) + pow(day, 2) + pow(daz, 2) );

  //Serial.println(d);

  if ( d > ACTIVATE_THRESHOLD ) {

  }
  else {
    l_dax = ((l_dax - l_dax_prev) / k) + l_dax_prev;
    l_day = ((l_day - l_day_prev) / k) + l_day_prev;
    l_daz = ((l_daz - l_daz_prev) / k) + l_daz_prev;
    //Serial.println("Woke up!!");
  }
}

void loop() {

  updateParameters();

  Serial.print(l_dax);
  Serial.print(" ");
  Serial.print(l_day);
  Serial.print(" ");
  Serial.print(l_daz);
  Serial.println("   ");


  if ( flg_sleep == false ) {
    if ( checkStatic(5000) == true ) {
      // go to sleep
      Serial.println("Go to Sleep");
      flg_sleep = true;
    }
  }

  // Sleep transaction
  {
    if ( flg_sleep == true ) {
      float s_lx;
      float s_ly;
      float s_lz;
      s_lx = l_dax;
      s_ly = l_day;
      s_lz = l_daz;

      float theta = 0.0;
      while (checkStatic(1)) {
        
        if(s_lx > 1) s_lx = s_lx - 0.3;
        else s_lx = 0;
        if(s_ly > 1) s_ly = s_ly - 0.3;
        else s_ly = 0;
        if(s_lz > 1) s_lz = s_lz - 0.3;
        else s_lz = 0;
        pixels.setPixelColor(0, pixels.Color((int)s_lx, (int)s_ly, (int)s_lz));
        pixels.setPixelColor(1, pixels.Color((int)s_lx, (int)s_ly, (int)s_lz));
        pixels.show();
        /*Serial.print(s_lx);
        Serial.print(" ");
        Serial.print(s_ly);
        Serial.print(" ");
        Serial.print(s_lz);
        Serial.print(" ");
        Serial.println("Run");*/
        
        delayMicroseconds(500);
        int num = random(6000);
        //Serial.println(num);
        if ( num == 100 ) {
          for ( float theta = 0.0; theta < PI; theta = theta + 0.002 ) {
            theta = theta + 0.002;
            double rad = sin(theta);
            int col = rad * 200;
            pixels.setPixelColor(0, pixels.Color(col, col, col));
            pixels.setPixelColor(1, pixels.Color(col, col, col));
            pixels.show();
          }
          pixels.setPixelColor(0, pixels.Color(0, 0, 0));
          pixels.setPixelColor(1, pixels.Color(0, 0, 0));
          pixels.show();
          delay(500);


        }
        updateParameters();
      }

      flg_sleep = false;
      Serial.println("Wake up");
    }



  }


  // Normal transaction
  {
    if ( flg_sleep == false ) {
      if( l_dax >= 255 ) l_dax = 255;
      if( l_day >= 255 ) l_day = 255;
      if( l_daz >= 255 ) l_daz = 255;
      pixels.setPixelColor(0, pixels.Color(l_dax, l_day, l_daz));
      pixels.setPixelColor(1, pixels.Color(l_dax, l_day, l_daz));
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
  l_dax_prev = l_dax;
  l_day_prev = l_day;
  l_daz_prev = l_daz;
  delay(1);
}
