extern crate sdl2;

use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use sdl2::rect::Rect;
use std::time::Duration;

mod constants;
use constants::*;

// const SCREEN_X: u32 = 800;
// const SCREEN_Y: u32 = 600;

macro_rules! init_sdl {
    // Initialize SDL2
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();

    // Create a window
    let window = video_subsystem
        .window("SDL2 Window", SCREEN_X, SCREEN_Y)
        .position_centered()
        .build()
        .unwrap();

    // Create a canvas
    let mut canvas = window.into_canvas().build().unwrap();
    let mut event_pump = sdl_context.event_pump().unwrap();
}

fn main() {
    init_sdl!();

    // Main loop
    'main: loop {
        // Handle events
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit { .. } | Event::KeyDown { keycode: Some(Keycode::Escape), .. } => {
                    break 'main;
                }
                _ => {}
            }
        }

        // Clear the canvas
        canvas.set_draw_color(Color::RGB(0, 0, 0));
        canvas.clear();

        // Present the canvas
        canvas.present();

        // Wait for a short duration
        ::std::thread::sleep(Duration::from_millis(16));
    }
}

