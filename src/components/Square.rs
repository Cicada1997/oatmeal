use crate::Vec2::Vec2;
use sdl2::rect::Rect as SDL_Rect;

pub struct Square {
    x: i32,
    y: i32,
    w: u32,
    h: u32,
}

impl Square {
    pub fn new(x: i32, y: i32, w: u32, h: u32) -> Self {
        Self { x, y, w, h }
    }

    pub fn pos(&self) -> Vec2 {
        Vec2::new(self.x as f64, self.y as f64)
    }

    pub fn to_SDL_rect(&self) -> SDL_Rect {
        SDL_Rect::new(
            self.x,
            self.y,
            self.w,
            self.h
        )
    }
}

