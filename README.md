
# Plants vs Zombies in C++

## Project Overview

This project is a simplified version of the popular game "Plants vs Zombies" implemented in C++ using Object-Oriented Programming (OOP) concepts. The game features a graphical user interface built with SFML (Simple and Fast Multimedia Library). The primary goal is to defend your garden from waves of incoming zombies by strategically placing various types of plants that attack or block the zombies.

## Features

- **Graphical User Interface**: Utilizes SFML to create a visually engaging game window.
- **Object-Oriented Design**: Implements core game mechanics using OOP principles such as encapsulation, inheritance, and polymorphism.
- **Game Loop**: Manages game state, events, and rendering through a well-structured main game loop.
- **Plants and Zombies**: Different types of plants with unique abilities to combat various zombie types.
- **Background and Sprites**: Dynamically loads and displays textures and sprites for the game background and characters.

## Classes and Design

### `Game`
- Manages the main game loop, event handling, and rendering.
- Initializes the game window and background.

### `Entity`
- Base class for all game entities (plants, zombies).
- Contains common attributes like position and texture.

### `Plant` (inherits from `Entity`)
- Represents different types of plants.
- Defines behavior and actions of plants (e.g., shooting, blocking).

### `Zombie` (inherits from `Entity`)
- Represents different types of zombies.
- Defines behavior and actions of zombies (e.g., movement, attack).

### `TextureManager`
- Manages loading and storing textures to avoid redundant loading.

## Getting Started

### Prerequisites
- C++ Compiler (GCC, Clang, MSVC)
- SFML Library

### Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/plants-vs-zombies-cpp.git
    cd plants-vs-zombies-cpp
    ```

2. Install SFML:
    Follow the instructions on the [SFML website](https://www.sfml-dev.org/download.php) to install SFML for your operating system.

3. Build the project:
    ```sh
    g++ -o game main.cpp -lsfml-graphics -lsfml-window -lsfml-system
    ```

4. Run the game:
    ```sh
    ./game
    ```

## Screenshots

![Game Screenshot](path_to_screenshot)

## Contributing

Contributions are welcome! Please fork the repository and submit pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [SFML](https://www.sfml-dev.org/) for providing a simple and fast multimedia library.
- The original "Plants vs Zombies" game for inspiration.
