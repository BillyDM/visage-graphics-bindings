#include "visage/graphics.h"
#include <visage_graphics/canvas.h>

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
    void VisageCanvas_clearDrawnShapes(VisageCanvas* canvas) {
        reinterpret_cast<visage::Canvas*>(canvas)->clearDrawnShapes();
    }
    void VisageCanvas_submit(VisageCanvas* canvas, int32_t submit_pass) {
        reinterpret_cast<visage::Canvas*>(canvas)->submit(static_cast<int>(submit_pass));
    }

    void VisageCanvas_setColor(VisageCanvas* canvas, VisageColor color) {
        reinterpret_cast<visage::Canvas*>(canvas)->setColor(color_to_cpp(color));
    }
    void VisageCanvas_fill(VisageCanvas* canvas, float x, float y, float width, float height) {
        reinterpret_cast<visage::Canvas*>(canvas)->fill(x, y, width, height);
    }
    void VisageCanvas_circle(VisageCanvas* canvas, float x, float y, float width) {
        reinterpret_cast<visage::Canvas*>(canvas)->circle(x, y, width);
    }
}