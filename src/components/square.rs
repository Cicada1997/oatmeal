use sdl2::rect::Rect;

#[derive(Clone, Debug)]
pub struct Square {
    pub x: i32,
    pub y: i32,
    pub w: u32,
    pub h: u32,
    pub color: (u8, u8, u8),
}

impl Square {
    pub fn new(x: i32, y: i32, w: u32, h: u32) -> Self {
        Self { x, y, w, h, color: (255, 255, 255) }
    }

    pub fn to_sdl_rect(&self) -> Rect {
        Rect::new(self.x, self.y, self.w, self.h)
    }
}

