use std::ptr::NonNull;

use crate::color::Color;

pub struct Gradient {
    ptr: NonNull<visage_graphics_sys::VisageGradient>,
    resolution: usize,
}

impl Gradient {
    pub fn new() -> Self {
        unsafe {
            Self {
                ptr: NonNull::new(visage_graphics_sys::VisageGradient_new()).unwrap(),
                resolution: 0,
            }
        }
    }

    pub fn from_colors(colors: &[Color]) -> Self {
        let mut new_self = Self::new();

        new_self.set_resolution(colors.len());

        for (i, color) in colors.iter().enumerate() {
            new_self.set_color(i, *color).unwrap();
        }

        new_self
    }

    pub fn set_resolution(&mut self, resolution: usize) {
        if self.resolution != resolution {
            self.resolution = resolution;
            unsafe {
                visage_graphics_sys::VisageGradient_setResolution(
                    self.ptr.as_ptr(),
                    resolution as i32,
                );
            }
        }
    }

    pub fn resolution(&self) -> usize {
        self.resolution
    }

    pub fn color(&self, index: usize) -> Option<Color> {
        if index >= self.resolution {
            return None;
        }

        // SAFETY: The value is initialized with data in the C function.
        unsafe {
            #[allow(invalid_value)]
            let mut color: visage_graphics_sys::VisageColor =
                std::mem::MaybeUninit::uninit().assume_init();
            visage_graphics_sys::VisageGradient_getColor(
                self.ptr.as_ptr(),
                index as i32,
                &mut color,
            );

            Some(Color::from_raw(color))
        }
    }

    pub fn set_color(&self, index: usize, color: Color) -> Result<(), ()> {
        if index >= self.resolution {
            return Err(());
        }

        unsafe {
            visage_graphics_sys::VisageGradient_setColor(
                self.ptr.as_ptr(),
                index as i32,
                color.raw(),
            );
        }

        Ok(())
    }

    pub fn interpolate_with(&mut self, other: &Self, t: f32) {
        debug_assert!(t >= 0.0 && t <= 1.0);

        unsafe {
            visage_graphics_sys::VisageGradient_interpolateWith(
                self.ptr.as_ptr(),
                other.ptr.as_ptr(),
                t,
            );
        }
    }

    pub fn multiply_alpha(&mut self, mult: f32) {
        unsafe {
            visage_graphics_sys::VisageGradient_multiplyAlpha(self.ptr.as_ptr(), mult);
        }
    }

    pub fn raw(&self) -> NonNull<visage_graphics_sys::VisageGradient> {
        self.ptr
    }
}

impl Default for Gradient {
    fn default() -> Self {
        Self::new()
    }
}

impl Drop for Gradient {
    fn drop(&mut self) {
        unsafe {
            visage_graphics_sys::VisageGradient_delete(self.ptr.as_ptr());
        }
    }
}

impl Clone for Gradient {
    fn clone(&self) -> Self {
        unsafe {
            Self {
                ptr: NonNull::new(visage_graphics_sys::VisageGradient_copy(self.ptr.as_ptr()))
                    .unwrap(),
                resolution: self.resolution,
            }
        }
    }
}

impl PartialEq for Gradient {
    fn eq(&self, other: &Self) -> bool {
        if self.resolution != other.resolution {
            return false;
        }

        for i in 0..self.resolution {
            let color_a = self.color(i).unwrap();
            let color_b = self.color(i).unwrap();

            if color_a != color_b {
                return false;
            }
        }

        true
    }
}
