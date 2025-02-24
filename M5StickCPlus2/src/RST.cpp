#include "M5StickCPlus2.h"

void button_RST()
{
    M5.update();  // Update button states
 
    if (M5.BtnB.wasPressed())
    {
        ESP.restart();
    }
    delay(10);
}
