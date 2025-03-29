use std::ptr::NonNull;
use std::rc::Rc;

pub use widestring::{Utf32Str, Utf32String, utf32str};

#[derive(Clone)]
pub struct Font {
    inner: Rc<FontInner>,
}

impl Font {
    pub unsafe fn new_from_file_bytes(font_data: &[u8], size: f32, dpi_scale: f32) -> Self {
        let ptr = unsafe {
            NonNull::new(visage_graphics_sys::VisageFont_new(
                size,
                font_data.as_ptr() as *const i8,
                font_data.len() as i32,
                dpi_scale,
            ))
            .unwrap()
        };

        Self {
            inner: Rc::new(FontInner { ptr }),
        }
    }

    pub fn new_lato_regular(size: f32, dpi_scale: f32) -> Self {
        let ptr = unsafe {
            NonNull::new(visage_graphics_sys::VisageFont_LatoRegular(size, dpi_scale)).unwrap()
        };

        Self {
            inner: Rc::new(FontInner { ptr }),
        }
    }

    pub fn new_droid_sans_mono(size: f32, dpi_scale: f32) -> Self {
        let ptr = unsafe {
            NonNull::new(visage_graphics_sys::VisageFont_DroidSansMono(
                size, dpi_scale,
            ))
            .unwrap()
        };

        Self {
            inner: Rc::new(FontInner { ptr }),
        }
    }

    pub fn new_twemoji_mozilla(size: f32, dpi_scale: f32) -> Self {
        let ptr = unsafe {
            NonNull::new(visage_graphics_sys::VisageFont_TwemojiMozilla(
                size, dpi_scale,
            ))
            .unwrap()
        };

        Self {
            inner: Rc::new(FontInner { ptr }),
        }
    }

    pub fn with_dpi_scale(&self, dpi_scale: f32) -> Self {
        let ptr = unsafe {
            NonNull::new(visage_graphics_sys::VisageFont_withDpiScale(
                self.inner.ptr.as_ptr(),
                dpi_scale,
            ))
            .unwrap()
        };

        Self {
            inner: Rc::new(FontInner { ptr }),
        }
    }

    pub fn dpi_scale(&self) -> f32 {
        unsafe { visage_graphics_sys::VisageFont_getDpiScale(self.inner.ptr.as_ptr()) }
    }

    pub fn width_overflow_index(
        &self,
        str: &Utf32Str,
        width: f32,
        round: bool,
        character_override: i32,
    ) -> i32 {
        unsafe {
            visage_graphics_sys::VisageFont_widthOverflowIndex(
                self.inner.ptr.as_ptr(),
                str.as_ptr(),
                str.len() as i32,
                width,
                round,
                character_override,
            )
        }
    }

    pub fn line_breaks<const MAX_LINE_BREAKS: usize>(
        &self,
        str: &Utf32Str,
        width: f32,
    ) -> Vec<i32> {
        let mut breaks: Vec<i32> = Vec::with_capacity(MAX_LINE_BREAKS);

        unsafe {
            let len = visage_graphics_sys::VisageFont_lineBreaks(
                self.inner.ptr.as_ptr(),
                str.as_ptr(),
                str.len() as i32,
                width,
                breaks.as_mut_ptr(),
                MAX_LINE_BREAKS as i32,
            );

            assert!(len >= 0);

            if len as usize > MAX_LINE_BREAKS {
                todo!();
            }

            breaks.set_len((len as usize).min(MAX_LINE_BREAKS));
        }

        breaks
    }

    pub fn string_width(&self, str: &Utf32Str, character_override: i32) -> f32 {
        unsafe {
            visage_graphics_sys::VisageFont_stringWidth(
                self.inner.ptr.as_ptr(),
                str.as_ptr(),
                str.len() as i32,
                character_override,
            )
        }
    }

    pub fn line_height(&self) -> f32 {
        unsafe { visage_graphics_sys::VisageFont_lineHeight(self.inner.ptr.as_ptr()) }
    }

    pub fn capital_height(&self) -> f32 {
        unsafe { visage_graphics_sys::VisageFont_capitalHeight(self.inner.ptr.as_ptr()) }
    }

    pub fn lower_dip_height(&self) -> f32 {
        unsafe { visage_graphics_sys::VisageFont_lowerDipHeight(self.inner.ptr.as_ptr()) }
    }

    pub fn size(&self) -> f32 {
        unsafe { visage_graphics_sys::VisageFont_size(self.inner.ptr.as_ptr()) }
    }

    pub fn raw_font_data(&self) -> &[u8] {
        unsafe {
            let mut data_size = 0;
            let data_ptr =
                visage_graphics_sys::VisageFont_fontData(self.inner.ptr.as_ptr(), &mut data_size);

            assert!(data_size > 0);
            assert!(!data_ptr.is_null());

            std::slice::from_raw_parts(data_ptr as *const u8, data_size as usize)
        }
    }

    pub fn raw(&self) -> NonNull<visage_graphics_sys::VisageFont> {
        self.inner.ptr
    }
}

impl PartialEq for Font {
    fn eq(&self, other: &Self) -> bool {
        Rc::ptr_eq(&self.inner, &other.inner)
    }
}

impl Eq for Font {}

struct FontInner {
    ptr: NonNull<visage_graphics_sys::VisageFont>,
}

impl Drop for FontInner {
    fn drop(&mut self) {
        unsafe {
            visage_graphics_sys::VisageFont_delete(self.ptr.as_ptr());
        }
    }
}
