use std::ptr::NonNull;

use crate::{
    brush::Brush,
    color::Color,
    text::{Direction, Text},
};

pub struct Canvas {
    ptr: NonNull<visage_graphics_sys::VisageCanvas>,
}

impl Drop for Canvas {
    fn drop(&mut self) {
        unsafe {
            visage_graphics_sys::VisageCanvas_destroy(self.ptr.as_ptr());
        }
    }
}

impl Canvas {
    pub fn new() -> Self {
        let ptr = unsafe { NonNull::new(visage_graphics_sys::VisageCanvas_new()).unwrap() };

        Self { ptr }
    }

    #[cfg(feature = "rwh_06")]
    pub unsafe fn pair_to_window<
        W: raw_window_handle_06::HasWindowHandle + raw_window_handle_06::HasDisplayHandle,
    >(
        &mut self,
        window: &W,
        window_width: u32,
        window_height: u32,
        dpi_scale: f32,
    ) {
        use raw_window_handle_06::{RawDisplayHandle, RawWindowHandle};
        use std::os::raw::c_void;

        let window_handle = window.window_handle().unwrap().as_raw();
        let display_handle = window.display_handle().unwrap().as_raw();

        let window_ptr: *mut c_void = match window_handle {
            RawWindowHandle::Xlib(h) => h.window as *mut c_void,
            _ => todo!(),
        };
        let display_ptr: *mut c_void = match display_handle {
            RawDisplayHandle::Xlib(handle) => handle.display.unwrap().as_ptr(),
            _ => todo!(),
        };

        unsafe {
            visage_graphics_sys::VisageRenderer_checkInitialization(window_ptr, display_ptr);

            visage_graphics_sys::VisageCanvas_pairToWindow(
                self.ptr.as_ptr(),
                window_ptr,
                window_width as i32,
                window_height as i32,
            );

            visage_graphics_sys::VisageCanvas_setDpiScale(self.ptr.as_ptr(), dpi_scale);
        }
    }

