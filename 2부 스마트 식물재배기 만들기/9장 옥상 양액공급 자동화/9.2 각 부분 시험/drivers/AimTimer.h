#ifndef AIM_TIMER_H
#define AIM_TIMER_H

#ifndef TIMER_INTERVAL_SEC 
#define TIMER_INTERVAL_SEC  10.0
#endif 

class AimTimer {

    public:
    // constructor에는 void나 변수형이 없다 
    AimTimer(double intervalSec);
    AimTimer();
    void AimTimerBase(double intervalSec);

    void set(double intervalSec);
    void reset(); 
    void nowOn(); 

    bool isOn(double intervalSec);
    bool isOn();

    private:
    unsigned long nextMil = 0;
    unsigned long intervalMil; 
    unsigned long SVintervalMil; 

    bool fst;
};
// ↑class의 끝에는 ;가 있다
#endif