extern crate sdl2;
extern crate hecs;

use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use sdl2::rect::Rect;
use std::time::Duration;

use hecs::*;

mod components;
use components::*;

mod constants;
use constants::*;

mod game;
use game::*;

type Error = Box<dyn std::error::Error + Send + Sync + 'static>;

fn run() -> Result<(), Error>{
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();

    let window = video_subsystem
        .window("SDL2 Window", SCREEN_WIDTH, SCREEN_HEIGHT)
        .position_centered()
        .build()
        .unwrap();

    let mut canvas = window.into_canvas().build().unwrap();
    let mut event_pump = sdl_context.event_pump().unwrap();

    'main: loop {
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit { .. } | Event::KeyDown { keycode: Some(Keycode::Escape), .. } => {
                    break 'main Ok(());
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

fn main() {
    let res = run();

    match res {
        Ok(_) => {}
        Err(e) => { 
            eprintln!("{}", e)
        }
    }
}
