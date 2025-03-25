#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

#[inline]
pub fn VisageColor_fromAHSV(alpha: f32, hue: f32, saturation: f32, value: f32) -> VisageColor {
    // SAFETY: The value is initialized with data in the C function.
    unsafe {
        #[allow(invalid_value)]
        let mut color: VisageColor = std::mem::MaybeUninit::uninit().assume_init();
        VisageColor_fromAHSV_inner(alpha, hue, saturation, value, &mut color);
        color
    }
}

#[inline]
pub fn VisageColor_fromABGR(abgr: u32) -> VisageColor {
    // SAFETY: The value is initialized with data in the C function.
    unsafe {
        #[allow(invalid_value)]
        let mut color: VisageColor = std::mem::MaybeUninit::uninit().assume_init();
        VisageColor_fromABGR_inner(abgr, &mut color);
        color
    }
}

#[inline]
pub fn VisageColor_fromARGB(argb: u32) -> VisageColor {
    // SAFETY: The value is initialized with data in the C function.
    unsafe {
        #[allow(invalid_value)]
        let mut color: VisageColor = std::mem::MaybeUninit::uninit().assume_init();
        VisageColor_fromARGB_inner(argb, &mut color);
        color
    }
}