int currentColorValueRed;
int currentColorValueGreen;
int currentColorValueBlue;
int colorData;
int brightnessValue;

void initColorCtl() {
  pinMode(ColorPotPin, INPUT);
}

uint32_t refreshColorCtl() {
  
    int potPinValue = map(analogRead(ColorPotPin), 0, 1024, 0, 255);
    float brightnessValue = 1;

    if (potPinValue <= 20) {
    currentColorValueRed = 255;
    currentColorValueGreen = 255;
    currentColorValueBlue = 255;
   }
   
    if ((potPinValue > 20) && (potPinValue <= 42.5)) {
    currentColorValueRed = 255;
    currentColorValueGreen = potPinValue * 6;
    currentColorValueBlue = 0;
   }
   
   if ((potPinValue > 42.5) && (potPinValue <= 85)) {
    currentColorValueRed = 255 - (potPinValue - 43) * 6;
    currentColorValueGreen = 255;
    currentColorValueBlue = 0;
   }
   
   if ((potPinValue > 85) && (potPinValue <= 127.5)) {
    currentColorValueRed = 0;
    currentColorValueGreen = 255;
    currentColorValueBlue = (potPinValue - 85) * 6;   }
   
  if ((potPinValue > 127.5) && (potPinValue <= 170)) {
    currentColorValueRed = 0;
    currentColorValueGreen = 255 - (potPinValue - 127.5) * 6;
    currentColorValueBlue = 255;
  }
   
  if ((potPinValue > 170) && (potPinValue <= 212.5)) {
    currentColorValueRed = (potPinValue - 170) * 6;
    currentColorValueGreen = 0;
    currentColorValueBlue = 255;
  }
  
  if ((potPinValue > 212.5) && (potPinValue <= 255)) {
    currentColorValueRed = 255;
    currentColorValueGreen = 0;
    currentColorValueBlue = 255 - (potPinValue - 212.5) * 6;
  }
    
    uint32_t high = strip.Color(currentColorValueRed * brightnessValue, currentColorValueGreen * brightnessValue, currentColorValueBlue * brightnessValue);
    return high;
}
