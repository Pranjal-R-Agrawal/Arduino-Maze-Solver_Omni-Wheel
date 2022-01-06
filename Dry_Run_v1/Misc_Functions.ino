void wait (int button) {
    while (digitalRead (button) == 1) {}
}

void checkDirectionFunction () {
    cDir = 0;
    for (int i = 0; i < 8; i++) {
        changeDir (1);
    }
    Serial.println ("\n-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x\n");
    cDir = 3;
    for (int i = 0; i < 8; i++) {
        changeDir (-1);
    }
    while (true) {}
}

void oldCode () {
    while ((onLine (nCenter) || onLine (nLeft) || onLine (nRight)) && !onLine (wTop) && !onLine (eTop)) {
        lineFollowing (0.05, 0.0, 70, 40);
    }
    if (onLine(wTop) || onLine (wBottom) || onLine (wCenter) || onLine (wLeft)) {
        changeDir (1);
    }
    else if (onLine(nTop) || onLine (nRight) || onLine (wCenter) || onLine (wLeft)) {
        changeDir (0);
    }
    else if (onLine(eTop) || onLine (eBottom) || onLine (eCenter) || onLine (eRight)) {
        changeDir (-1);
    }
    else {
        changeDir (2);
    }
    decelerate (60, 2);
    brake (200);
    float spd1 = 30;
    long timerXYZ = millis ();
    while ((millis () - timerXYZ) <= 400) {
        digitalWrite (20, HIGH);
        spd1 += 1;
        if (spd1 >= 70) spd1 = 70;
        lineFollowing (0.05, 0.0, (int)spd1, 40);
    }
    digitalWrite (20, LOW);
}
