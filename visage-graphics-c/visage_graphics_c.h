#ifndef VISAGE_C_WRAPPER_H
#define VISAGE_C_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#include <stdint.h>
#define nullptr NULL
#endif

#define VISAGE_MAX(a,b) ((a) > (b) ? (a) : (b))

// -- Renderer -------------------------------------------------------------------------------------

void VisageRenderer_checkInitialization(void* model_window, void* display);

//void setScreenshotData(const uint8_t* data, int width, int height, int pitch, bool blue_red);
//const Screenshot& screenshot() const { return screenshot_; }
//const std::string& errorMessage() const { return error_message_; }
bool VisageRenderer_supported();
bool VisageRenderer_swapChainSupported();
bool VisageRenderer_initialized();

// -- Color ----------------------------------------------------------------------------------------

typedef enum VisageColorChannel {
    VisageColorChannelBlue,
    VisageColorChannelGreen,
    VisageColorChannelRed,
    VisageColorChannelAlpha,
    VisageNumColorChannels,
} VisageColorChannel;

typedef struct VisageColor {
    float values [VisageNumColorChannels];
    float hdr;
} VisageColor;

VisageColor VisageColor_fromAHSV(float alpha, float hue, float saturation, float value);
VisageColor VisageColor_fromABGR(unsigned int abgr);
VisageColor VisageColor_fromARGB(unsigned int argb);
VisageColor VisageColor_fromHexString(const char* str);
unsigned int VisageColor_toABGR(const VisageColor* color);
unsigned int VisageColor_toARGB(const VisageColor* color);
unsigned int VisageColor_toRGB(const VisageColor* color);
inline int VisageColor_compare(const VisageColor* a, const VisageColor* b) {
    for (int i = 0; i < VisageNumColorChannels; ++i) {
        if (a->values[i] < b->values[i])
          return -1;
        if (a->values[i] > b->values[i])
          return 1;
      }
      if (a->hdr < b->hdr)
        return -1;
      if (a->hdr > b->hdr)
        return 1;
      return 0;
}
inline float VisageColor_value(const VisageColor* color) {
    float max_bg = VISAGE_MAX(color->values[VisageColorChannelBlue], color->values[VisageColorChannelGreen]);
    return VISAGE_MAX(max_bg, color->values[VisageColorChannelRed]);
}
float VisageColor_saturation(const VisageColor* color);
float VisageColor_hue(const VisageColor* color);
inline VisageColor VisageColor_interpolate(const VisageColor* a, const VisageColor* b, float t) {
    VisageColor result;
    for (int i = 0; i < VisageNumColorChannels; ++i) {
        result.values[i] = a->values[i] + (b->values[i] - a->values[i]) * t;
    }
    result.hdr = a->hdr + (b->hdr - a->hdr) * t;
    return result;
}

// -- Canvas ---------------------------------------------------------------------------------------

struct VisageCanvas_t;
typedef struct VisageCanvas_t VisageCanvas;

VisageCanvas* VisageCanvas_new();
void VisageCanvas_destroy(VisageCanvas* canvas);

void VisageCanvas_pairToWindow(VisageCanvas* canvas, void* window_handle, int width, int height);

void VisageCanvas_submit(VisageCanvas* canvas, int submit_pass);

void VisageCanvas_setColor(VisageCanvas* canvas, VisageColor color);
void VisageCanvas_fill(VisageCanvas* canvas, float x, float y, float width, float height);
void VisageCanvas_circle(VisageCanvas* canvas, float x, float y, float width);


#ifdef __cplusplus
}
#endif

#endif /* VISAGE_C_WRAPPER_H */