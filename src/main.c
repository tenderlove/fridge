#include <lunchbox.h>
#include <lunchbox/dht.h>
#include <lunchbox/port.h>

#define LED1 10 // LED1 is on P1.0
#define LED2 16 // LED2 is on P1.6
#define DHT1 15 // fridge sensor on P1.5
#define RED LED1
#define GREEN LED2

#define HEATING 0
#define COOLING 1

#define DESIRED_TEMP 200
#define MAX_TEMP DESIRED_TEMP + 11
#define MIN_TEMP DESIRED_TEMP - 11

dht fridge;
dht ambient;
int mode;

void off(void) {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
}

void on(void) {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
}

// This function is called once when your program starts.
void setup(void) {
  wdt_disable(); // disable the watchdog timer

  pinMode(LED1, OUTPUT); // set LED1 to an output
  pinMode(LED2, OUTPUT); // set LED2 to an output
  pinMode(DHT1, INPUT); // set fridge to an output
  on();

  dhtInit(&fridge, DHT1);
  dhtSetup(&fridge);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  delaySeconds(1); /* Let the sensor stabilize. */
  portInit(0);

  __enable_interrupt();
  portTx(0);
  portTx(0);
  portTx(0);
  portTx(0);
  portTx('\n');
  off();
}

void green(void) { digitalWrite(GREEN, HIGH); }
void red(void)   { digitalWrite(RED, HIGH); }

#define WRITE_TEMP(a, b, c, d) \
  portTx(a); \
  portTx(b); \
  portTx(c); \
  portTx(d); \
  portTx('\n'); \

// This function is called over and over in an infinite loop.
void loop(void) {
  int read;

  green();

  wdt_enable();
  read = dhtRead(&fridge);
  wdt_disable();

  if(DHT_OK == read) {
    WRITE_TEMP(fridge.rhIntegral,
               fridge.rhDecimal,
               fridge.tIntegral,
               fridge.tDecimal);
  } else {
    red();
    WRITE_TEMP(99, 99, 99, 99);
  }

  off();

  delaySeconds(2);
}
