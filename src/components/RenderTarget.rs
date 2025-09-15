use crate::Vec2::*;
use crate::Square::*;
use sdl2::rect::Rect as SDL_rect;

use std::env;
use std::fs;

pub struct RenderTarget {
    points:      Vec<Vec2>,
    // square_size: u32,
};

impl RenderTarget {
    pub fn new(&self, path: Str&, square_size: u32) -> &Self {
        let args: Vec<String> = env::args().collect();

        let query = &args[1];
        let file_path = &args[2];

        let contents = fs::read_to_string(path);

        let current_x;
        let current_y;
        
        for c in contents {
            if c == '\n' {
                current_y += square_size;
                continue;
            }

            points.push_back(SDL_rect::new(current_x, current_y));
            current_x += square_size;
        }
    }
}
