
#include <device/device.h>
#include <cmath>

namespace wtflights {
namespace device {

// algorithm from http://www.cs.rit.edu/~ncs/color/t_convert.html
ColorRGB colorFromHSV(float h, float s, float v )
{
    ColorRGB ret = {0.0f};
    int i;
    float f, p, q, t;
    if( s == 0 ) {
        // achromatic (grey)
        ret.r = ret.g = ret.b = v;
        return ret;
    }
    h /= 60;			// sector 0 to 5
    i = floorf( h );
    f = h - i;			// factorial part of h
    p = v * ( 1 - s );
    q = v * ( 1 - s * f );
    t = v * ( 1 - s * ( 1 - f ) );
    switch( i ) {
        case 0:
            ret.r = v;
            ret.g = t;
            ret.b = p;
            break;
        case 1:
            ret.r = q;
            ret.g = v;
            ret.b = p;
            break;
        case 2:
            ret.r = p;
            ret.g = v;
            ret.b = t;
            break;
        case 3:
            ret.r = p;
            ret.g = q;
            ret.b = v;
            break;
        case 4:
            ret.r = t;
            ret.g = p;
            ret.b = v;
            break;
        default:		// case 5:
            ret.r = v;
            ret.g = p;
            ret.b = q;
            break;
    }
    return ret;
}

}
}
