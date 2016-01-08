
#include "radio.h"
#include "DataAccess.h"

#define TIMEOUT 2500

using namespace ydleMaster ;

radio * radio::radioObj ;

void radio::Init () {
  // sets up the wiringPi library
  if (wiringPiSetup() < 0) {
    fprintf(stderr, "Unable to setup wiringPi: %s\n", strerror(errno));
    return;
  }
  rx_pin = PARAM_INT("radio.rx_pin");
  tx_pin = PARAM_INT("radio.tx_pin");
	pinMode(tx_pin, OUTPUT);
	pinMode(rx_pin, INPUT);
  start = true;
}

void radio::Start () {
  if ( wiringPiISR(rx_pin, INT_EDGE_BOTH, &radio::radioInterrupt) < 0 ) {
    fprintf(stderr, "Unable to setup ISR: %s\n", strerror(errno));
    return;
  }
}

void radio::Receive()
{
  if (start) {
    start = false;
    return;
  }
  uint8_t rx_value = digitalRead(rx_pin);
  Dispatch (rx_value);
}

void radio::Send(bool b)
{
  digitalWrite(tx_pin, b);
}

radio::radio()
{
  radioObj = this ;
}

void radio::radioInterrupt()
{
  radioObj->Receive();
}