mod components;
mod constants;
mod game;
// mod vec2;
// mod square;

use game::Game;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut game = Game::new()?;
    game.run()?;
    Ok(())
}