    pub fn set_dimensions(&mut self, width: u32, height: u32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_setDimensions(
                self.ptr.as_ptr(),
                width as i32,
                height as i32,
            );
        }
    }

    pub fn set_dpi_scale(&mut self, dpi_scale: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_setDpiScale(self.ptr.as_ptr(), dpi_scale);
        }
    }

    pub fn set_native_pixel_scale(&mut self) {
        unsafe {
            visage_graphics_sys::VisageCanvas_setNativePixelScale(self.ptr.as_ptr());
        }
    }

    pub fn set_logical_pixel_scale(&mut self) {
        unsafe {
            visage_graphics_sys::VisageCanvas_setLogicalPixelScale(self.ptr.as_ptr());
        }
    }

    pub fn clear_drawn_shapes(&mut self) {
        unsafe {
            visage_graphics_sys::VisageCanvas_clearDrawnShapes(self.ptr.as_ptr());
        }
    }

    pub fn submit(&mut self, submit_pass: i32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_submit(self.ptr.as_ptr(), submit_pass);
        }
    }

    pub fn update_time(&mut self, time: f64) {
        unsafe {
            visage_graphics_sys::VisageCanvas_updateTime(self.ptr.as_ptr(), time);
        }
    }

    pub fn set_windowless(&mut self, width: u32, height: u32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_setWindowless(
                self.ptr.as_ptr(),
                width as i32,
                height as i32,
            );
        }
    }

    pub fn remove_from_window(&mut self) {
        unsafe {
            visage_graphics_sys::VisageCanvas_removeFromWindow(self.ptr.as_ptr());
        }
    }

    pub fn reqeust_screenshot(&mut self) {
        unsafe {
            visage_graphics_sys::VisageCanvas_requestScreenshot(self.ptr.as_ptr());
        }
    }

    pub fn dpi_scale(&self) -> f32 {
        unsafe { visage_graphics_sys::VisageCanvas_dpiScale(self.ptr.as_ptr()) }
    }

    pub fn time(&self) -> f64 {
        unsafe { visage_graphics_sys::VisageCanvas_time(self.ptr.as_ptr()) }
    }

    pub fn delta_time(&self) -> f64 {
        unsafe { visage_graphics_sys::VisageCanvas_deltaTime(self.ptr.as_ptr()) }
    }

    pub fn frame_count(&self) -> i32 {
        unsafe { visage_graphics_sys::VisageCanvas_frameCount(self.ptr.as_ptr()) }
    }

    pub fn set_color(&mut self, color: impl Into<Color>) {
        let color: Color = color.into();

        unsafe {
            visage_graphics_sys::VisageCanvas_setColor(self.ptr.as_ptr(), color.raw());
        }
    }

    pub fn set_brush(&mut self, brush: &Brush) {
        unsafe {
            visage_graphics_sys::VisageCanvas_setBrush(self.ptr.as_ptr(), brush.raw().as_ptr());
        }
    }

    pub fn fill(&mut self, x: f32, y: f32, width: f32, height: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_fill(self.ptr.as_ptr(), x, y, width, height);
        }
    }
    pub fn circle(&mut self, x: f32, y: f32, width: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_circle(self.ptr.as_ptr(), x, y, width);
        }
    }
    pub fn fade_circle(&mut self, x: f32, y: f32, width: f32, pixel_width: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_fadeCircle(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                pixel_width,
            );
        }
    }
    pub fn ring(&mut self, x: f32, y: f32, width: f32, thickness: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_ring(self.ptr.as_ptr(), x, y, width, thickness);
        }
    }
    pub fn squircle(&mut self, x: f32, y: f32, width: f32, power: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_squircle(self.ptr.as_ptr(), x, y, width, power);
        }
    }
    pub fn squircle_border(&mut self, x: f32, y: f32, width: f32, power: f32, thickness: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_squircleBorder(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                power,
                thickness,
            );
        }
    }
    pub fn super_ellipse(&mut self, x: f32, y: f32, width: f32, height: f32, power: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_superEllipse(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                height,
                power,
            );
        }
    }
    pub fn rounded_arc(
        &mut self,
        x: f32,
        y: f32,
        width: f32,
        thickness: f32,
        center_radians: f32,
        radians: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_roundedArc(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                thickness,
                center_radians,
                radians,
            );
        }
    }
    pub fn flat_arc(
        &mut self,
        x: f32,
        y: f32,
        width: f32,
        thickness: f32,
        center_radians: f32,
        radians: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_flatArc(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                thickness,
                center_radians,
                radians,
            );
        }
    }
    pub fn arc(
        &mut self,
        x: f32,
        y: f32,
        width: f32,
        thickness: f32,
        center_radians: f32,
        radians: f32,
        rounded: bool,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_arc(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                thickness,
                center_radians,
                radians,
                rounded,
            );
        }
    }
    pub fn rounded_arc_shadow(
        &mut self,
        x: f32,
        y: f32,
        width: f32,
        thickness: f32,
        center_radians: f32,
        radians: f32,
        shadow_width: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_roundedArcShadow(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                thickness,
                center_radians,
                radians,
                shadow_width,
            );
        }
    }
    pub fn flat_arc_shadow(
        &mut self,
        x: f32,
        y: f32,
        width: f32,
        thickness: f32,
        center_radians: f32,
        radians: f32,
        shadow_width: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_flatArcShadow(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                thickness,
                center_radians,
                radians,
                shadow_width,
            );
        }
    }
    pub fn segment(
        &mut self,
        a_x: f32,
        a_y: f32,
        b_x: f32,
        b_y: f32,
        thickness: f32,
        rounded: bool,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_segment(
                self.ptr.as_ptr(),
                a_x,
                a_y,
                b_x,
                b_y,
                thickness,
                rounded,
            );
        }
    }
    pub fn quadratic(
        &mut self,
        a_x: f32,
        a_y: f32,
        b_x: f32,
        b_y: f32,
        c_x: f32,
        c_y: f32,
        thickness: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_quadratic(
                self.ptr.as_ptr(),
                a_x,
                a_y,
                b_x,
                b_y,
                c_x,
                c_y,
                thickness,
            );
        }
    }
    pub fn rectangle(&mut self, x: f32, y: f32, width: f32, height: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_rectangle(self.ptr.as_ptr(), x, y, width, height);
        }
    }
    pub fn rounded_rectangle(&mut self, x: f32, y: f32, width: f32, height: f32, rounding: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_roundedRectangle(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                height,
                rounding,
            );
        }
    }
    pub fn diamond(&mut self, x: f32, y: f32, width: f32, rounding: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_diamond(self.ptr.as_ptr(), x, y, width, rounding);
        }
    }
    pub fn left_rounded_rectangle(
        &mut self,
        x: f32,
        y: f32,
        width: f32,
        height: f32,
        rounding: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_leftRoundedRectangle(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                height,
                rounding,
            );
        }
    }
    pub fn right_rounded_rectangle(
        &mut self,
        x: f32,
        y: f32,
        width: f32,
        height: f32,
        rounding: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_rightRoundedRectangle(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                height,
                rounding,
            );
        }
    }
    pub fn top_rounded_rectangle(
        &mut self,
        x: f32,
        y: f32,
        width: f32,
        height: f32,
        rounding: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_topRoundedRectangle(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                height,
                rounding,
            );
        }
    }
    pub fn bottom_rounded_rectangle(
        &mut self,
        x: f32,
        y: f32,
        width: f32,
        height: f32,
        rounding: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_bottomRoundedRectangle(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                height,
                rounding,
            );
        }
    }
    pub fn rectangle_shadow(&mut self, x: f32, y: f32, width: f32, height: f32, blur_radius: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_rectangleShadow(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                height,
                blur_radius,
            );
        }
    }
    pub fn rounded_rectangle_shadow(
        &mut self,
        x: f32,
        y: f32,
        width: f32,
        height: f32,
        rounding: f32,
        blur_radius: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_roundedRectangleShadow(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                height,
                rounding,
                blur_radius,
            );
        }
    }
    pub fn rounded_rectangle_border(
        &mut self,
        x: f32,
        y: f32,
        width: f32,
        height: f32,
        rounding: f32,
        thickness: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_roundedRectangleBorder(
                self.ptr.as_ptr(),
                x,
                y,
                width,
                height,
                rounding,
                thickness,
            );
        }
    }
    pub fn triangle(&mut self, a_x: f32, a_y: f32, b_x: f32, b_y: f32, c_x: f32, c_y: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_triangle(
                self.ptr.as_ptr(),
                a_x,
                a_y,
                b_x,
                b_y,
                c_x,
                c_y,
            );
        }
    }
    pub fn triangle_border(
        &mut self,
        a_x: f32,
        a_y: f32,
        b_x: f32,
        b_y: f32,
        c_x: f32,
        c_y: f32,
        thickness: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_triangleBorder(
                self.ptr.as_ptr(),
                a_x,
                a_y,
                b_x,
                b_y,
                c_x,
                c_y,
                thickness,
            );
        }
    }
    pub fn rounded_triangle_border(
        &mut self,
        a_x: f32,
        a_y: f32,
        b_x: f32,
        b_y: f32,
        c_x: f32,
        c_y: f32,
        rounding: f32,
        thickness: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_roundedTriangleBorder(
                self.ptr.as_ptr(),
                a_x,
                a_y,
                b_x,
                b_y,
                c_x,
                c_y,
                rounding,
                thickness,
            );
        }
    }
    pub fn rounded_triangle(
        &mut self,
        a_x: f32,
        a_y: f32,
        b_x: f32,
        b_y: f32,
        c_x: f32,
        c_y: f32,
        rounding: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_roundedTriangle(
                self.ptr.as_ptr(),
                a_x,
                a_y,
                b_x,
                b_y,
                c_x,
                c_y,
                rounding,
            );
        }
    }
    pub fn triangle_left(&mut self, x: f32, y: f32, width: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_triangleLeft(self.ptr.as_ptr(), x, y, width);
        }
    }
    pub fn triangle_right(&mut self, x: f32, y: f32, width: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_triangleRight(self.ptr.as_ptr(), x, y, width);
        }
    }
    pub fn triangle_up(&mut self, x: f32, y: f32, width: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_triangleUp(self.ptr.as_ptr(), x, y, width);
        }
    }
    pub fn triangle_down(&mut self, x: f32, y: f32, width: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_triangleDown(self.ptr.as_ptr(), x, y, width);
        }
    }

    pub fn text(
        &mut self,
        text: &Text,
        x: f32,
        y: f32,
        width: f32,
        height: f32,
        direction: Direction,
    ) {
        unsafe {
            visage_graphics_sys::VisageCanvas_text(
                self.ptr.as_ptr(),
                text.raw().as_ptr(),
                x,
                y,
                width,
                height,
                direction as i32,
            );
        }
    }

    pub fn save_state(&mut self) {
        unsafe {
            visage_graphics_sys::VisageCanvas_saveState(self.ptr.as_ptr());
        }
    }

    pub fn restore_state(&mut self) {
        unsafe {
            visage_graphics_sys::VisageCanvas_restoreState(self.ptr.as_ptr());
        }
    }

    pub fn set_position(&mut self, x: f32, y: f32) {
        unsafe {
            visage_graphics_sys::VisageCanvas_setPosition(self.ptr.as_ptr(), x, y);
        }
    }
}
