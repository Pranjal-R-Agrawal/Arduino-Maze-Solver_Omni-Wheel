void decelerate (int start, int decrement) {
    for (int spd = start; spd >= 0; spd -= decrement) {
        omni (spd);
    }
}

void brake (int x) {
    omni (0);
    delay (x);
}

void cbrake (int x) {
    omni (-20);
    delay (x);
}
