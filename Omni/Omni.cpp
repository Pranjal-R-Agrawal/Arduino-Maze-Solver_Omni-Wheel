#include "Arduino.h"
#include "Omni.h"

#define STBY 13

#define pwm1 5
#define m1a 22
#define m1b 23

#define pwm2 6
#define m2a 24
#define m2b 25

#define pwm3 7
#define m3a 26
#define m3b 27

#define pwm4 8
#define m4a 28
#define m4b 29

int motorPins[4][3] = {{pwm1, m1a, m1b}, {pwm2, m2a, m2b}, {pwm3, m3a, m3b}, {pwm4, m4a, m4b}};

Omni :: Omni (bool adjustSpeed, double optimumVoltage, double currentVoltage, double constant) {
    pinMode (STBY, OUTPUT);
    for (int i = 0; i < 4; i++) for (int j = 0; j < 3; j++) pinMode (motorPins [i][j], OUTPUT);

    analogWrite(pwm1, 0);
    analogWrite(pwm2, 0);
    analogWrite(pwm3, 0);
    analogWrite(pwm4, 0);

    setDir (0);

    _adjustSpeed = adjustSpeed;
    _optimumVoltage = optimumVoltage;
    _currentVoltage = currentVoltage;
    _constant = constant;
}

void Omni :: motors (int spd1, int spd2, int spd3, int spd4) {

    digitalWrite(STBY, HIGH);

    // motor 1 (top right)
    analogWrite(pwm1, _adjust (abs (spd1)));
    digitalWrite(m1a, (spd1 >= 0) ? HIGH : LOW);
    digitalWrite(m1b, (spd1 >= 0) ? LOW : HIGH);

    // motor 2 (bottom right)
    analogWrite(pwm2, _adjust (abs (spd2)));
    digitalWrite(m2a, (spd2 >= 0) ? HIGH : LOW);
    digitalWrite(m2b, (spd2 >= 0) ? LOW : HIGH);

    // motor 3 (bottom left)
    analogWrite(pwm3, _adjust (abs (spd3)));
    digitalWrite(m3a, (spd3 >= 0) ? HIGH : LOW);
    digitalWrite(m3b, (spd3 >= 0) ? LOW : HIGH);

    // motor 4 (top left)
    analogWrite(pwm4, _adjust (abs (spd4)));
    digitalWrite(m4a, (spd4 >= 0) ? HIGH : LOW);
    digitalWrite(m4b, (spd4 >= 0) ? LOW : HIGH);

}

void Omni :: goStraight (int spd) {
    goStraight (cDir, spd);
}

void Omni :: goStraight (int dir, int spd) {
    if (dir == 0) motors (spd, spd, spd, spd);
    else if (dir == 1) motors (spd, -spd, spd, -spd);
    else if (dir == 2) motors (-spd, -spd, -spd, -spd);
    else if (dir == 3) motors (-spd, spd, -spd, spd);
}

int Omni :: _adjust (double spd) {
    if (_adjustSpeed) {
        double percent = (_optimumVoltage - _currentVoltage) / _optimumVoltage * _constant;
        return ((int)(spd * (1 + percent)));
    }
    else return ((int) spd);
}

void Omni :: changeDir (int change) {
    cDir = _updateDir (cDir, change);
    lDir = _updateDir (cDir, +1);
    rDir = _updateDir (cDir, -1);
    uDir = _updateDir (cDir, -2);

    nTop = cDir * 4; nRight = nTop + 1; nCenter = nTop + 2; nLeft = nTop + 3;
    eRight = rDir * 4; eBottom = eRight + 1; eCenter = eRight + 2; eTop = eRight + 3;
    wLeft = lDir * 4; wTop = wLeft + 1; wCenter = wLeft + 2; wBottom = wLeft + 3;
    sBottom = uDir * 4; sLeft = sBottom + 1; sCenter = sBottom + 2; sRight = sBottom + 3;

    //if (debug) printDir ();
}

int Omni :: _updateDir (int dir, int change) {
    dir = (dir + change) % 4;
    if (dir < 0) dir = 4 + dir;
    return dir;
}

void Omni :: setDir (int dir) {
    cDir = dir;
    changeDir (0);
}

void Omni :: decelerate (int start, int decrement) {
    for (int spd = start; spd >= 0; spd -= decrement) {
        goStraight (spd);
        if (spd < decrement) spd = decrement;
    }
    goStraight (0);
}

void Omni :: brake (int x) {
    goStraight (0);
    delay (x);
}
