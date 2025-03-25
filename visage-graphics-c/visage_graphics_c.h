#ifndef VISAGE_C_WRAPPER_H
#define VISAGE_C_WRAPPER_H

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdbool.h>
#include <stdint.h>
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

void VisageColor_fromAHSV_inner(float alpha, float hue, float saturation, float value, VisageColor* returnValue);
void VisageColor_fromABGR_inner(uint32_t abgr, VisageColor* returnValue);
void VisageColor_fromARGB_inner(uint32_t argb, VisageColor* returnValue);
void VisageColor_fromHexString_inner(const char* str, VisageColor* returnValue);

static inline VisageColor VisageColor_fromAHSV(float alpha, float hue, float saturation, float value) {
    VisageColor color;
    VisageColor_fromAHSV_inner(alpha, hue, saturation, value, &color);
    return color;
}
static inline VisageColor VisageColor_fromABGR(uint32_t abgr) {
  VisageColor color;
  VisageColor_fromABGR_inner(abgr, &color);
  return color;
}
static inline VisageColor VisageColor_fromARGB(uint32_t argb) {
  VisageColor color;
  VisageColor_fromARGB_inner(argb, &color);
  return color;
}
static inline VisageColor VisageColor_fromHexString(const char* str) {
  VisageColor color;
  VisageColor_fromHexString_inner(str, &color);
  return color;
}

uint32_t VisageColor_toABGR(const VisageColor* color);
uint32_t VisageColor_toARGB(const VisageColor* color);
uint32_t VisageColor_toRGB(const VisageColor* color);
static inline int VisageColor_compare(const VisageColor* a, const VisageColor* b) {
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
static inline float VisageColor_value(const VisageColor* color) {
    float max_bg = VISAGE_MAX(color->values[VisageColorChannelBlue], color->values[VisageColorChannelGreen]);
    return VISAGE_MAX(max_bg, color->values[VisageColorChannelRed]);
}
float VisageColor_saturation(const VisageColor* color);
float VisageColor_hue(const VisageColor* color);
static inline VisageColor VisageColor_interpolate(const VisageColor* a, const VisageColor* b, float t) {
    VisageColor result;
    for (int i = 0; i < VisageNumColorChannels; ++i) {
        result.values[i] = a->values[i] + (b->values[i] - a->values[i]) * t;
    }
    result.hdr = a->hdr + (b->hdr - a->hdr) * t;
    return result;
}

// -- Gradient -------------------------------------------------------------------------------------

struct VisageGradient_t;
typedef struct VisageGradient_t VisageGradient;

VisageGradient* VisageGradient_new();
VisageGradient* VisageGradient_clone(const VisageGradient* gradient);
VisageGradient* VisageGradient_fromSampleFunction(int32_t resolution, void (*sample_function)(float, VisageColor*));
void VisageGradient_delete(VisageGradient* gradient);

int32_t VisageGradient_getResolution(const VisageGradient* gradient);
void VisageGradient_setResolution(VisageGradient* gradient, int32_t resolution);
void VisageGradient_getColor(const VisageGradient* gradient, int32_t index, VisageColor* returnValue);
void VisageGradient_setColor(VisageGradient* gradient, int32_t index, VisageColor color);
void VisageGradient_interpolateWith(VisageGradient* gradient, const VisageGradient* other, float t);
void VisageGradient_sample(const VisageGradient* gradient, float t, VisageColor* returnValue);
void VisageGradient_multiplyAlpha(VisageGradient* gradient, float mult);
int32_t VisageGradient_compare(const VisageGradient* a, const VisageGradient* b);

// -- Brush ----------------------------------------------------------------------------------------

struct VisageBrush_t;
typedef struct VisageBrush_t VisageBrush;

VisageBrush* VisageBrush_new();
VisageBrush* VisageBrush_clone(const VisageBrush* brush);
void VisageBrush_delete(VisageBrush* brush);

void VisageBrush_solid(VisageBrush* brush, const VisageColor* color);
void VisageBrush_horizontal(VisageBrush* brush, const VisageGradient* gradient);
void VisageBrush_horizontalFromTwo(VisageBrush* brush, VisageColor left, VisageColor right);
void VisageBrush_vertical(VisageBrush* brush, const VisageGradient* gradient);
void VisageBrush_verticalFromTwo(VisageBrush* brush, VisageColor top, VisageColor bottom);
void VisageBrush_linear(VisageBrush* brush, const VisageGradient* gradient, float from_x, float from_y, float to_x, float to_y);
void VisageBrush_linearFromTwo(VisageBrush* brush, VisageColor from_color, VisageColor to_color, float from_x, float from_y, float to_x, float to_y);
void VisageBrush_interpolateWith(VisageBrush* brush, const VisageBrush* other, float t);
void VisageBrush_multiplyAlpha(VisageBrush* brush, float t);

// -- Canvas ---------------------------------------------------------------------------------------

struct VisageCanvas_t;
typedef struct VisageCanvas_t VisageCanvas;

VisageCanvas* VisageCanvas_new();
void VisageCanvas_destroy(VisageCanvas* canvas);

void VisageCanvas_pairToWindow(VisageCanvas* canvas, void* window_handle, int32_t width, int32_t height);
void VisageCanvas_setDimensions(VisageCanvas* canvas, int32_t width, int32_t height);
void VisageCanvas_setDpiScale(VisageCanvas* canvas, float scale);
void VisageCanvas_setNativePixelScale(VisageCanvas* canvas);
void VisageCanvas_setLogicalPixelScal(VisageCanvas* canvas);
void VisageCanvas_clearDrawnShapes(VisageCanvas* canvas);
void VisageCanvas_submit(VisageCanvas* canvas, int32_t submit_pass);
void VisageCanvas_updateTime(VisageCanvas* canvas, double time);
void VisageCanvas_setWindowless(VisageCanvas* canvas, int32_t width, int32_t height);
void VisageCanvas_removeFromWindow(VisageCanvas* canvas);
void VisageCanvas_requestScreenshot(VisageCanvas* canvas);

float VisageCanvas_dpiScale(VisageCanvas* canvas);
double VisageCanvas_time(VisageCanvas* canvas);
double VisageCanvas_deltaTime(VisageCanvas* canvas);
int32_t VisageCanvas_frameCount(VisageCanvas* canvas);

void VisageCanvas_setColor(VisageCanvas* canvas, VisageColor color);
void VisageCanvas_setBrush(VisageCanvas* canvas, const VisageBrush* brush);
void VisageCanvas_fill(VisageCanvas* canvas, float x, float y, float width, float height);
void VisageCanvas_circle(VisageCanvas* canvas, float x, float y, float width);

#ifdef __cplusplus
}
#endif

#endif /* VISAGE_C_WRAPPER_H */