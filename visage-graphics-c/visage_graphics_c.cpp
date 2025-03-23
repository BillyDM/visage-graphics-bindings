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

    VisageColor VisageColor_fromAHSV(float alpha, float hue, float saturation, float value) {
        return color_from_cpp(visage::Color::fromAHSV(alpha, hue, saturation, value));
    }
    VisageColor VisageColor_fromABGR(unsigned int abgr) {
        return color_from_cpp(visage::Color::fromABGR(abgr));
    }
    VisageColor VisageColor_fromARGB(unsigned int argb) {
        return color_from_cpp(visage::Color::fromARGB(argb));
    }
    VisageColor VisageColor_fromHexString(const char* str) {
        std::string cpp_str = std::string(str);
        return color_from_cpp(visage::Color::fromHexString(cpp_str));
    }
    unsigned int VisageColor_toABGR(const VisageColor* color) {
        return color_to_cpp(*color).toABGR();
    }
    unsigned int VisageColor_toARGB(const VisageColor* color) {
        return color_to_cpp(*color).toARGB();
    }
    unsigned int VisageColor_toRGB(const VisageColor* color) {
        return color_to_cpp(*color).toRGB();
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
    
    void VisageCanvas_pairToWindow(VisageCanvas* canvas, void* window_handle, int width, int height) {
        reinterpret_cast<visage::Canvas*>(canvas)->pairToWindow(window_handle, width, height);
    }

    void VisageCanvas_submit(VisageCanvas* canvas, int submit_pass) {
        reinterpret_cast<visage::Canvas*>(canvas)->submit(submit_pass);
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