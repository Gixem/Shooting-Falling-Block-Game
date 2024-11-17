# Shooting-Falling-Block-Game

A simple 2D game where players control a laser that shoots upward to hit falling boxes. The game includes box movement, laser firing, and collision detection.

## Features

- **Falling Boxes:** Boxes fall from the top of the screen and reset when they reach the bottom.
- **Laser Shooting:** The player can fire a laser by pressing the spacebar.
- **Collision Detection:** When the laser hits a box, the box disappears.
- **Game Loop:** The game continuously updates the screen with new positions for the boxes and the laser.

## Requirements

This project uses the `icb_gui.h` library for graphical user interface components and basic game handling. Ensure that the necessary libraries and dependencies are set up on your system to run this project.

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/your-username/laser-shooting-game.git
   cd laser-shooting-game
   ```

2. **Build the project**:
   Follow the instructions in your development environment to compile the code. You will need the ICBYTES library set up to handle graphical rendering.

3. **Run the game**:
   After building the project, run the executable to start the game.

## How to Play

1. Press the **Spacebar** to fire a laser.
2. Try to hit the falling boxes with the laser.
3. If the laser hits a box, the box disappears.
4. Boxes reset when they fall off the screen.

## Code Structure

- **Main Game Loop:** Handles the creation, movement, and rendering of boxes and the laser.
- **Collision Detection:** Checks if the laser hits a box and deactivates it upon collision.
- **Threading:** Uses threads to handle laser movement and box movement simultaneously.
- **Graphics:** Uses `icb_gui.h` for rendering the game elements on the screen.
