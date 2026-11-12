#include <Arduino.h>
#include "AimTimer.h"

void AimTimer::AimTimerBase(double INintervalSec) {
  intervalMil = INintervalSec * 1000;
  SVintervalMil = intervalMil;
  fst = true;
}

// constructor
AimTimer::AimTimer() {
  AimTimerBase(TIMER_INTERVAL_SEC);
}

AimTimer::AimTimer(double INintervalSec) {
  AimTimerBase(INintervalSec);
}

void AimTimer::set(double INintervalSec) {
  intervalMil = INintervalSec * 1000;
  SVintervalMil = intervalMil;
  nextMil = millis() + intervalMil;
}

void AimTimer::reset() {
  SVintervalMil = intervalMil;
  nextMil = millis() + intervalMil;
}

void AimTimer::nowOn() {
  SVintervalMil = intervalMil;
  nextMil = 0; 
  fst = false;
}

bool AimTimer::isOn() {
  
  bool rtcd = false;

  if ( intervalMil == 0 ) {
    return false;
  }

  
  if ( nextMil == 0 || millis() > nextMil) {
    nextMil = millis() + intervalMil;
    if (fst) {
      fst = false;
      rtcd = false;
    }
    else {
      rtcd = true;
    }
    rtcd = true;
  }
  else {
    rtcd = false;
  }
  
  return rtcd;
}

bool AimTimer::isOn(double INintervalSec) {
  unsigned long INintervalMil = INintervalSec * 1000;
  if (SVintervalMil != INintervalMil) {
    set(INintervalSec);
  }
  
  return isOn();
}