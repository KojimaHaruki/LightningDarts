#pragma once
struct sKey {
    static constexpr int RELEASED = -2;
    static constexpr int PRESSEDtoRELEASED = -1;
    static constexpr int NO_SIGNAL = 0;
    static constexpr int RELEASEDtoPRESSED = 1;
    static constexpr int PRESSED = 2;
};
