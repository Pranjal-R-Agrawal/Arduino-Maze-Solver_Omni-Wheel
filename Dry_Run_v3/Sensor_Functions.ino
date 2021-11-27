void calibrate (int spd, int duration) {
    for (int i = 0; i < 16; i++) {
        threshold [0][i] = 1024;
        threshold [1][i] = 0;
    }

    wait (button2);
    delay (500);

    int timer = millis ();
    omni (spd);

    while ((millis () - timer) <= duration) {
        for (int i = 0; i < 16; i++) {
            if (analogRead (i) < threshold [0][i]) threshold [0][i] = analogRead (i);
            if (analogRead (i) > threshold [1][i]) threshold [1][i] = analogRead (i);
            threshold [2][i] = (threshold [0][i] + threshold [1][i]) / 2;
        }
    }

    brake (200);

    if (debug) {
        for (int i = 0; i < 16; i++) {
            Serial.print (threshold [0][i]);
            Serial.print (", ");
            Serial.print (threshold [2][i]);
            Serial.print (", ");
            Serial.println (threshold [1][i]);
        }
    }
}

void changeDir (int change) {
    cDir = updateDir (cDir, change);
    lDir = updateDir (cDir, +1);
    rDir = updateDir (cDir, -1);
    uDir = updateDir (cDir, -2);

    nTop = cDir * 4; nRight = nTop + 1; nCenter = nTop + 2; nLeft = nTop + 3;
    eRight = rDir * 4; eBottom = eRight + 1; eCenter = eRight + 2; eTop = eRight + 3;
    wLeft = lDir * 4; wTop = wLeft + 1; wCenter = wLeft + 2; wBottom = wLeft + 3;
    sBottom = uDir * 4; sLeft = sBottom + 1; sCenter = sBottom + 2; sRight = sBottom + 3;

    if (debug) printDir ();
}

int updateDir (int dir, int change) {
    dir = (dir + change) % 4;
    if (dir < 0) dir = 4 + dir;
    return dir;
}

bool onLine (int sensor) {
    if (!scaleSensors) return (analogRead (sensor) < threshold [2][sensor]);
    else return (sensorRead (sensor) < 500);
}

void printDir () {
    Serial.print ((String)"Current Direction = " + (String)cDir + (String)", ");
    Serial.print ((String)"Left Direction = " + (String)lDir + (String)", ");
    Serial.print ((String)"Right Direction = " + (String)rDir + (String)", ");
    Serial.println ((String)"Reverse Direction = " + (String)uDir);
}

int sensorRead (int sensor) {
    return (((double)(analogRead (sensor) - threshold [0][sensor]) / (double)(threshold [1][sensor] - threshold [0][sensor])) * 1000);
}

void printSensors () {
    for (int i = 0; i < 16; i++) {
        Serial.print ((String) sensorRead (i) + (String)", ");
    }
    Serial.println();
    delay (100);
}
