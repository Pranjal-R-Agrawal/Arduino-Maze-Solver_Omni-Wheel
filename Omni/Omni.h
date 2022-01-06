#ifndef Omni_h
#define Omni_h

#include "Arduino.h"

class Omni {
    public:
        Omni (bool adjustSpeed = false, double optimumVoltage = 0, double currentVoltage = 0, double constant = 0);
        
        void motors (int spd1, int spd2, int spd3, int spd4);
        void goStraight (int spd);
        void goStraight (int dir, int spd);
        void changeDir (int change);
        void setDir (int dir);
        void decelerate (int start, int decrement);
        void brake (int x);

        int cDir, lDir, rDir, uDir;
        int nTop, nLeft, nCenter, nRight;
        int eTop, eCenter, eBottom, eRight;
        int wTop, wCenter, wBottom, wLeft;
        int sBottom, sLeft, sCenter, sRight;

    private:
        bool _adjustSpeed;
        double _optimumVoltage, _currentVoltage, _constant;
        int _motorPins[4][3];
        int _adjust (double spd);
        int _updateDir (int dir, int change);
};

#endif
