use std::ffi::CString;

#[repr(transparent)]
#[derive(Debug, Clone, Copy)]
pub struct Color(visage_graphics_sys::VisageColor);

impl Color {
    pub const BLACK: Self = Self(visage_graphics_sys::VisageColor {
        values: [0.0, 0.0, 0.0, 1.0],
        hdr: 1.0,
    });
    pub const WHITE: Self = Self(visage_graphics_sys::VisageColor {
        values: [1.0, 1.0, 1.0, 1.0],
        hdr: 1.0,
    });
    pub const TRANSPARENT: Self = Self(visage_graphics_sys::VisageColor {
        values: [0.0, 0.0, 0.0, 0.0],
        hdr: 1.0,
    });

    pub fn from_raw(raw: visage_graphics_sys::VisageColor) -> Self {
        Self(raw)
    }

    pub fn raw(&self) -> visage_graphics_sys::VisageColor {
        self.0
    }

    pub fn from_ahsv(alpha: f32, hue: f32, saturation: f32, value: f32) -> Self {
        Self(visage_graphics_sys::VisageColor_fromAHSV(
            alpha, hue, saturation, value,
        ))
    }

    pub const fn from_linear(r: f32, g: f32, b: f32, a: f32) -> Self {
        debug_assert!(r >= 0.0 && r <= 1.0);
        debug_assert!(g >= 0.0 && g <= 1.0);
        debug_assert!(b >= 0.0 && b <= 1.0);
        debug_assert!(a >= 0.0 && a <= 1.0);

        Self(visage_graphics_sys::VisageColor {
            values: [b, g, r, a],
            hdr: 1.0,
        })
    }

    pub fn from_abgr(abgr: u32) -> Self {
        Self(visage_graphics_sys::VisageColor_fromABGR(abgr))
    }

    pub fn from_argb(argb: u32) -> Self {
        Self(visage_graphics_sys::VisageColor_fromARGB(argb))
    }

    pub fn from_hex_string(hex: &str) -> Self {
        let mut color = visage_graphics_sys::VisageColor {
            values: [0.0; 4],
            hdr: 0.0,
        };

        unsafe {
            visage_graphics_sys::VisageColor_fromHexString_inner(
                CString::new(hex).unwrap().as_ptr(),
                &mut color,
            );
        }

        Self(color)
    }

    pub fn to_abgr(&self) -> u32 {
        unsafe { visage_graphics_sys::VisageColor_toABGR(&self.0) }
    }

    pub fn to_argb(&self) -> u32 {
        unsafe { visage_graphics_sys::VisageColor_toABGR(&self.0) }
    }

    pub fn to_rgb(&self) -> u32 {
        unsafe { visage_graphics_sys::VisageColor_toABGR(&self.0) }
    }

    pub fn b(&self) -> f32 {
        self.0.values[visage_graphics_sys::VisageColorChannel_VisageColorChannelBlue as usize]
    }

    pub fn g(&self) -> f32 {
        self.0.values[visage_graphics_sys::VisageColorChannel_VisageColorChannelGreen as usize]
    }

    pub fn r(&self) -> f32 {
        self.0.values[visage_graphics_sys::VisageColorChannel_VisageColorChannelRed as usize]
    }

    pub fn a(&self) -> f32 {
        self.0.values[visage_graphics_sys::VisageColorChannel_VisageColorChannelAlpha as usize]
    }

    pub fn hdr(&self) -> f32 {
        self.0.hdr
    }

    #[inline]
    pub fn set_blue(&mut self, val: f32) {
        self.0.values[visage_graphics_sys::VisageColorChannel_VisageColorChannelBlue as usize] =
            val.clamp(0.0, 1.0);
    }

    #[inline]
    pub fn set_green(&mut self, val: f32) {
        self.0.values[visage_graphics_sys::VisageColorChannel_VisageColorChannelGreen as usize] =
            val.clamp(0.0, 1.0);
    }

    #[inline]
    pub fn set_red(&mut self, val: f32) {
        self.0.values[visage_graphics_sys::VisageColorChannel_VisageColorChannelRed as usize] =
            val.clamp(0.0, 1.0);
    }

    #[inline]
    pub fn set_alpha(&mut self, val: f32) {
        self.0.values[visage_graphics_sys::VisageColorChannel_VisageColorChannelAlpha as usize] =
            val.clamp(0.0, 1.0);
    }

    #[inline]
    pub fn set_hdr(&mut self, val: f32) {
        self.0.hdr = val.max(0.0);
    }

    pub fn value(&self) -> f32 {
        self.b().max(self.g()).max(self.r())
    }

    pub fn saturation(&self) -> f32 {
        unsafe { visage_graphics_sys::VisageColor_saturation(&self.0) }
    }

    pub fn hue(&self) -> f32 {
        unsafe { visage_graphics_sys::VisageColor_hue(&self.0) }
    }

    #[inline]
    pub fn interpolate(a: &Self, b: &Self, t: f32) -> Self {
        debug_assert!(t >= 0.0 && t <= 1.0);

        Self(visage_graphics_sys::VisageColor {
            values: [
                a.0.values[0] + (b.0.values[0] - a.0.values[0]) * t,
                a.0.values[1] + (b.0.values[1] - a.0.values[1]) * t,
                a.0.values[2] + (b.0.values[2] - a.0.values[2]) * t,
                a.0.values[3] + (b.0.values[3] - a.0.values[3]) * t,
            ],
            hdr: a.0.hdr + (b.0.hdr - a.0.hdr) * t,
        })
    }
}

impl Ord for Color {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        for (&ch, &other_ch) in self.0.values.iter().zip(other.0.values.iter()) {
            if ch < other_ch {
                return std::cmp::Ordering::Less;
            }
            if ch > other_ch {
                return std::cmp::Ordering::Greater;
            }
        }

        if self.0.hdr < other.0.hdr {
            std::cmp::Ordering::Less
        } else if self.0.hdr > other.0.hdr {
            std::cmp::Ordering::Greater
        } else {
            std::cmp::Ordering::Equal
        }
    }
}

impl PartialOrd for Color {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl PartialEq for Color {
    fn eq(&self, other: &Self) -> bool {
        self.0.values == other.0.values && self.0.hdr == other.0.hdr
    }
}

impl Eq for Color {}

impl From<u32> for Color {
    fn from(value: u32) -> Self {
        Color::from_argb(value)
    }
}
