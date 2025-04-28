# Connect Four Game in C

This repository contains a C implementation of the classic **Connect Four** game. The game can be played in two modes:

- **Player vs Player (PvP)**: Two players take turns to drop discs into the columns.
- **Player vs AI (PvAI)**: The player competes against an AI that uses a minimax algorithm with alpha-beta pruning to make strategic moves.

## Features

- **Player vs Player Mode**: Two human players can play against each other.
- **Player vs AI Mode**: Play against an AI that calculates optimal moves.
- **Terminal-Based Interface**: The game is played entirely in the terminal, with the board and user interface rendered in a simple text format.
- **Minimax Algorithm**: The AI uses the minimax algorithm with alpha-beta pruning to evaluate and select the best moves.
- **Scoring System**: Tracks the winner and displays the result after each game.

## Requirements

- A C compiler (such as GCC) to compile the code.
- A terminal or command-line interface to run the game.

## How to Compile and Run

### 1. **Clone the repository**:
   ```bash
   git clone https://github.com/MRL-creator/ConnectFour.git
   ```
### 2. **Navigate to the project folder**:
   ```bash
   cd ConnectFour
   ```
### 3. **Compile the game**:
   ```bash
   gcc -o connect4 connect4.c
   ```
### 4. **Run the game**:
   ```bash
   ./connect4
   ```
## How to Play

### Player vs Player (PvP) Mode
- The first player is represented by `X`, and the second player is represented by `O`.
- Players take turns to drop their disc in one of the columns.
- The first player to form a horizontal, vertical, or diagonal line of four discs wins the game.

### Player vs AI (PvAI) Mode
- You will play as `X`, and the AI will play as `O`.
- You can choose a column to drop your disc, and the AI will calculate the best move based on the minimax algorithm.
- The first to connect four discs wins.

## Minimax Algorithm

The AI uses the **minimax algorithm** with **alpha-beta pruning** to make decisions. The AI evaluates potential moves using a heuristic evaluation function, which helps it choose the most strategic move. The search tree is pruned to improve efficiency.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Feel free to fork this repository, contribute improvements, and open issues for suggestions or bug reports. If you want to add more features or enhancements, make sure to open a pull request!

## Acknowledgements

- The **minimax algorithm** and **alpha-beta pruning** are essential components for AI gameplay in this project.
- Thanks to the C programming community for their open-source contributions and continuous improvement of algorithms in game development.


