void lineFollowing () {
    lineFollowing (70);
}

void lineFollowing (int spd) {
    lineFollowing (0.1, 0.07, spd, 40);
}

double error = 0, P = 0, D = 0, previousError = 0, PID = 0;
int lsp, rsp;

void lineFollowing (double kP, double kD, int spd, int var) {

    previousError = error;
    
    if (!scaleSensors) error = (analogRead (nLeft) - threshold [2][nLeft]) - (analogRead (nRight) - threshold [2][nRight]);
    else error = sensorRead (nLeft) - sensorRead (nRight);
    
    P = error;
    D = error - previousError;
    PID = (P * kP) + (D * kD);

    lsp = (int)(spd + PID);
    rsp = (int)(spd - PID);

    if (lsp < (spd - var)) lsp = (spd - var);
    else if (lsp > (spd + var)) lsp = (spd + var);
    if (rsp < (spd - var)) rsp = (spd - var);
    else if (rsp > (spd + var)) rsp = (spd + var);

    if (cDir == 0) motors (rsp, rsp, lsp, lsp);
    else if (cDir == 1) motors (rsp, -lsp, lsp, -rsp);
    else if (cDir == 2) motors (-lsp, -lsp, -rsp, -rsp);
    else if (cDir == 3) motors (-lsp, rsp, -rsp, lsp);

}
