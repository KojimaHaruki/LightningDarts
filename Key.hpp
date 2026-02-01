#pragma once
struct sKeyState {
    static constexpr int RELEASE = -2;
    static constexpr int PRESStoRELEASE = -1;
    static constexpr int NO_SIGNAL = 0;
    static constexpr int RELEASEtoPRESS = 1;
    static constexpr int PRESS = 2;
};
