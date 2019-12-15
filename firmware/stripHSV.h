typedef struct HSVdata {
  byte h, s, v;
  // default values are UNINITIALIZED
  inline HSVdata() __attribute__((always_inline))
  {
  }
  
  // allow construction from 32-bit (really 24-bit) bit 0xHHSSVV color code
  inline HSVdata(uint32_t colorcode)  __attribute__((always_inline))
  : h((colorcode >> 16) & 0xFF), s((colorcode >> 8) & 0xFF), v((colorcode >> 0) & 0xFF)
  {
  }
  
  inline HSVdata& operator= (const uint32_t colorcode) __attribute__((always_inline))
  {
    h = (colorcode >> 16) & 0xFF;
    s = (colorcode >>  8) & 0xFF;
    v=  (colorcode >>  0) & 0xFF;
    return *this;
  }
};
inline __attribute__((always_inline)) bool operator== (const HSVdata& lhsv, const HSVdata& rhsv) {
  return (lhsv.h == rhsv.h) && (lhsv.s == rhsv.s) && (lhsv.v == rhsv.v);
}

HSVdata RgbToHsv(LEDdata rgb)
{
    HSVdata hsv;
    uint8_t rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * ((long)(rgbMax - rgbMin)) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}
