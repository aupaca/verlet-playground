#include "vec.h"
#include <cmath>

glm::vec4 normalRgbFromHex(int hexColor)
{
    float r = (hexColor >> 16) & 0xff;
    float g = (hexColor >> 8) & 0xff;
    float b = hexColor & 0xff;
    return {r/255.f, g/255.f, b/255.f, 1.f};
}
