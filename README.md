# Baba Is You — C++ Implementation

A C++ implementation of the puzzle game **Baba Is You**, in which the rules of each level are written as word blocks on the grid (e.g. `BABA IS YOU`, `WALL IS STOP`, `FLAG IS WIN`). Pushing these blocks changes the rules of the game in real time.

## Features

- Grid-based game engine with movement and chain-pushing mechanics
- **Dynamic rule system**: rules are detected on the grid (horizontally and vertically) and re-evaluated after every move
- Properties such as `YOU`, `WIN`, `STOP`, `PUSH`, `SINK`, `KILL`
- Object transformation rules (e.g. `ROCK IS FLAG`) 
- Levels loaded from text files
- Graphical interface with SFML

## Design

- **Object-oriented architecture**: [classes, e.g. `Board`, `Cell`, `Entity`, `Game`]
- Separation between game logic and display 

## Tech stack

**C++** 11 · Makefile · SFML

## How to build and run

```bash
make
```

## Authors

OOP project in C++, Paris Cité University.

Steven Lahoud, Yasmine Bouzoumita
