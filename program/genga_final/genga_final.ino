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
#define NUMPIXELS      4
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define SLEEP_THRESHOLD 5
#define ACTIVATE_THRESHOLD 25
#define K_VARIABLE 15

boolean active = false;
float k = K_VARIABLE;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pixels.begin();
  ax = ay = az = 0;
  ax_prev = ay_prev = az_prev = 0;
  dax = day = daz = 0;
  dax_prev = day_prev = daz_prev = 0;

}

unsigned long time_start = 0;

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

  Serial.println(d);

  if ( d > ACTIVATE_THRESHOLD ) {
    active = true;
  }
  else {
    l_dax = ((l_dax - l_dax_prev) / k) + l_dax_prev;
    l_day = ((l_day - l_day_prev) / k) + l_day_prev;
    l_daz = ((l_daz - l_daz_prev) / k) + l_daz_prev;
    //Serial.println("Woke up!!");
    active = false;
  }
//  delay(50);
}

boolean active_pre = false;

void loop() {

  updateParameters();

//  Serial.print(d);
//  Serial.print(l_dax);
//  Serial.print(" ");
//  Serial.print(l_day);
//  Serial.print(" ");
//  Serial.print(l_daz);
//  Serial.println("   ");


  // Normal transaction

  if(active == false){
    
    pixels.clear();
    pixels.show();
    }

    
    if ( active == true) {
      if( l_dax >= 255 ) l_dax = 255;
      if( l_day >= 255 ) l_day = 255;
      if( l_daz >= 255 ) l_daz = 255;
      float w = 0.0;
      for( w = 0.0;w < PI;w = w + 0.01){
        for(int i = 0;i<6;i++){
//        pixels.setPixelColor(i, pixels.Color(l_dax, l_day, l_daz));
        pixels.setPixelColor(i, pixels.Color(l_dax*sin(w), l_day*sin(w), l_daz*sin(w)));
        pixels.show();
        }
      }
//      delay(2000);

    
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

//  active_pre = active;
  
}
