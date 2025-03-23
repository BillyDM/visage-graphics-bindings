use std::env;
use std::path::PathBuf;

fn main() {
    // Build the static library with CMake.
    let mut config = cmake::Config::new("../../visage-graphics-c");
    config.define("BUILD_SHARED_LIBS", "OFF");

    // Disable building the static library with the Debug profile.
    if config.get_profile() == "Debug" {
        config.profile("RelWithDebInfo");
    };

    // Link C++ standard library & platform-specific libraries.
    #[cfg(target_os = "linux")]
    {
        println!("cargo:rustc-link-lib=dylib=stdc++");
    }

    let dst = config.build().join("build");
    
    // Tell cargo to link to the compiled library.
    println!("cargo:rustc-link-search=native={}", dst.display());
    println!("cargo:rustc-link-lib=static=VisageGraphicsC");

    // Link compiled bgfx libraries.
    println!("cargo:rustc-link-search=native={}", dst.clone().join("_deps/bgfx-build/cmake/bgfx").display());
    println!("cargo:rustc-link-lib=static=bgfx");
    println!("cargo:rustc-link-search=native={}", dst.clone().join("_deps/bgfx-build/cmake/bimg").display());
    println!("cargo:rustc-link-lib=static=bimg");
    println!("cargo:rustc-link-search=native={}", dst.clone().join("_deps/bgfx-build/cmake/bx").display());
    println!("cargo:rustc-link-lib=static=bx");

    // Link compiled freetype library.
    println!("cargo:rustc-link-search=native={}", dst.clone().join("_deps/freetype-build").display());
    println!("cargo:rustc-link-lib=static=freetype");

    // Link compiled nanosvg library.
    println!("cargo:rustc-link-search=native={}", dst.clone().join("_deps/visage-build/visage_graphics/third_party/nanosvg").display());
    println!("cargo:rustc-link-lib=static=nanosvg");
    println!("cargo:rustc-link-lib=static=nanosvgrast");

    // Link embedded content.
    println!("cargo:rustc-link-search=native={}", dst.clone().join("_deps/visage-build/visage_graphics").display());
    println!("cargo:rustc-link-lib=static=VisageEmbeddedFonts");
    println!("cargo:rustc-link-lib=static=VisageEmbeddedIcons");
    println!("cargo:rustc-link-lib=static=VisageEmbeddedShaders");

    // Generate Rust bindings from the C header.
    let bindings = bindgen::Builder::default()
        .header("../../visage-graphics-c/visage_graphics_c.h")
        // Tell cargo to invalidate the built crate whenever any of the
        // included header files changed.
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");

    // Write the bindings to the $OUT_DIR/bindings.rs file.
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap()).join("bindings.rs");
    bindings
        .write_to_file(out_path)
        .expect("Couldn't write bindings!");
}