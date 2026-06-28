# Tetris Game (C++)

A real-time, console-based Tetris game built in **C++** as a first-year course project. The game runs entirely in the terminal using the Windows Console API for rendering and keyboard input, implementing the classic Tetris mechanics from scratch.

## Features

- **Tetromino movement** — move pieces left, right, and down in real time
- **Rotation** — rotate tetrominoes with collision-aware checks
- **Collision detection** — prevents pieces from overlapping or leaving the playing field
- **Line clearing** — completed rows are cleared automatically
- **Scoring** — points awarded as lines are cleared
- **Instant drop** — drop the current piece straight to the bottom
- **Game-over detection** — ends the game when pieces stack to the top

## Tech Stack

- **Language:** C++
- **Platform:** Windows (uses the Windows Console API for real-time input and rendering)
- **Concepts:** Object-oriented design, game-loop architecture, 2D grid manipulation, real-time keyboard handling

## Controls

| Key | Action |
|-----|--------|
| `A` / Left Arrow  | Move left |
| `D` / Right Arrow | Move right |
| `S` / Down Arrow  | Move down (soft drop) |
| `W` / Up Arrow    | Rotate piece |
| `Space`           | Instant drop |
| `Esc`             | Quit |

> Update this table to match the actual keys used in your code.

## How to Compile and Run

This project uses the Windows Console API, so it is built for Windows.

```bash
# Compile with g++ (MinGW)
g++ tetris.cpp -o tetris

# Run
tetris.exe
```

If you use a different compiler (e.g. MSVC), compile the single `tetris.cpp` source file as a console application.

## How It Works

The game maintains a 2D grid representing the playing field. On each tick of the game loop, the active tetromino attempts to move down; player input can shift or rotate it. Before any move is applied, a collision check verifies the target cells are empty and within bounds. When a tetromino can no longer move down, it locks into the grid, completed rows are detected and cleared, the score updates, and a new tetromino spawns at the top. The game ends when a newly spawned piece immediately collides.

## About

Built as a first-year course project to practice C++, object-oriented programming, and real-time game-loop logic.
