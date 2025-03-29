use std::ptr::NonNull;

use widestring::Utf32Str;

use crate::font::Font;

#[repr(i32)]
#[derive(Default, Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum Direction {
    Left = 0,
    #[default]
    Up,
    Right,
    Down,
}

bitflags::bitflags! {
    #[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
    pub struct Justification: u32 {
        const CENTER = visage_graphics_sys::VisageJustification_kVisageJustificationCenter;
        const LEFT = visage_graphics_sys::VisageJustification_kVisageJustificationLeft;
        const Right = visage_graphics_sys::VisageJustification_kVisageJustificationRight;
        const Top = visage_graphics_sys::VisageJustification_kVisageJustificationTop;
        const Bottom = visage_graphics_sys::VisageJustification_kVisageJustificationBottom;
        const TopLeft = visage_graphics_sys::VisageJustification_kVisageJustificationTopLeft;
        const BottomLeft = visage_graphics_sys::VisageJustification_kVisageJustificationBottomLeft;
        const TopRight = visage_graphics_sys::VisageJustification_kVisageJustificationTopRight;
        const BottomRight = visage_graphics_sys::VisageJustification_kVisageJustificationBottomRight;
    }
}

pub struct Text {
    ptr: NonNull<visage_graphics_sys::VisageText>,
    font: Font,
}

impl Text {
    pub fn new(font: &Font) -> Self {
        let ptr = unsafe {
            NonNull::new(visage_graphics_sys::VisageText_new(font.raw().as_ptr())).unwrap()
        };

        Self {
            ptr,
            font: font.clone(),
        }
    }

    pub fn set_text(&mut self, s: &Utf32Str) {
        unsafe {
            visage_graphics_sys::VisageText_setTextU32WithLength(
                self.ptr.as_ptr(),
                s.as_ptr(),
                s.len() as i32,
            );
        }
    }

    pub fn text(&self) -> &Utf32Str {
        unsafe {
            let mut string_length = 0;

            let s_ptr =
                visage_graphics_sys::VisageText_getTextU32(self.ptr.as_ptr(), &mut string_length);

            if s_ptr.is_null() || string_length <= 0 {
                return &Utf32Str::from_slice_unchecked(&[]);
            } else {
                let s_slice = std::slice::from_raw_parts(s_ptr, string_length as usize);
                Utf32Str::from_slice_unchecked(s_slice)
            }
        }
    }

    pub fn set_font(&mut self, font: &Font) {
        if &self.font == font {
            return;
        }

        self.font = font.clone();

        unsafe {
            visage_graphics_sys::VisageText_setFont(self.ptr.as_ptr(), font.raw().as_ptr());
        }
    }

    pub fn set_justification(&mut self, justification: Justification) {
        unsafe {
            visage_graphics_sys::VisageText_setJustification(
                self.ptr.as_ptr(),
                justification.bits(),
            );
        }
    }

    pub fn justification(&self) -> Justification {
        unsafe {
            Justification::from_bits_retain(visage_graphics_sys::VisageText_getJustification(
                self.ptr.as_ptr(),
            ))
        }
    }

    pub fn set_multi_line(&mut self, multi_line: bool) {
        unsafe {
            visage_graphics_sys::VisageText_setMultiLine(self.ptr.as_ptr(), multi_line);
        }
    }

    pub fn multi_line(&self) -> bool {
        unsafe { visage_graphics_sys::VisageText_getMultiLine(self.ptr.as_ptr()) }
    }

    pub fn set_character_override(&mut self, character_override: i32) {
        unsafe {
            visage_graphics_sys::VisageText_setCharacterOverride(
                self.ptr.as_ptr(),
                character_override,
            );
        }
    }

    pub fn character_override(&self) -> i32 {
        unsafe { visage_graphics_sys::VisageText_getCharacterOverride(self.ptr.as_ptr()) }
    }

    pub fn raw(&self) -> NonNull<visage_graphics_sys::VisageText> {
        self.ptr
    }
}

impl Clone for Text {
    fn clone(&self) -> Self {
        let ptr = unsafe {
            NonNull::new(visage_graphics_sys::VisageText_copy(self.ptr.as_ptr())).unwrap()
        };

        Self {
            ptr,
            font: self.font.clone(),
        }
    }
}

impl Drop for Text {
    fn drop(&mut self) {
        unsafe {
            visage_graphics_sys::VisageText_delete(self.ptr.as_ptr());
        }
    }
}
