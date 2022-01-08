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
    goAbsDir (_cDir, spd);
}

void Omni :: goAbsDir (int dir, int spd) {
    if (dir == 0) motors (spd, spd, spd, spd);
    else if (dir == 1) motors (spd, -spd, spd, -spd);
    else if (dir == 2) motors (-spd, -spd, -spd, -spd);
    else if (dir == 3) motors (-spd, spd, -spd, spd);
}

void Omni :: goRelDir (int dir, int spd) {
    goAbsDir (_updateDir (_cDir, dir), spd);
}

void Omni :: move (int spdL, int spdR) {
    if (_cDir == 0) motors (spdR, spdR, spdL, spdL);
    else if (_cDir == 1) motors (spdR, -spdL, spdL, -spdR);
    else if (_cDir == 2) motors (-spdL, -spdL, -spdR, -spdR);
    else if (_cDir == 3) motors (-spdL, spdR, -spdR, spdL);
}

void Omni :: rotate (char dir, int spd) {
    int spdL = (dir == 'l')? (-spd) : spd;
    int spdR = -spdL;
    move (spdL, spdR);
}

int Omni :: _adjust (double spd) {
    if (_adjustSpeed) {
        double percent = (_optimumVoltage - _currentVoltage) / _optimumVoltage * _constant;
        return ((int)(spd * (1 + percent)));
    }
    else return ((int) spd);
}

void Omni :: changeDir (int change) {
    _cDir = _updateDir (_cDir, change);
    _rDir = _updateDir (_cDir, -1);
    _lDir = _updateDir (_cDir, +1);
    _uDir = _updateDir (_cDir, -2);

    nTop = _cDir * 4; nRight = nTop + 1; nCenter = nTop + 2; nLeft = nTop + 3;
    eRight = _rDir * 4; eBottom = eRight + 1; eCenter = eRight + 2; eTop = eRight + 3;
    wLeft = _lDir * 4; wTop = wLeft + 1; wCenter = wLeft + 2; wBottom = wLeft + 3;
    sBottom = _uDir * 4; sLeft = sBottom + 1; sCenter = sBottom + 2; sRight = sBottom + 3;
}

int Omni :: _updateDir (int dir, int change) {
    dir = (dir + change) % 4;
    if (dir < 0) dir = 4 + dir;
    return dir;
}

void Omni :: setDir (int dir) {
    _cDir = dir;
    changeDir (0);
}

int Omni :: getDir (char dir) {
    if (dir == 'c') return (_cDir);
    else if (dir == 'r') return (_rDir);
    else if (dir == 'l') return (_lDir);
    else if (dir == 'u') return (_uDir);
}

void Omni :: decelerate (int initialSpeed, int finalSpeed, int decrement) {
    for (int spd = initialSpeed; spd >= finalSpeed; spd -= decrement) goStraight (spd);
    goStraight (finalSpeed);
}

void Omni :: brake (int x) {
    goStraight (0);
    delay (x);
}
