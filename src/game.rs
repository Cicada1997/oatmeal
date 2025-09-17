use sdl2::Sdl;
use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use sdl2::render::Canvas;
use sdl2::video::Window;
use sdl2::VideoSubsystem;

use std::time::Duration;

use hecs::World;

use crate::constants::*;
use crate::components::square::Square;
use crate::components::vec2::Vec2;
use crate::components::render_target::RenderTarget;

type Position = Vec2;

pub struct Game {
    world: World,
    canvas: Canvas<Window>,
    // keep window for potential reuse
    window: Window,
    sdl_context: Sdl,
    video_subsystem: VideoSubsystem,
    event_pump: sdl2::EventPump,
    pos: Position,
    player: hecs::Entity,
    player_character: RenderTarget,
}

impl Game {
    pub fn new() -> Result<Self, Box<dyn std::error::Error>> {
        let sdl_context         = sdl2::init()?;
        let video_subsystem     = sdl_context.video()?;
        let window              = video_subsystem
            .window("SDL2 window", SCREEN_WIDTH, SCREEN_HEIGHT)
            .position_centered()
            .build()?;
        let canvas              = window.into_canvas().build()?;
        let event_pump          = sdl_context.event_pump()?;
        let mut world           = World::new();

        let player_character    = RenderTarget::new("src/assets/player/player.txt", 1);

        let sq                  = Square::new(100, 100, 50, 50);
        let player              = world.spawn((sq.clone(),));

        let window_handle       = canvas.window().clone();

        Ok(Self {
            world,
            canvas,
            window: window_handle,
            sdl_context,
            video_subsystem,
            event_pump,
            pos: Position::new(0.0, 0.0),
            player,
            player_character: player_character?,
        })
    }

    fn event_handler(&mut self) -> bool {
        for event in self.event_pump.poll_iter() {
            match event {
                Event::Quit { .. } => return false,
                Event::KeyDown { keycode: Some(Keycode::Escape), .. } => return false,
                _ => {}
            }
        }
        true
    }

    pub fn run(&mut self) -> Result<(), Box<dyn std::error::Error>> {
        let rt = std::env::args().nth(1)
            .and_then(|p| RenderTarget::new(&p, 16).ok());

        loop {
            if !self.event_handler() {
                break;
            }

            self.canvas.set_draw_color(Color::RGB(0, 0, 0));
            self.canvas.clear();

            if let Some(ref rt) = rt {
                for p in &rt.points {
                    self.canvas.set_draw_color(p.color);
                    let _ = self.canvas.fill_rect(p.rect);
                }
            }

            for (_ent, square) in &mut self.world.query::<&Square>() {
                self.canvas.set_draw_color(Color::RGB(square.color.0, square.color.1, square.color.2));
                let _ = self.canvas.fill_rect(square.to_sdl_rect());
            }

            for point in &self.player_character.points {
                self.canvas.set_draw_color(point.color);
                let _ = self.canvas.fill_rect(point.rect);
            }

            self.canvas.present();
            ::std::thread::sleep(Duration::from_millis(16));
        }

        Ok(())
    }
}

