use std::ptr::NonNull;

use crate::{color::Color, gradient::Gradient};

pub struct Brush {
    ptr: NonNull<visage_graphics_sys::VisageBrush>,
}

impl Brush {
    pub fn new() -> Self {
        unsafe {
            Self {
                ptr: NonNull::new(visage_graphics_sys::VisageBrush_new()).unwrap(),
            }
        }
    }

    pub fn solid(&mut self, color: Color) {
        unsafe {
            visage_graphics_sys::VisageBrush_solid(self.ptr.as_ptr(), color.raw());
        }
    }

    pub fn horizontal(&mut self, gradient: &Gradient) {
        unsafe {
            visage_graphics_sys::VisageBrush_horizontal(self.ptr.as_ptr(), gradient.raw().as_ptr());
        }
    }

    pub fn horizontal_from_two(&mut self, left: Color, right: Color) {
        unsafe {
            visage_graphics_sys::VisageBrush_horizontalFromTwo(
                self.ptr.as_ptr(),
                left.raw(),
                right.raw(),
            );
        }
    }

    pub fn vertical(&mut self, gradient: &Gradient) {
        unsafe {
            visage_graphics_sys::VisageBrush_vertical(self.ptr.as_ptr(), gradient.raw().as_ptr());
        }
    }

    pub fn vertical_from_two(&mut self, top: Color, bottom: Color) {
        unsafe {
            visage_graphics_sys::VisageBrush_verticalFromTwo(
                self.ptr.as_ptr(),
                top.raw(),
                bottom.raw(),
            );
        }
    }

    pub fn linear(&mut self, gradient: &Gradient, from_x: f32, from_y: f32, to_x: f32, to_y: f32) {
        unsafe {
            visage_graphics_sys::VisageBrush_linear(
                self.ptr.as_ptr(),
                gradient.raw().as_ptr(),
                from_x,
                from_y,
                to_x,
                to_y,
            );
        }
    }

    pub fn linear_from_two(
        &mut self,
        from_color: Color,
        to_color: Color,
        from_x: f32,
        from_y: f32,
        to_x: f32,
        to_y: f32,
    ) {
        unsafe {
            visage_graphics_sys::VisageBrush_linearFromTwo(
                self.ptr.as_ptr(),
                from_color.raw(),
                to_color.raw(),
                from_x,
                from_y,
                to_x,
                to_y,
            );
        }
    }

    pub fn interpolate_with(&mut self, other: &Self, t: f32) {
        unsafe {
            visage_graphics_sys::VisageBrush_interpolateWith(
                self.ptr.as_ptr(),
                other.ptr.as_ptr(),
                t,
            );
        }
    }

    pub fn multiply_alpha(&mut self, mult: f32) {
        unsafe {
            visage_graphics_sys::VisageBrush_multiplyAlpha(self.ptr.as_ptr(), mult);
        }
    }

    pub fn raw(&self) -> NonNull<visage_graphics_sys::VisageBrush> {
        self.ptr
    }
}

impl Default for Brush {
    fn default() -> Self {
        Self::new()
    }
}

impl Drop for Brush {
    fn drop(&mut self) {
        unsafe {
            visage_graphics_sys::VisageBrush_delete(self.ptr.as_ptr());
        }
    }
}

impl Clone for Brush {
    fn clone(&self) -> Self {
        unsafe {
            Self {
                ptr: NonNull::new(visage_graphics_sys::VisageBrush_copy(self.ptr.as_ptr()))
                    .unwrap(),
            }
        }
    }
}
