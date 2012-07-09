#include <lunchbox.h>
#include <lunchbox/dht.h>
#include <lunchbox/port.h>

#define LED1 10 // LED1 is on P1.0
#define LED2 16 // LED2 is on P1.6
#define PUMP 14 // fridge pump is on P1.4
#define DHT1 15 // fridge sensor on P1.5
#define DHT2 17 // ambient sensor on P1.7

#define HEATING 0
#define COOLING 1

#define DESIRED_TEMP 211
#define MAX_TEMP DESIRED_TEMP + 11
#define MIN_TEMP DESIRED_TEMP - 11

dht fridge;
dht ambient;
int mode;

// This function is called once when your program starts.
void setup(void) {
  wdt_disable(); // disable the watchdog timer

  pinMode(LED1, OUTPUT); // set LED1 to an output
  pinMode(LED2, OUTPUT); // set LED2 to an output
  pinMode(PUMP, OUTPUT); // set fridge to an output
  pinMode(DHT1, INPUT); // set fridge to an output
  pinMode(DHT2, INPUT); // set fridge to an output

  dhtInit(&fridge, DHT1);
  dhtSetup(&fridge);
  dhtInit(&ambient, DHT2);
  dhtSetup(&ambient);

  digitalWrite(PUMP, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  mode = HEATING;

  delaySeconds(10); /* Let the sensor stabilize. */
  portInit(0);

  __enable_interrupt();
}

void red(void) {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
}

void green(void) {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
}

void off(void) {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
}

void on(void) {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
}

// This function is called over and over in an infinite loop.
void loop(void) {
  int fridge_temp;

  on();
  wdt_enable();

  if(DHT_OK != dhtRead(&fridge)) {
    digitalWrite(LED1, HIGH);
    return;
  }

  if(DHT_OK != dhtRead(&ambient)) {
    digitalWrite(LED1, HIGH);
    return;
  }

  wdt_disable();
  off();

  portTx(ambient.rhIntegral);
  portTx(ambient.rhDecimal);
  portTx(ambient.tIntegral);
  portTx(ambient.tDecimal);
  portTx(fridge.rhIntegral);
  portTx(fridge.rhDecimal);
  portTx(fridge.tIntegral);
  portTx(fridge.tDecimal);
  portTx('\n');

  fridge_temp = (fridge.tIntegral << 8) + fridge.tDecimal;

  switch(mode) {
    case COOLING:
      if (fridge_temp <= MIN_TEMP) {
        // Fridge is too cold, need to heat
        mode = HEATING;
        digitalWrite(PUMP, LOW);
        red();
      } else {
        // Currently cooling
        green();
      }
      break;
    case HEATING:
      if (fridge_temp >= MAX_TEMP) {
        // Fridge is too hot, need to cool
        mode = COOLING;
        digitalWrite(PUMP, HIGH);
        green();
      } else {
        // Currently heating
        red();
      }
      break;
  }

  delaySeconds(2);
}
