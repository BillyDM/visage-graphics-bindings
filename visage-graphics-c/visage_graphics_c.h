#ifndef VISAGE_C_WRAPPER_H
#define VISAGE_C_WRAPPER_H

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdbool.h>
#include <stdint.h>
#define char32_t uint32_t
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
VisageGradient* VisageGradient_copy(const VisageGradient* gradient);
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
VisageBrush* VisageBrush_copy(const VisageBrush* brush);
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

// -- Line -----------------------------------------------------------------------------------------

struct VisageLine_t;
typedef struct VisageLine_t VisageLine;

VisageLine* VisageLine_new(int32_t points);
VisageLine* VisageLine_copy(const VisageLine* line);
void VisageLine_delete(VisageLine* line);

int32_t VisageLine_getNumPoints(const VisageLine* line);
void VisageLine_setNumPoints(VisageLine* line, int32_t points);
float VisageLine_getLineValueScale(const VisageLine* line);
void VisageLine_setLineValueScale(VisageLine* line, float line_value_scale);
float VisageLine_getFillValueScale(const VisageLine* line);
void VisageLine_setFillValueScale(VisageLine* line, float fill_value_scale);
// Returns a pointer to the list of x values.
// If the number of points in this line is 0, then this will return a null pointer.
// Changing the number of points may invalidate this pointer.
float* VisageLine_xValues(VisageLine* line);
// Returns a pointer to the list of y values.
// If the number of points in this line is 0, then this will return a null pointer.
// Changing the number of points may invalidate this pointer.
float* VisageLine_yValues(VisageLine* line);
// Returns a pointer to the list of values.
// If the number of points in this line is 0, then this will return a null pointer.
// Changing the number of points may invalidate this pointer.
float* VisageLine_values(VisageLine* line);

// -- Font -----------------------------------------------------------------------------------------

enum VisageJustification {
  kVisageJustificationCenter = 0,
  kVisageJustificationLeft = 0x1,
  kVisageJustificationRight = 0x2,
  kVisageJustificationTop = 0x10,
  kVisageJustificationBottom = 0x20,
  kVisageJustificationTopLeft = kVisageJustificationTop | kVisageJustificationLeft,
  kVisageJustificationBottomLeft = kVisageJustificationBottom | kVisageJustificationLeft,
  kVisageJustificationTopRight = kVisageJustificationTop | kVisageJustificationRight,
  kVisageJustificationBottomRight = kVisageJustificationBottom | kVisageJustificationRight,
};

struct VisageFont_t;
typedef struct VisageFont_t VisageFont;

VisageFont* VisageFont_new(float size, const char* font_data, int32_t data_size, float dpi_scale);
VisageFont* VisageFont_copy(const VisageFont* font);
VisageFont* VisageFont_withDpiScale(const VisageFont* font, float dpi_scale);
void VisageFont_delete(VisageFont* font);

float VisageFont_getDpiScale(const VisageFont* font);
int32_t VisageFont_widthOverflowIndex(const VisageFont* font, const char32_t* string, int32_t string_length, float width, bool round, int32_t character_override);
int32_t VisageFont_lineBreaks(const VisageFont* font, const char32_t* string, int32_t string_length, float width, int* line_breaks, int32_t line_breaks_length);
float VisageFont_stringWidth(const VisageFont* font, const char32_t* string, int32_t string_length, int32_t character_override);
float VisageFont_lineHeight(const VisageFont* font);
float VisageFont_capitalHeight(const VisageFont* font);
float VisageFont_lowerDipHeight(const VisageFont* font);
float VisageFont_size(const VisageFont* font);
const char* VisageFont_fontData(const VisageFont* font, int32_t* data_size);

// -- Text -----------------------------------------------------------------------------------------

struct VisageText_t;
typedef struct VisageText_t VisageText;

VisageText* VisageText_new();
VisageText* VisageText_copy(const VisageText* text);
void VisageText_delete(VisageText* text);

void VisageText_setText(VisageText* text, const char* s);
void VisageText_setTextWithLength(VisageText* text, const char* s, int32_t length);
void VisageText_setTextU32(VisageText* text, const char32_t* s);
void VisageText_setTextU32WithLength(VisageText* text, const char32_t* s, int32_t length);
// `string_length` returns the number of characters in the string, not including any null-termination.
const char32_t* VisageText_getTextU32(const VisageText* text, int32_t* string_length);

void VisageText_setFont(VisageText* text, const VisageFont* font);
const VisageFont* VisageText_getFont(const VisageText* text);

void VisageText_setJustification(VisageText* text, int32_t justification);
int32_t VisageText_getJustification(const VisageText* text);

void VisageText_setMultiLine(VisageText* text, bool multi_line);
bool VisageText_getMultiLine(const VisageText* text);

void VisageText_setCharacterOverride(VisageText* text, int32_t character);
int32_t VisageText_getCharacterOverride(const VisageText* text);

// -- Canvas ---------------------------------------------------------------------------------------

enum Direction {
  Left = 0,
  Up,
  Right,
  Down,
};

struct VisageCanvas_t;
typedef struct VisageCanvas_t VisageCanvas;

