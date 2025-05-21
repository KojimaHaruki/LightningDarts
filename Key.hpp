#pragma once
struct Key {
    static constexpr int RELEASED = -2;
    static constexpr int PRESSEDtoRELEASED = -1;
    static constexpr int NO_SIGNAL = 0;
    static constexpr int RELEASEDtoPRESSED = 1;
    static constexpr int PRESSED = 2;
    int pressCount = 0; // positive: pressed, negative: released
    int pressState = NO_SIGNAL; // -2: released->released, -1: pressed->released, 1: released->pressed, 2: pressed -> pressed
};
