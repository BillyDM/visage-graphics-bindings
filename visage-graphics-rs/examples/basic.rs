use std::error::Error;
use std::os::raw::c_void;

use winit::application::ApplicationHandler;
use winit::dpi::LogicalSize;
use winit::event::WindowEvent;
use winit::event_loop::{ActiveEventLoop, EventLoop};
use winit::raw_window_handle::{
    HasDisplayHandle, HasWindowHandle, RawDisplayHandle, RawWindowHandle,
};
use winit::window::{Window, WindowAttributes, WindowId};

use visage_graphics_rs::visage_graphics_sys;

#[derive(Default)]
struct App {
    state: Option<WindowState>,
}

struct WindowState {
    window: Window,
    canvas: *mut visage_graphics_sys::VisageCanvas,
}

impl Drop for App {
    fn drop(&mut self) {
        if let Some(state) = &self.state {
            unsafe {
                visage_graphics_sys::VisageCanvas_destroy(state.canvas);
            }
        }
    }
}

impl ApplicationHandler for App {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        if self.state.is_none() {
            let window = event_loop
                .create_window(
                    WindowAttributes::default().with_inner_size(LogicalSize::new(800, 600)),
                )
                .unwrap();

            let window_size = window.inner_size();
            let dpi_scale = window.scale_factor() as f32;

            let raw_display = window.display_handle().unwrap();
            let raw_window = window.window_handle().unwrap();

            let canvas = unsafe {
                let raw_display_ptr: *mut c_void = match raw_display.as_raw() {
                    RawDisplayHandle::Xlib(handle) => handle.display.unwrap().as_ptr(),
                    _ => todo!(),
                };

                let raw_window_ptr: *mut c_void = match raw_window.as_raw() {
                    RawWindowHandle::Xlib(handle) => handle.window as *mut c_void,
                    _ => todo!(),
                };

                visage_graphics_sys::VisageRenderer_checkInitialization(
                    raw_window_ptr,
                    raw_display_ptr,
                );

                dbg!(visage_graphics_sys::VisageRenderer_initialized());
                dbg!(visage_graphics_sys::VisageRenderer_supported());
                dbg!(visage_graphics_sys::VisageRenderer_swapChainSupported());

                let canvas = visage_graphics_sys::VisageCanvas_new();

                visage_graphics_sys::VisageCanvas_pairToWindow(
                    canvas,
                    raw_window_ptr,
                    window_size.width as i32,
                    window_size.height as i32,
                );

                visage_graphics_sys::VisageCanvas_setDpiScale(canvas, dpi_scale);

                canvas
            };

            window.request_redraw();

            self.state = Some(WindowState { window, canvas });
        }
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, _: WindowId, event: WindowEvent) {
        match event {
            WindowEvent::CloseRequested => {
                event_loop.exit();
            }
            WindowEvent::RedrawRequested => {
                let Some(state) = &self.state else {
                    return;
                };

                let window_size = state.window.inner_size();
                let window_width = window_size.width as f32;
                let window_height = window_size.height as f32;

                unsafe {
                    visage_graphics_sys::VisageCanvas_clearDrawnShapes(state.canvas);

                    visage_graphics_sys::VisageCanvas_setColor(
                        state.canvas,
                        visage_graphics_sys::VisageColor_fromARGB(0xff000066),
                    );
                    visage_graphics_sys::VisageCanvas_fill(
                        state.canvas,
                        0.0,
                        0.0,
                        window_width,
                        window_height,
                    );

                    let circle_radius = window_height * 0.1;
                    let x = window_width * 0.5 - circle_radius;
                    let y = window_height * 0.5 - circle_radius;
                    visage_graphics_sys::VisageCanvas_setColor(
                        state.canvas,
                        visage_graphics_sys::VisageColor_fromARGB(0xff00ffff),
                    );
                    visage_graphics_sys::VisageCanvas_circle(
                        state.canvas,
                        x,
                        y,
                        2.0 * circle_radius,
                    );

                    // Notify that you're about to draw.
                    state.window.pre_present_notify();

                    visage_graphics_sys::VisageCanvas_submit(state.canvas, 0);
                }

                // For contiguous redraw loop you can request a redraw from here.
                //state.window.request_redraw();
            }
            WindowEvent::Resized(new_physical_size) => {
                let Some(state) = &self.state else {
                    return;
                };

                unsafe {
                    visage_graphics_sys::VisageCanvas_setDimensions(
                        state.canvas,
                        new_physical_size.width as i32,
                        new_physical_size.height as i32,
                    );
                }

                state.window.request_redraw();
            }
            WindowEvent::ScaleFactorChanged {
                scale_factor,
                inner_size_writer: _,
            } => {
                let Some(state) = &self.state else {
                    return;
                };

                unsafe {
                    visage_graphics_sys::VisageCanvas_setDpiScale(
                        state.canvas,
                        scale_factor as f32,
                    );
                }

                state.window.request_redraw();
            }
            _ => (),
        }
    }
}

fn main() -> Result<(), Box<dyn Error>> {
    let event_loop = EventLoop::new()?;

    event_loop.run_app(&mut App::default())?;

    Ok(())
}
