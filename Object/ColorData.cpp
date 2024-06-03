#include "ColorData.h"

void ColorData::ChangeColor(ColorData* cd)
{
    int tmp;

    tmp = color;
    color = cd->GetColorData();
    cd->SetColorData(tmp);
}
