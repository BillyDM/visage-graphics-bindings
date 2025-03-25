#include "visage/graphics.h"
#include <visage_graphics/canvas.h>
#include <visage_graphics/gradient.h>
#include <visage_utils/space.h>

#include "visage_graphics_c.h"

inline VisageColor color_from_cpp(visage::Color color) {
    return VisageColor { .values = {color.blue(), color.green(), color.red(), color.alpha() }, .hdr = color.hdr() };
}
inline visage::Color color_to_cpp(const VisageColor color) {
    return visage::Color {
        color.values[VisageColorChannelAlpha],
        color.values[VisageColorChannelRed],
        color.values[VisageColorChannelGreen],
        color.values[VisageColorChannelBlue],
        color.hdr
    };
}

extern "C"
{
    // -- Renderer -------------------------------------------------------------------------------------

    void VisageRenderer_checkInitialization(void* model_window, void* display) {
        visage::Renderer::instance().checkInitialization(model_window, display);
    }
    bool VisageRenderer_supported() {
        return visage::Renderer::instance().supported();
    }
    bool VisageRenderer_swapChainSupported() {
        return visage::Renderer::instance().swapChainSupported();
    }
    bool VisageRenderer_initialized() {
        return visage::Renderer::instance().initialized();
    }

    // -- Color ----------------------------------------------------------------------------------------

    void VisageColor_fromAHSV_inner(float alpha, float hue, float saturation, float value, VisageColor* returnValue) {
        *returnValue = color_from_cpp(visage::Color::fromAHSV(alpha, hue, saturation, value));
    }
    void VisageColor_fromABGR_inner(uint32_t abgr, VisageColor* returnValue) {
        *returnValue = color_from_cpp(visage::Color::fromABGR(static_cast<unsigned int>(abgr)));
    }
    void VisageColor_fromARGB_inner(uint32_t argb, VisageColor* returnValue) {
        *returnValue = color_from_cpp(visage::Color::fromARGB(static_cast<unsigned int>(argb)));
    }
    void VisageColor_fromHexString_inner(const char* str, VisageColor* returnValue) {
        std::string cpp_str = std::string(str);
        *returnValue = color_from_cpp(visage::Color::fromHexString(cpp_str));
    }
    uint32_t VisageColor_toABGR(const VisageColor* color) {
        return static_cast<uint32_t>(color_to_cpp(*color).toABGR());
    }
    uint32_t VisageColor_toARGB(const VisageColor* color) {
        return static_cast<uint32_t>(color_to_cpp(*color).toARGB());
    }
    uint32_t VisageColor_toRGB(const VisageColor* color) {
        return static_cast<uint32_t>(color_to_cpp(*color).toRGB());
    }
    float VisageColor_saturation(const VisageColor* color) {
        return color_to_cpp(*color).saturation();
    }
    float VisageColor_hue(const VisageColor* color) {
        return color_to_cpp(*color).hue();
    }
    
    // -- Gradient -------------------------------------------------------------------------------------

    VisageGradient* VisageGradient_new() {
        auto gradient = new visage::Gradient;
        return reinterpret_cast<VisageGradient*>(gradient);
    }
    VisageGradient* VisageGradient_clone(const VisageGradient* gradient) {
        auto clone = new visage::Gradient(*reinterpret_cast<const visage::Gradient*>(gradient));
        return reinterpret_cast<VisageGradient*>(clone);
    }
    VisageGradient* VisageGradient_fromSampleFunction(int32_t resolution, void (*sample_function)(float, VisageColor*)) {
        auto sample_function_cpp = [&](float t) {
            VisageColor color;
            sample_function(t, &color);
            return color_to_cpp(color);
        };

        auto gradient = new visage::Gradient(visage::Gradient::fromSampleFunction(static_cast<int>(resolution), sample_function_cpp));
        return reinterpret_cast<VisageGradient*>(gradient);
    }
    void VisageGradient_delete(VisageGradient* gradient) {
        delete reinterpret_cast<visage::Gradient*>(gradient);
    }

    int32_t VisageGradient_getResolution(const VisageGradient* gradient) {
        return static_cast<int32_t>(reinterpret_cast<const visage::Gradient*>(gradient)->resolution());
    }
    void VisageGradient_setResolution(VisageGradient* gradient, int32_t resolution) {
        reinterpret_cast<visage::Gradient*>(gradient)->setResolution(static_cast<int>(resolution));
    }
    void VisageGradient_getColor(const VisageGradient* gradient, int32_t index, VisageColor* returnValue) {
        auto colors = reinterpret_cast<const visage::Gradient*>(gradient)->colors();
        if (index < colors.size()) {
            *returnValue = color_from_cpp(colors[index]);
        }
    }
    void VisageGradient_setColor(VisageGradient* gradient, int32_t index, VisageColor color) {
        reinterpret_cast<visage::Gradient*>(gradient)->setColor(static_cast<int>(index), color_to_cpp(color));
    }
    void VisageGradient_interpolateWith(VisageGradient* gradient, const VisageGradient* other, float t) {
        auto b = reinterpret_cast<const visage::Gradient*>(gradient);
        reinterpret_cast<visage::Gradient*>(gradient)->interpolateWith(*b, t);
    }
    void VisageGradient_sample(const VisageGradient* gradient, float t, VisageColor* returnValue) {
        auto color = reinterpret_cast<const visage::Gradient*>(gradient)->sample(t);
        *returnValue = color_from_cpp(color);
    }
    void VisageGradient_multiplyAlpha(VisageGradient* gradient, float mult) {
        auto g = reinterpret_cast<visage::Gradient*>(gradient);
        *g = g->withMultipliedAlpha(mult);
    }
    int32_t VisageGradient_compare(const VisageGradient* a, const VisageGradient* b) {
        auto a_cpp = reinterpret_cast<const visage::Gradient*>(a);
        auto b_cpp = reinterpret_cast<const visage::Gradient*>(b);
        return static_cast<int32_t>(visage::Gradient::compare(*a_cpp, *b_cpp));
    }

    // -- Brush ----------------------------------------------------------------------------------------

    VisageBrush* VisageBrush_new() {
        auto brush = new visage::Brush;
        return reinterpret_cast<VisageBrush*>(brush);
    }
    VisageBrush* VisageBrush_clone(const VisageBrush* brush) {
        auto clone = new visage::Brush(*reinterpret_cast<const visage::Brush*>(brush));
        return reinterpret_cast<VisageBrush*>(clone);
    }
    void VisageBrush_delete(VisageBrush* brush) {
        delete reinterpret_cast<visage::Brush*>(brush);
    }

    void VisageBrush_solid(VisageBrush* brush, const VisageColor* color) {
        *reinterpret_cast<visage::Brush*>(brush) = visage::Brush::solid(color_to_cpp(*color));
    }
    void VisageBrush_horizontal(VisageBrush* brush, const VisageGradient* gradient) {
        *reinterpret_cast<visage::Brush*>(brush) = visage::Brush::horizontal(*reinterpret_cast<const visage::Gradient*>(gradient));
    }
    void VisageBrush_horizontalFromTwo(VisageBrush* brush, VisageColor left, VisageColor right) {
        *reinterpret_cast<visage::Brush*>(brush) = visage::Brush::horizontal(color_to_cpp(left), color_to_cpp(right));
    }
    void VisageBrush_vertical(VisageBrush* brush, const VisageGradient* gradient) {
        *reinterpret_cast<visage::Brush*>(brush) = visage::Brush::vertical(*reinterpret_cast<const visage::Gradient*>(gradient));
    }
    void VisageBrush_verticalFromTwo(VisageBrush* brush, VisageColor top, VisageColor bottom) {
        *reinterpret_cast<visage::Brush*>(brush) = visage::Brush::vertical(color_to_cpp(top), color_to_cpp(bottom));
    }
    void VisageBrush_linear(VisageBrush* brush, const VisageGradient* gradient, float from_x, float from_y, float to_x, float to_y) {
        auto g = reinterpret_cast<const visage::Gradient*>(gradient);
        auto from_position = visage::Point(from_x, from_y);
        auto to_position = visage::Point(to_x, to_y);
        *reinterpret_cast<visage::Brush*>(brush) = visage::Brush::linear(*g, from_position, to_position);
    }
    void VisageBrush_linearFromTwo(VisageBrush* brush, VisageColor from_color, VisageColor to_color, float from_x, float from_y, float to_x, float to_y) {
        auto from_color_cpp = color_to_cpp(from_color);
        auto to_color_cpp = color_to_cpp(to_color);
        auto from_position = visage::Point(from_x, from_y);
        auto to_position = visage::Point(to_x, to_y);
        *reinterpret_cast<visage::Brush*>(brush) = visage::Brush::linear(from_color_cpp, to_color_cpp, from_position, to_position);
    }
    void VisageBrush_interpolateWith(VisageBrush* brush, const VisageBrush* other, float t) {
        reinterpret_cast<visage::Brush*>(brush)->interpolateWith(*reinterpret_cast<const visage::Brush*>(brush), t);
    }
    void VisageBrush_multiplyAlpha(VisageBrush* brush, float mult) {
        auto b = reinterpret_cast<visage::Brush*>(brush);
        *b = b->withMultipliedAlpha(mult);
    }

    // -- Canvas ---------------------------------------------------------------------------------------

    VisageCanvas* VisageCanvas_new() {
        auto canvas = new visage::Canvas;
        return reinterpret_cast<VisageCanvas*>(canvas);
    }
    void VisageCanvas_destroy(VisageCanvas* canvas) {
        delete reinterpret_cast<visage::Canvas*>(canvas);
    }
    
    void VisageCanvas_pairToWindow(VisageCanvas* canvas, void* window_handle, int32_t width, int32_t height) {
        reinterpret_cast<visage::Canvas*>(canvas)->pairToWindow(window_handle, static_cast<int>(width), static_cast<int>(height));
    }
    void VisageCanvas_setDimensions(VisageCanvas* canvas, int32_t width, int32_t height) {
        reinterpret_cast<visage::Canvas*>(canvas)->setDimensions(static_cast<int>(width), static_cast<int>(height));
    }
    void VisageCanvas_setDpiScale(VisageCanvas* canvas, float scale) {
        reinterpret_cast<visage::Canvas*>(canvas)->setDpiScale(scale);
    }
    void VisageCanvas_setNativePixelScale(VisageCanvas* canvas) {
        reinterpret_cast<visage::Canvas*>(canvas)->setNativePixelScale();
    }
    void VisageCanvas_setLogicalPixelScal(VisageCanvas* canvas) {
        reinterpret_cast<visage::Canvas*>(canvas)->setLogicalPixelScale();
    }
    void VisageCanvas_clearDrawnShapes(VisageCanvas* canvas) {
        reinterpret_cast<visage::Canvas*>(canvas)->clearDrawnShapes();
    }
    void VisageCanvas_submit(VisageCanvas* canvas, int32_t submit_pass) {
        reinterpret_cast<visage::Canvas*>(canvas)->submit(static_cast<int>(submit_pass));
    }
    void VisageCanvas_updateTime(VisageCanvas* canvas, double time) {
        reinterpret_cast<visage::Canvas*>(canvas)->updateTime(time);
    }
    void VisageCanvas_setWindowless(VisageCanvas* canvas, int32_t width, int32_t height) {
        reinterpret_cast<visage::Canvas*>(canvas)->setWindowless(static_cast<int>(width), static_cast<int>(height));
    }
    void VisageCanvas_removeFromWindow(VisageCanvas* canvas) {
        reinterpret_cast<visage::Canvas*>(canvas)->removeFromWindow();
    }
    void VisageCanvas_requestScreenshot(VisageCanvas* canvas) {
        reinterpret_cast<visage::Canvas*>(canvas)->requestScreenshot();
    }

    float VisageCanvas_dpiScale(VisageCanvas* canvas) {
        return reinterpret_cast<visage::Canvas*>(canvas)->dpiScale();
    }
    double VisageCanvas_time(VisageCanvas* canvas) {
        return reinterpret_cast<visage::Canvas*>(canvas)->time();
    }
    double VisageCanvas_deltaTime(VisageCanvas* canvas) {
        return reinterpret_cast<visage::Canvas*>(canvas)->deltaTime();
    }
    int32_t VisageCanvas_frameCount(VisageCanvas* canvas) {
        return static_cast<int32_t>(reinterpret_cast<visage::Canvas*>(canvas)->frameCount());
    }

    void VisageCanvas_setColor(VisageCanvas* canvas, VisageColor color) {
        reinterpret_cast<visage::Canvas*>(canvas)->setColor(color_to_cpp(color));
    }
    void VisageCanvas_setBrush(VisageCanvas* canvas, const VisageBrush* brush) {
        reinterpret_cast<visage::Canvas*>(canvas)->setBrush(*reinterpret_cast<const visage::Brush*>(brush));
    }
    void VisageCanvas_fill(VisageCanvas* canvas, float x, float y, float width, float height) {
        reinterpret_cast<visage::Canvas*>(canvas)->fill(x, y, width, height);
    }
    void VisageCanvas_circle(VisageCanvas* canvas, float x, float y, float width) {
        reinterpret_cast<visage::Canvas*>(canvas)->circle(x, y, width);
    }
}