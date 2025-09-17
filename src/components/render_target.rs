use sdl2::pixels::Color;
use sdl2::rect::Rect;
use std::collections::HashMap;
use std::fs;

#[derive(Debug, Clone)]
pub struct Point {
    pub rect: Rect,
    pub color: Color,
}

#[derive(Debug, Clone)]
pub struct RenderTarget {
    pub points: Vec<Point>,
}


impl RenderTarget {
    pub fn new(path: &str, square_size: u32) -> Result<Self, Box<dyn std::error::Error>> {
        let contents = fs::read_to_string(path)?;
        let mut mode = Mode::None;
        let mut color_map: HashMap<char, Color> = HashMap::new();
        let mut map_rows: Vec<String> = Vec::new();
        for line in contents.lines() {
            let line = line.trim();
            if line.is_empty() {
                continue;
            }
            if let Some(rest) = line.strip_prefix('\\') {
                match rest.to_lowercase().as_str() {
                    "map" => mode = Mode::Map,
                    "colors" => mode = Mode::Colors,
                    _ => mode = Mode::None,
                }
                continue;
            }
            match mode {
                Mode::None => {}
                Mode::Map => map_rows.push(line.to_string()),
                Mode::Colors => {
                    if let Some((k, v)) = line.split_once(':') {
                        let key = k.trim().chars().next().unwrap_or('?');
                        let comps: Vec<u8> = v
                            .trim()
                            .split(',')
                            .filter_map(|s| s.trim().parse::<u8>().ok())
                            .collect();
                        if comps.len() == 3 {
                            color_map.insert(key, Color::RGB(comps[0], comps[1], comps[2]));
                        }
                    }
                }
            }
        }

        let mut points = Vec::new();
        for (row_idx, row) in map_rows.iter().enumerate() {
            for (col_idx, ch) in row.chars().enumerate() {
                let x = (col_idx as i32) * (square_size as i32);
                let y = (row_idx as i32) * (square_size as i32);
                let rect = Rect::new(x, y, square_size, square_size);
                let color = color_map.get(&ch).cloned().unwrap_or(Color::RGB(255, 255, 255));
                points.push(Point { rect, color });
            }
        }

        Ok(Self { points })
    }
}

enum Mode { None, Map, Colors }

