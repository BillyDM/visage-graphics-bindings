# Visage-Graphics Language Bindings
Experimental language bindings to the hardware-accelerated [bgfx](https://github.com/bkaradzic/bgfx)-based 2D graphics engine from the [Visage](https://github.com/VitalAudio/visage) GUI library. Note this *ONLY* binds to the `visage_graphics` module inside Visage and not to the entire Visage library.

*This repository is currently a work in progress and is very experimental. There is no guarantee that these bindings will ever be finished.*

Currently this repository provides bindings for C and Rust. Feel free to open a PR with any language bindings you wish to see! (Once the C bindings are complete of course.)

# Building

[CMake](https://cmake.org/) is required. Also check the [bgfx documentation](https://bkaradzic.github.io/bgfx/build.html) for a list of other dependencies.

## Building the C examples:

```
mkdir build && cd build
cmake ../
make
```

## Building the Rust examples:

```
cd visage-graphics-rs
cargo run --example basic
```