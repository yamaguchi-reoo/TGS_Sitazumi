#include "ColorData.h"

void ColorData::ChangeColor(ColorData cd)
{
    int tmp;

    tmp = color;
    color = cd.GetColerData();
    cd.SetColorData(tmp);

}
