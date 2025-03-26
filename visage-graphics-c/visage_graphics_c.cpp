#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <visage/graphics.h>
#include <visage_graphics/canvas.h>
#include <visage_graphics/font.h>
#include <visage_graphics/gradient.h>
#include <visage_graphics/shapes.h>
#include <visage_graphics/text.h>
#include <visage_utils/defines.h>
#include <visage_utils/space.h>
#include <visage_utils/string_utils.h>
#include <embedded/fonts.h>

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

inline visage::Direction direction_to_cpp(int32_t direction) {
    switch(direction) {
        case 1:
            return visage::Direction::Up;
        case 2: 
            return visage::Direction::Right;
        case 3:
            return visage::Direction::Down;
        default:
            return visage::Direction::Left;
    }
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

    // -- Line -----------------------------------------------------------------------------------------

    VisageLine* VisageLine_new(int32_t points) {
        auto line = new visage::Line(points);
        return reinterpret_cast<VisageLine*>(line);
    }
    VisageLine* VisageLine_copy(const VisageLine* line) {
        auto clone = new visage::Line(*reinterpret_cast<const visage::Line*>(line));
        return reinterpret_cast<VisageLine*>(clone);
    }
    void VisageLine_delete(VisageLine* line) {
        delete reinterpret_cast<visage::Line*>(line);
    }

    int32_t VisageLine_getNumPoints(const VisageLine* line) {
        return static_cast<int32_t>(reinterpret_cast<const visage::Line*>(line)->num_points);
    }
    void VisageLine_setNumPoints(VisageLine* line, int32_t points) {
        reinterpret_cast<visage::Line*>(line)->setNumPoints(static_cast<int>(points));
    }
    float VisageLine_getLineValueScale(const VisageLine* line) {
        return reinterpret_cast<const visage::Line*>(line)->line_value_scale;
    }
    void VisageLine_setLineValueScale(VisageLine* line, float line_value_scale) {
        reinterpret_cast<visage::Line*>(line)->line_value_scale = line_value_scale;
    }
    float VisageLine_getFillValueScale(const VisageLine* line) {
        return reinterpret_cast<const visage::Line*>(line)->fill_value_scale;
    }
    void VisageLine_setFillValueScale(VisageLine* line, float fill_value_scale) {
        reinterpret_cast<visage::Line*>(line)->fill_value_scale = fill_value_scale;
    }
    float* VisageLine_xValues(VisageLine* line) {
        auto line_cpp = reinterpret_cast<visage::Line*>(line);

        if (line_cpp->num_points == 0) {
            return nullptr;
        } else {
            return &(line_cpp->x[0]);
        }
    }
    float* VisageLine_yValues(VisageLine* line) {
        auto line_cpp = reinterpret_cast<visage::Line*>(line);

        if (line_cpp->num_points == 0) {
            return nullptr;
        } else {
            return &(line_cpp->y[0]);
        }
    }
    float* VisageLine_values(VisageLine* line) {
        auto line_cpp = reinterpret_cast<visage::Line*>(line);

        if (line_cpp->num_points == 0) {
            return nullptr;
        } else {
            return &(line_cpp->values[0]);
        }
    }

    // -- Font -----------------------------------------------------------------------------------------

    VisageFont* VisageFont_new(float size, const char* font_data, int32_t data_size, float dpi_scale) {
        auto font = new visage::Font(size, font_data, static_cast<int>(data_size), dpi_scale);
        return reinterpret_cast<VisageFont*>(font);
    }
    VisageFont* VisageFont_copy(const VisageFont* font) {
        auto clone = new visage::Font(*reinterpret_cast<const visage::Font*>(font));
        return reinterpret_cast<VisageFont*>(clone);
    }
    VisageFont* VisageFont_withDpiScale(const VisageFont* font, float dpi_scale) {
        auto new_font = new visage::Font(reinterpret_cast<const visage::Font*>(font)->withDpiScale(dpi_scale));
        return reinterpret_cast<VisageFont*>(new_font);
    }
    void VisageFont_delete(VisageFont* font) {
        delete reinterpret_cast<visage::Font*>(font);
    }

    float VisageFont_getDpiScale(const VisageFont* font) {
        return reinterpret_cast<const visage::Font*>(font)->dpiScale();
    }

    int32_t VisageFont_widthOverflowIndex(const VisageFont* font, const char32_t* string, int32_t string_length, float width, bool round, int32_t character_override) {
        auto font_cpp = reinterpret_cast<const visage::Font*>(font);
        auto string_length_cpp = static_cast<int>(string_length);
        auto character_override_cpp = static_cast<int>(character_override);

        return font_cpp->widthOverflowIndex(string, string_length_cpp, width, round, character_override_cpp);
    }
    int32_t VisageFont_lineBreaks(const VisageFont* font, const char32_t* string, int32_t string_length, float width, int* line_breaks, int32_t line_breaks_length) {
        auto font_cpp = reinterpret_cast<const visage::Font*>(font);
        auto string_length_cpp = static_cast<int>(string_length);

        auto result = font_cpp->lineBreaks(string, string_length_cpp, width);

        auto length = std::min(result.size(), static_cast<size_t>(line_breaks_length));

        if (length > 0) {
            std::memcpy(line_breaks, &(result[0]), length);
        }

        return result.size();
    }
    float VisageFont_stringWidth(const VisageFont* font, const char32_t* string, int32_t string_length, int32_t character_override) {
        auto font_cpp = reinterpret_cast<const visage::Font*>(font);
        auto string_length_cpp = static_cast<int>(string_length);
        auto character_override_cpp = static_cast<int>(character_override);

        return font_cpp->stringWidth(string, string_length_cpp, character_override_cpp);
    }
    float VisageFont_lineHeight(const VisageFont* font) {
        return reinterpret_cast<const visage::Font*>(font)->lineHeight();
    }
    float VisageFont_capitalHeight(const VisageFont* font) {
        return reinterpret_cast<const visage::Font*>(font)->capitalHeight();
    }
    float VisageFont_lowerDipHeight(const VisageFont* font) {
        return reinterpret_cast<const visage::Font*>(font)->lowerDipHeight();
    }
    float VisageFont_size(const VisageFont* font) {
        return reinterpret_cast<const visage::Font*>(font)->size();
    }
    const char* VisageFont_fontData(const VisageFont* font, int32_t* data_size) {
        auto font_cpp = reinterpret_cast<const visage::Font*>(font);
        *data_size = font_cpp->dataSize();
        return font_cpp->fontData();
    }

    VisageFont* VisageFont_LatoRegular(float size, float dpi_scale) {
        auto font = new visage::Font(size, visage::fonts::Lato_Regular_ttf);
        return reinterpret_cast<VisageFont*>(font);
    }
    VisageFont* VisageFont_DroidSansMono(float size, float dpi_scale) {
        auto font = new visage::Font(size, visage::fonts::DroidSansMono_ttf);
        return reinterpret_cast<VisageFont*>(font);
    }
    VisageFont* VisageFont_TwemojiMozilla(float size, float dpi_scale) {
        auto font = new visage::Font(size, visage::fonts::Twemoji_Mozilla_ttf);
        return reinterpret_cast<VisageFont*>(font);
    }

    // -- Text -----------------------------------------------------------------------------------------

    VisageText* VisageText_new(const VisageFont* font) {
        auto text = new visage::Text;
        text->setFont(*reinterpret_cast<const visage::Font*>(font));
        return reinterpret_cast<VisageText*>(text);
    }
    VisageText* VisageText_copy(const VisageText* text) {
        auto clone = new visage::Text(*reinterpret_cast<const visage::Text*>(text));
        return reinterpret_cast<VisageText*>(clone);
    }
    void VisageText_delete(VisageText* text) {
        delete reinterpret_cast<visage::Text*>(text);
    }

    void VisageText_setText(VisageText* text, const char* s) {
        visage::String cpp_str = visage::String(s);
        reinterpret_cast<visage::Text*>(text)->setText(cpp_str);
    }
    void VisageText_setTextWithLength(VisageText* text, const char* s, int32_t length) {
        std::string cpp_str = std::string(s, length);
        reinterpret_cast<visage::Text*>(text)->setText(cpp_str);
    }
    void VisageText_setTextU32(VisageText* text, const char32_t* s) {
        visage::String cpp_str = visage::String(s);
        reinterpret_cast<visage::Text*>(text)->setText(cpp_str);
    }
    void VisageText_setTextU32WithLength(VisageText* text, const char32_t* s, int32_t length) {
        std::u32string cpp_str = std::u32string(s, length);
        reinterpret_cast<visage::Text*>(text)->setText(cpp_str);
    }
    const char32_t* VisageText_getTextU32(const VisageText* text, int32_t* length) {
        auto text_cpp = reinterpret_cast<const visage::Text*>(text);
        auto s = &(text_cpp->text());
        *length = s->length();
        return s->c_str();
    }

    void VisageText_setFont(VisageText* text, const VisageFont* font) {
        reinterpret_cast<visage::Text*>(text)->setFont(*reinterpret_cast<const visage::Font*>(font));
    }
    const VisageFont* VisageText_getFont(const VisageText* text) {
        return reinterpret_cast<const VisageFont*>(&(reinterpret_cast<const visage::Text*>(text)->font()));
    }

    void VisageText_setJustification(VisageText* text, int32_t justification) {
        reinterpret_cast<visage::Text*>(text)->setJustification(static_cast<visage::Font::Justification>(justification));
    }
    int32_t VisageText_getJustification(const VisageText* text) {
        return static_cast<int32_t>(reinterpret_cast<const visage::Text*>(text)->justification());
    }

    void VisageText_setMultiLine(VisageText* text, bool multi_line) {
        reinterpret_cast<visage::Text*>(text)->setMultiLine(multi_line);
    }
    bool VisageText_getMultiLine(const VisageText* text) {
        return reinterpret_cast<const visage::Text*>(text)->multiLine();
    }

    void VisageText_setCharacterOverride(VisageText* text, int32_t character) {
        reinterpret_cast<visage::Text*>(text)->setCharacterOverride(static_cast<int>(character));
    }
    int32_t VisageText_getCharacterOverride(const VisageText* text) {
        return static_cast<int32_t>(reinterpret_cast<const visage::Text*>(text)->characterOverride());
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
    void VisageCanvas_setLogicalPixelScale(VisageCanvas* canvas) {
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
    void VisageCanvas_fadeCircle(VisageCanvas* canvas, float x, float y, float width, float pixel_width) {
        reinterpret_cast<visage::Canvas*>(canvas)->fadeCircle(x, y, width, pixel_width);
    }
    void VisageCanvas_ring(VisageCanvas* canvas, float x, float y, float width, float thickness) {
        reinterpret_cast<visage::Canvas*>(canvas)->ring(x, y, width, thickness);
    }
    void VisageCanvas_squircle(VisageCanvas* canvas, float x, float y, float width, float power) {
        reinterpret_cast<visage::Canvas*>(canvas)->squircle(x, y, width, power);
    }
    void VisageCanvas_squircleBorder(VisageCanvas* canvas, float x, float y, float width, float power, float thickness) {
        reinterpret_cast<visage::Canvas*>(canvas)->squircleBorder(x, y, width, power, thickness);
    }
    void VisageCanvas_superEllipse(VisageCanvas* canvas, float x, float y, float width, float height, float power) {
        reinterpret_cast<visage::Canvas*>(canvas)->superEllipse(x, y, width, height, power);
    }
    void VisageCanvas_roundedArc(VisageCanvas* canvas, float x, float y, float width, float thickness, float center_radians, float radians) {
        reinterpret_cast<visage::Canvas*>(canvas)->roundedArc(x, y, width, thickness, center_radians, radians);
    }
    void VisageCanvas_flatArc(VisageCanvas* canvas, float x, float y, float width, float thickness, float center_radians, float radians) {
        reinterpret_cast<visage::Canvas*>(canvas)->flatArc(x, y, width, thickness, center_radians, radians);
    }
    void VisageCanvas_arc(VisageCanvas* canvas, float x, float y, float width, float thickness, float center_radians, float radians, bool rounded) {
        reinterpret_cast<visage::Canvas*>(canvas)->arc(x, y, width, thickness, center_radians, radians, rounded);
    }
    void VisageCanvas_roundedArcShadow(VisageCanvas* canvas, float x, float y, float width, float thickness, float center_radians, float radians, float shadow_width) {
        reinterpret_cast<visage::Canvas*>(canvas)->roundedArcShadow(x, y, width, thickness, center_radians, radians, shadow_width);
    }
    void VisageCanvas_flatArcShadow(VisageCanvas* canvas, float x, float y, float width, float thickness, float center_radians, float radians, float shadow_width) {
        reinterpret_cast<visage::Canvas*>(canvas)->flatArcShadow(x, y, width, thickness, center_radians, radians, shadow_width);
    }
    void VisageCanvas_segment(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float thickness, bool rounded) {
        reinterpret_cast<visage::Canvas*>(canvas)->segment(a_x, a_y, b_x, b_y, thickness, rounded);
    }
    void VisageCanvas_quadratic(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float c_x, float c_y, float thickness) {
        reinterpret_cast<visage::Canvas*>(canvas)->quadratic(a_x, a_y, b_x, b_y, c_x, c_y, thickness);
    }
    void VisageCanvas_rectangle(VisageCanvas* canvas, float x, float y, float width, float height) {
        reinterpret_cast<visage::Canvas*>(canvas)->rectangle(x, y, width, height);
    }
    void VisageCanvas_rectangleBorder(VisageCanvas* canvas, float x, float y, float width, float height, float thickness) {
        reinterpret_cast<visage::Canvas*>(canvas)->rectangleBorder(x, y, width, height, thickness);
    }
    void VisageCanvas_roundedRectangle(VisageCanvas* canvas, float x, float y, float width, float height, float rounding) {
        reinterpret_cast<visage::Canvas*>(canvas)->roundedRectangle(x, y, width, height, rounding);
    }
    void VisageCanvas_diamond(VisageCanvas* canvas, float x, float y, float width, float rounding) {
        reinterpret_cast<visage::Canvas*>(canvas)->diamond(x, y, width, rounding);
    }
    void VisageCanvas_leftRoundedRectangle(VisageCanvas* canvas, float x, float y, float width, float height, float rounding) {
        reinterpret_cast<visage::Canvas*>(canvas)->leftRoundedRectangle(x, y, width, height, rounding);
    }
    void VisageCanvas_rightRoundedRectangle(VisageCanvas* canvas, float x, float y, float width, float height, float rounding) {
        reinterpret_cast<visage::Canvas*>(canvas)->rightRoundedRectangle(x, y, width, height, rounding);
    }
    void VisageCanvas_topRoundedRectangle(VisageCanvas* canvas, float x, float y, float width, float height, float rounding) {
        reinterpret_cast<visage::Canvas*>(canvas)->topRoundedRectangle(x, y, width, height, rounding);
    }
    void VisageCanvas_bottomRoundedRectangle(VisageCanvas* canvas, float x, float y, float width, float height, float rounding) {
        reinterpret_cast<visage::Canvas*>(canvas)->bottomRoundedRectangle(x, y, width, height, rounding);
    }
    void VisageCanvas_rectangleShadow(VisageCanvas* canvas, float x, float y, float width, float height, float blur_radius) {
        reinterpret_cast<visage::Canvas*>(canvas)->rectangleShadow(x, y, width, height, blur_radius);
    }
    void VisageCanvas_roundedRectangleShadow(VisageCanvas* canvas, float x, float y, float width, float height, float rounding, float blur_radius) {
        reinterpret_cast<visage::Canvas*>(canvas)->roundedRectangleShadow(x, y, width, height, rounding, blur_radius);
    }
    void VisageCanvas_roundedRectangleBorder(VisageCanvas* canvas, float x, float y, float width, float height, float rounding, float thickness) {
        reinterpret_cast<visage::Canvas*>(canvas)->roundedRectangleBorder(x, y, width, height, rounding, thickness);
    }
    void VisageCanvas_triangle(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float c_x, float c_y) {
        reinterpret_cast<visage::Canvas*>(canvas)->triangle(a_x, a_y, b_x, b_y, c_x, c_y);
    }
    void VisageCanvas_triangleBorder(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float c_x, float c_y, float thickness) {
        reinterpret_cast<visage::Canvas*>(canvas)->triangleBorder(a_x, a_y, b_x, b_y, c_x, c_y, thickness);
    }
    void VisageCanvas_roundedTriangleBorder(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float c_x, float c_y, float rounding, float thickness) {
        reinterpret_cast<visage::Canvas*>(canvas)->roundedTriangleBorder(a_x, a_y, b_x, b_y, c_x, c_y, rounding, thickness);
    }
    void VisageCanvas_roundedTriangle(VisageCanvas* canvas, float a_x, float a_y, float b_x, float b_y, float c_x, float c_y, float rounding) {
        reinterpret_cast<visage::Canvas*>(canvas)->roundedTriangle(a_x, a_y, b_x, b_y, c_x, c_y, rounding);
    }
    void VisageCanvas_triangleLeft(VisageCanvas* canvas, float triangle_x, float triangle_y, float triangle_width) {
        reinterpret_cast<visage::Canvas*>(canvas)->triangleLeft(triangle_x, triangle_y, triangle_width);
    }
    void VisageCanvas_triangleRight(VisageCanvas* canvas, float triangle_x, float triangle_y, float triangle_width) {
        reinterpret_cast<visage::Canvas*>(canvas)->triangleRight(triangle_x, triangle_y, triangle_width);
    }
    void VisageCanvas_triangleUp(VisageCanvas* canvas, float triangle_x, float triangle_y, float triangle_width) {
        reinterpret_cast<visage::Canvas*>(canvas)->triangleUp(triangle_x, triangle_y, triangle_width);
    }
    void VisageCanvas_triangleDown(VisageCanvas* canvas, float triangle_x, float triangle_y, float triangle_width) {
        reinterpret_cast<visage::Canvas*>(canvas)->triangleDown(triangle_x, triangle_y, triangle_width);
    }

    void VisageCanvas_line(VisageCanvas* canvas, VisageLine* line, float x, float y, float width, float height, float line_width) {
        reinterpret_cast<visage::Canvas*>(canvas)->line(reinterpret_cast<visage::Line*>(line), x, y, width, height, line_width);
    }
    void VisageCanvas_lineFill(VisageCanvas* canvas, VisageLine* line, float x, float y, float width, float height, float fill_position) {
        reinterpret_cast<visage::Canvas*>(canvas)->lineFill(reinterpret_cast<visage::Line*>(line), x, y, width, height, fill_position);
    }

    void VisageCanvas_text(VisageCanvas* canvas, VisageText* text, float x, float y, float width, float height, int32_t direction) {
        reinterpret_cast<visage::Canvas*>(canvas)->text(reinterpret_cast<visage::Text*>(text), x, y, width, height, direction_to_cpp(direction));
    }

    void VisageCanvas_saveState(VisageCanvas* canvas) {
        reinterpret_cast<visage::Canvas*>(canvas)->saveState();
    }
    void VisageCanvas_restoreState(VisageCanvas* canvas) {
        reinterpret_cast<visage::Canvas*>(canvas)->restoreState();
    }

    void VisageCanvas_setPosition(VisageCanvas* canvas, float x, float y) {
        reinterpret_cast<visage::Canvas*>(canvas)->setPosition(x, y);
    }
}