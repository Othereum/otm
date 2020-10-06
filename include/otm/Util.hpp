#pragma once
#include "Angle.hpp"
#include "Vector.hpp"

namespace otm
{
template <class Float>
Angle<RadR> V2HFov(Angle<RadR> vfov, Vector<Float, 2> scr)
{
    return 2 * Atan(Tan(vfov / 2) * (scr[0] / scr[1]));
}

template <class Float>
Angle<RadR> H2VFov(Angle<RadR> hfov, Vector<Float, 2> scr)
{
    return 2 * Atan(Tan(hfov / 2) * (scr[1] / scr[0]));
}
} // namespace otm