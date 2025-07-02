#include "property.h"

double getRatio(PropertyType attack, PropertyType defense) {
    const double table[4][4] = {
        {1.0, 1.0, 1.0, 1.0},
        {1.0, 0.5, 0.5, 2.0},
        {1.0, 2.0, 0.5, 0.5},
        {1.0, 0.5, 2.0, 0.5}
    };
    return table[attack][defense];
}