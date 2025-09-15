use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use sdl2::rect::Rect;
use sdl2::render::{
    Canvas as SDL_Canvas,
};

use sdl2::video::{
    Window as SDL_Window,
};

use std::time::Duration;

use hecs::{
    World as ECSWorld,
};

use crate::components::*;
use crate::Vec2::Vec2;
use crate::Square::Square;

type Position = Vec2;

pub struct Game {
    world: ECSWorld,
}

impl Game {
    pub fn new(&mut self) -> &Self {
        self.world = ECSWorld::new();

        let pos = Position::new(0.0, 0.0);
        let sqr = Square::new(0, 0, 50, 50);

        let player = self.world.spawn((sqr,));

        self
    }

    pub fn run() {

    }

    pub fn draw(&mut self, mut canvas: SDL_Canvas<SDL_Window>) {
        let mut squares = self.world.query_mut::<(&Square)>()
            .into_iter()
            .collect::<Vec<_>>();

        for (entity, square) in squares {
            canvas.fill_rect(square.to_SDL_rect());
        }
    }
}
