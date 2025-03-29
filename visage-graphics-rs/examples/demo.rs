use std::error::Error;

use visage_graphics_rs::brush::Brush;
use visage_graphics_rs::canvas::Canvas;
use visage_graphics_rs::color::Color;
use visage_graphics_rs::font::Font;
use visage_graphics_rs::gradient::Gradient;
use visage_graphics_rs::text::{Direction, Justification, Text};
use widestring::utf32str;
use winit::application::ApplicationHandler;
use winit::dpi::LogicalSize;
use winit::event::WindowEvent;
use winit::event_loop::{ActiveEventLoop, EventLoop};
use winit::window::{Window, WindowAttributes, WindowId};

#[derive(Default)]
struct App {
    state: Option<WindowState>,
}

struct WindowState {
    canvas: Canvas,
    font: Font,
    window: Window,
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

            let mut canvas = Canvas::new();
            unsafe {
                canvas.pair_to_window(&window, window_size.width, window_size.height, dpi_scale);
            }

            window.request_redraw();

            let font = Font::new_lato_regular(16.0, 1.0);

            self.state = Some(WindowState {
                window,
                font,
                canvas,
            });
        }
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, _: WindowId, event: WindowEvent) {
        match event {
            WindowEvent::CloseRequested => {
                event_loop.exit();
            }
            WindowEvent::RedrawRequested => {
                let Some(state) = &mut self.state else {
                    return;
                };

                let window_size = state.window.inner_size();
                let window_width = window_size.width as f32;
                let window_height = window_size.height as f32;

                state.canvas.clear_drawn_shapes();

                state.canvas.set_color(0xff000066);
                state.canvas.fill(0.0, 0.0, window_width, window_height);

                let circle_radius = window_height * 0.1;
                let x = window_width * 0.5 - circle_radius;
                let y = window_height * 0.5 - circle_radius;

                let gradient = Gradient::from_colors(&[0xff00ffff.into(), 0xff0000ff.into()]);
                let mut brush = Brush::new();
                brush.horizontal(&gradient);
                state.canvas.set_brush(&brush);

                state.canvas.circle(x, y, 2.0 * circle_radius);

                state.canvas.set_color(Color::WHITE);

                let mut text = Text::new(&state.font);
                text.set_text(utf32str!("Hello World!"));
                text.set_justification(Justification::LEFT);
                state
                    .canvas
                    .text(&text, 20.0, 20.0, 100.0, 30.0, Direction::default());

                // Notify that you're about to draw.
                state.window.pre_present_notify();

                state.canvas.submit(0);

                // For contiguous redraw loop you can request a redraw from here.
                //state.window.request_redraw();
            }
            WindowEvent::Resized(new_physical_size) => {
                let Some(state) = &mut self.state else {
                    return;
                };

                state
                    .canvas
                    .set_dimensions(new_physical_size.width, new_physical_size.height);

                state.window.request_redraw();
            }
            WindowEvent::ScaleFactorChanged {
                scale_factor,
                inner_size_writer: _,
            } => {
                let Some(state) = &mut self.state else {
                    return;
                };

                state.canvas.set_dpi_scale(scale_factor as f32);
                state.font = state.font.with_dpi_scale(scale_factor as f32);

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