VisageCanvas* VisageCanvas_new();
void VisageCanvas_destroy(VisageCanvas* canvas);

void VisageCanvas_pairToWindow(VisageCanvas* canvas, void* window_handle, int32_t width, int32_t height);
void VisageCanvas_setDimensions(VisageCanvas* canvas, int32_t width, int32_t height);
void VisageCanvas_setDpiScale(VisageCanvas* canvas, float scale);
void VisageCanvas_setNativePixelScale(VisageCanvas* canvas);
void VisageCanvas_setLogicalPixelScale(VisageCanvas* canvas);
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
void VisageCanvas_fadeCircle(VisageCanvas* canvas, float x, float y, float width, float pixel_width);
void VisageCanvas_ring(VisageCanvas* canvas, float x, float y, float width, float thickness);
void VisageCanvas_squircle(VisageCanvas* canvas, float x, float y, float width, float power);
void VisageCanvas_squircleBorder(VisageCanvas* canvas, float x, float y, float width, float power, float thickness);
void VisageCanvas_superEllipse(VisageCanvas* canvas, float x, float y, float width, float height, float power);
void VisageCanvas_roundedArc(VisageCanvas* canvas, float x, float y, float width, float thickness, float center_radians, float radians);
void VisageCanvas_flatArc(VisageCanvas* canvas, float x, float y, float width, float thickness, float center_radians, float radians);
void VisageCanvas_arc(VisageCanvas* canvas, float x, float y, float width, float thickness, float center_radians, float radians, bool rounded);
void VisageCanvas_roundedArcShadow(VisageCanvas* canvas, float x, float y, float width, float thickness, float center_radians, float radians, float shadow_width);
void VisageCanvas_flatArcShadow(VisageCanvas* canvas, float x, float y, float width, float thickness, float center_radians, float radians, float shadow_width);
void VisageCanvas_segment(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float thickness, bool rounded);
void VisageCanvas_quadratic(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float c_x, float c_y, float thickness);
void VisageCanvas_rectangle(VisageCanvas* canvas, float x, float y, float width, float height);
void VisageCanvas_rectangleBorder(VisageCanvas* canvas, float x, float y, float width, float height, float thickness);
void VisageCanvas_roundedRectangle(VisageCanvas* canvas, float x, float y, float width, float height, float rounding);
void VisageCanvas_diamond(VisageCanvas* canvas, float x, float y, float width, float rounding);
void VisageCanvas_leftRoundedRectangle(VisageCanvas* canvas, float x, float y, float width, float height, float rounding);
void VisageCanvas_rightRoundedRectangle(VisageCanvas* canvas, float x, float y, float width, float height, float rounding);
void VisageCanvas_topRoundedRectangle(VisageCanvas* canvas, float x, float y, float width, float height, float rounding);
void VisageCanvas_bottomRoundedRectangle(VisageCanvas* canvas, float x, float y, float width, float height, float rounding);
void VisageCanvas_rectangleShadow(VisageCanvas* canvas, float x, float y, float width, float height, float blur_radius);
void VisageCanvas_roundedRectangleShadow(VisageCanvas* canvas, float x, float y, float width, float height, float rounding, float blur_radius);
void VisageCanvas_roundedRectangleBorder(VisageCanvas* canvas, float x, float y, float width, float height, float rounding, float thickness);
void VisageCanvas_triangle(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float c_x, float c_y);
void VisageCanvas_triangleBorder(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float c_x, float c_y, float thickness);
void VisageCanvas_roundedTriangleBorder(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float c_x, float c_y, float rounding, float thickness);
void VisageCanvas_roundedTriangle(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float c_x, float c_y, float rounding);
void VisageCanvas_triangleLeft(VisageCanvas* canvas, float triangle_x, float triangle_y, float triangle_width);
void VisageCanvas_triangleRight(VisageCanvas* canvas, float triangle_x, float triangle_y, float triangle_width);
void VisageCanvas_triangleUp(VisageCanvas* canvas, float triangle_x, float triangle_y, float triangle_width);
void VisageCanvas_triangleDown(VisageCanvas* canvas, float triangle_x, float triangle_y, float triangle_width);

void VisageCanvas_line(VisageCanvas* canvas, VisageLine* line, float x, float y, float width, float height, float line_width);
void VisageCanvas_lineFill(VisageCanvas* canvas, VisageLine* line, float x, float y, float width, float height, float fill_position);

void VisageCanvas_text(VisageCanvas* canvas, VisageText* text, float x, float y, float width, float height, int32_t direction);

void VisageCanvas_saveState(VisageCanvas* canvas);
void VisageCanvas_restoreState(VisageCanvas* canvas);

void VisageCanvas_setPosition(VisageCanvas* canvas, float x, float y);

//void VisageCanvas_setClampBounds(VisageCanvas* canvas, float x, float y, float width, float height);
//void VisageCanvas_trimClampBounds(VisageCanvas* canvas, int32_t x, int32_t y, int32_t width, int32_t height);

#ifdef __cplusplus
}
#endif

#endif /* VISAGE_C_WRAPPER_H */