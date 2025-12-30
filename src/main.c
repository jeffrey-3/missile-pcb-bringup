#include "vehicle.h"

int main(void) {
    vehicle_init();

    for (;;) {
        vehicle_update();
    }

    return 0;
}
