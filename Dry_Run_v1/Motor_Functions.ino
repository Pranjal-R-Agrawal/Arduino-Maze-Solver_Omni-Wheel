void motors (int spd1, int spd2, int spd3, int spd4) {

    digitalWrite(STBY, HIGH);

    // motor 1 (top right)
    analogWrite(pwm1, adjust (abs (spd1)));
    digitalWrite(m1a, (spd1 >= 0) ? HIGH : LOW);
    digitalWrite(m1b, (spd1 >= 0) ? LOW : HIGH);

    // motor 2 (bottom right)
    analogWrite(pwm2, adjust (abs (spd2)));
    digitalWrite(m2a, (spd2 >= 0) ? HIGH : LOW);
    digitalWrite(m2b, (spd2 >= 0) ? LOW : HIGH);

    // motor 3 (bottom left)
    analogWrite(pwm3, adjust (abs (spd3)));
    digitalWrite(m3a, (spd3 >= 0) ? HIGH : LOW);
    digitalWrite(m3b, (spd3 >= 0) ? LOW : HIGH);

    // motor 4 (top left)
    analogWrite(pwm4, adjust (abs (spd4)));
    digitalWrite(m4a, (spd4 >= 0) ? HIGH : LOW);
    digitalWrite(m4b, (spd4 >= 0) ? LOW : HIGH);

}

void omni (int spd) {
    omni (cDir, spd);
}

void omni (int dir, int spd) {
    if (dir == 0) motors (spd, spd, spd, spd);
    else if (dir == 1) motors (spd, -spd, spd, -spd);
    else if (dir == 2) motors (-spd, -spd, -spd, -spd);
    else if (dir == 3) motors (-spd, spd, -spd, spd);
}

int adjust (double spd) {
    if (adjustSpeed) {
        double percent = (optimumVoltage - currentVoltage) / optimumVoltage * constant;
        return (spd * (1 + percent));
    }
    else return ((int) spd);
}
