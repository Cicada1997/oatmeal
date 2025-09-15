pub struct Vec2 {
    pub x: f64,
    pub y: f64,
}

impl Vec2 {
    pub fn new(x: f64, y: f64) -> Self {
        Self { x, y }
    }

    pub fn length(&self) -> f64 {
        (self.x*self.x + self.y*self.y).sqrt()
    }

    pub fn norm(&self) -> Self {
        let len = self.length();
        if len == 0.0 { 
            return Self { x: 0.0, y: 0.0 };

        } else {
            return Self { x: self.x / len, y: self.y / len };
        }
    }
}
