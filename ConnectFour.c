#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"
#define RESET   "\x1b[0m"
#define BOLD    "\x1b[1m"
#define UNDERLINE "\x1b[4m"

int depth;

#define ROWS 6
#define COLS 7
#define X_PIECE RED BOLD "X" RESET
#define O_PIECE YELLOW BOLD "O" RESET

// Declare the function prototypes
void initBoard(char board[ROWS][COLS]);
void printBoard(char board[ROWS][COLS]);
void testinitBoard();
void testDropPiece();
void testCheckWin();
void testGetAlignmentLength();
void testGetBestMove();
void testGetAIChoice();

// Declare the function prototypes
int dropPiece(char board[ROWS][COLS], int col, char piece);
int checkWin(char board[ROWS][COLS], char piece);
int getAIChoice(char board[ROWS][COLS], int lastPlayerMove);
int getBestMove(char board[ROWS][COLS], char player); // **Declare the function prototype**
int getAlignmentLength(char board[ROWS][COLS], int row, int col, char piece);
int evaluateBoard(char board[ROWS][COLS]);
int minimax(char board[ROWS][COLS], int depth, int alpha, int beta, int isMaximizing);



int main() {
    char board[ROWS][COLS];
    srand(time(NULL));

    testinitBoard();
    testDropPiece();
    testCheckWin();
    testGetAlignmentLength();
    testGetBestMove();
    testGetAIChoice();

    printf("\n\n\n\n\n");
    int turn, col, validMove, gameMode;
    char player;
    char playAgain;
    int startingPlayer = 0; // 0 for 'X', 1 for 'O'
    int lastPlayerMove = -1;  // Track last player's move

    /**
     * Prompts the user to choose a game mode (Player vs Player or Player vs AI).
     * Validates the input to ensure it is an integer and either 1 or 2.
     * If the input is invalid, it prompts the user again until a valid input is received.
     */
    while (1) {
        printf(CYAN BOLD"Choose game mode:\n1. Player vs Player\n2. Player vs AI\nEnter choice: " RESET);

        // Check if the input is a valid integer
        if (scanf("%d", &gameMode) != 1) {
            // Clear the invalid input from the buffer
            while (getchar() != '\n');
            printf(RED BOLD"\nInvalid input. Please enter a number (1 or 2).\n\n" RESET);
            continue;
        }
        
        if (gameMode == 2) {
            printf(MAGENTA "Enter the AI difficulty (1 - 5):\n" RESET);
            printf(RED BOLD"The higher the difficulty, the slower the AI will play as it thinks deeper: " RESET);
            scanf("%d", &depth);
            printf("\n");
        
            // Ensure the depth is within a valid range
            if (depth < 1) depth = 1;
            if (depth > 5) depth = 5;
            printf(MAGENTA BOLD "Chosen difficulty: %d\n", depth);
        }

        if (gameMode == 1 || gameMode == 2)
        {
            break; // valid input, exit the loop
        } 
        else 
        {
            printf(RED BOLD"\nInvalid choice. Please enter 1 or 2.\n\n" RESET);
        }
        
    }

    /**
     * Main game loop for Connect Four.
     * Initializes the game board and alternates turns between players.
     * Provides advice on the best move for both players.
     * Handles both Player vs Player and Player vs AI game modes.
     * Checks for win conditions and handles end-of-game scenarios.
     * Prompts the user to play again and swaps the starting player for each new game.
     */
    do {
        initBoard(board);
        turn = 0;
        player = (startingPlayer % 2 == 0) ? 'X' : 'O'; // Swap the starting player each session

        while (1) {
            printBoard(board);
            printf(WHITE BOLD "Turn: Player %c\n\n", player);

            // **Show best move advice for both players**
            int adviceCol = getBestMove(board, player);
            printf(MAGENTA BOLD "Advice: Best column to play is %d!\n\n" RESET, adviceCol + 1);

            if (gameMode == 2 && player == 'O') { // AI turn
                col = getAIChoice(board, lastPlayerMove);
                dropPiece(board, col, player);
                printf("AI chooses column %d\n", col + 1);
                
            } else { // Human turn
                printf(CYAN BOLD "Player %c, enter column (1-7): " RESET, player);
                if (scanf("%d", &col) != 1) {
                    // Clear the input buffer
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf(RED BOLD "Invalid input. Please enter a number.\n" RESET);
                    continue; // Skip to the next iteration of the loop
                }
                getchar();
                col--;

                if (col < 0 || col >= COLS) {
                    printf(RED BOLD"Invalid column. Choose between 1 and 7.\n" RESET);
                    continue;
                }

                validMove = dropPiece(board, col, player);
                if (!validMove) {
                    printf(RED BOLD "Column is full. Try again.\n" RESET);
                    continue;
                }
            }

            // **Check for a win**
            if (checkWin(board, player)) {
                printBoard(board);
                printf(YELLOW UNDERLINE BOLD"Player %c wins!\n"RESET, player);
                break;
            }

            turn++;
            if (turn == ROWS * COLS) {
                printBoard(board);
                printf(YELLOW UNDERLINE"It's a draw!\n" RESET);
                break;
            }

            // **Switch to next player**
            player = (player == 'X') ? 'O' : 'X';
        }

        // **Ask if the user wants to play again**
        printf(CYAN "Do you want to play again? (y/n): " RESET);
        scanf(" %c", &playAgain);

        // Swap starting player for the next session
        startingPlayer = (startingPlayer + 1) % 2;

    } while (playAgain == 'y' || playAgain == 'Y');

    printf(MAGENTA "Thanks for playing!\n");
    return 0;
}

/**
 * Initializes the game board by setting all positions to an empty space (' ').
 *
 * This function sets up the game board by filling each cell with a space character,
 * indicating that the cell is empty and ready for gameplay.
 *
 * @param board The game board represented as a 2D array of characters.
 */
void initBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = ' ';
}

/**
 * Prints the current state of the game board to the console.
 *
 * @param board The game board represented as a 2D array of characters.
 */
 void printBoard(char board[ROWS][COLS]) {
    printf(BOLD "\n   1   2   3   4   5   6   7\n" RESET);
    for (int i = 0; i < ROWS; i++) {
        printf(CYAN " | " RESET);
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == 'X') printf(X_PIECE CYAN" | " RESET);
            else if (board[i][j] == 'O') printf(O_PIECE CYAN" | " RESET);
            else printf(CYAN "  | " RESET);
        }
        printf("\n");
    }
    printf(CYAN " |---|---|---|---|---|---|---|\n\n" RESET);
}

/**
 * Drops a piece into the specified column on the game board.
 *
 * This function attempts to place the given piece ('X' or 'O') into the specified column.
 * It starts from the bottom of the column and places the piece in the first available empty slot.
 * If the column is full, it returns 0 indicating failure; otherwise, it returns 1 indicating success.
 *
 * @param board The game board represented as a 2D array of characters.
 * @param col The column where the piece should be dropped.
 * @param piece The piece to be dropped ('X' or 'O').
 * @return 1 if the piece was successfully dropped, 0 if the column is full.
 */
int dropPiece(char board[ROWS][COLS], int col, char piece) {
    // Check if the column is valid
    if (col < 0 || col >= COLS) {
        return 0; // Invalid column
    }

    // Attempt to drop the piece in the specified column
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == ' ') {
            board[i][col] = piece;
            return 1; // Success
        }
    }

    return 0; // Column is full
}

/**
 * Checks if the specified piece has won the game.
 *
 * This function checks for a winning condition by looking for four consecutive pieces
 * of the same type (either 'X' or 'O') in horizontal, vertical, and both diagonal directions.
 *
 * @param board The game board represented as a 2D array of characters.
 * @param piece The piece to check for a win ('X' or 'O').
 * @return 1 if the specified piece has won, 0 otherwise.
 */
int checkWin(char board[ROWS][COLS], char piece) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] != piece) continue;
            
            if (j + 3 < COLS && board[i][j + 1] == piece && board[i][j + 2] == piece && board[i][j + 3] == piece)
                return 1;
            
            if (i + 3 < ROWS && board[i + 1][j] == piece && board[i + 2][j] == piece && board[i + 3][j] == piece)
                return 1;
            
            if (i + 3 < ROWS && j + 3 < COLS && board[i + 1][j + 1] == piece && board[i + 2][j + 2] == piece && board[i + 3][j + 3] == piece)
                return 1;
            
            if (i - 3 >= 0 && j + 3 < COLS && board[i - 1][j + 1] == piece && board[i - 2][j + 2] == piece && board[i - 3][j + 3] == piece)
                return 1;
        }
    }
    return 0;
}

/**
 * Calculates the maximum alignment length of the specified piece starting from a given position.
 *
 * This function checks in four possible directions (horizontal, vertical, diagonal \, and diagonal /)
 * to determine the longest sequence of the specified piece starting from the given row and column.
 * It helps in evaluating the board state for potential winning moves or threats.
 *
 * @param board The game board represented as a 2D array of characters.
 * @param row The starting row position.
 * @param col The starting column position.
 * @param piece The piece to check for alignment ('X' or 'O').
 * @return The maximum alignment length of the specified piece.
 */
int getAlignmentLength(char board[ROWS][COLS], int row, int col, char piece) {
    int maxCount = 0;
    
    // Directions: horizontal, vertical, diagonal (\), diagonal (/)
    int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

    for (int d = 0; d < 4; d++) {
        int dr = directions[d][0], dc = directions[d][1];
        int count = 1;

        // Check in both directions
        for (int i = 1; i < 4; i++) {
            int r = row + dr * i, c = col + dc * i;
            if (r >= 0 && r < ROWS && c >= 0 && c < COLS && board[r][c] == piece)
                count++;
            else
                break;
        }
        for (int i = 1; i < 4; i++) {
            int r = row - dr * i, c = col - dc * i;
            if (r >= 0 && r < ROWS && c >= 0 && c < COLS && board[r][c] == piece)
                count++;
            else
                break;
        }

        if (count > maxCount)
            maxCount = count;
    }
    
    return maxCount;
}

/**
 * Determines the best column for the AI to place its piece using the minimax algorithm.
 *
 * The function simulates each possible move for the AI, evaluates the resulting board state using the minimax algorithm,
 * and selects the move with the highest score. If no strategic move is found, it picks the first available column.
 *
 * @param board The game board represented as a 2D array of characters.
 * @return The column index (0-based) where the AI should place its piece.
 */
int getAIChoice(char board[ROWS][COLS], int lastPlayerMove) {
    int bestMove = -1;
    int bestScore = -10000;

    for (int col = 0; col < COLS; col++) {
        for (int row = ROWS - 1; row >= 0; row--) {
            if (board[row][col] == ' ') {
                board[row][col] = 'O';  // Simulate AI move
                int score = minimax(board, depth, -10000, 10000, 0); // Adjust depth as needed
                board[row][col] = ' ';  // Undo move

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = col;
                }
                break;
            }
        }
    }

    // If no strategic move is found, pick the first available column
    if (bestMove == -1) {
        for (int col = 0; col < COLS; col++) {
            if (board[0][col] == ' ') {
                bestMove = col;
                break;
            }
        }
    }

    return bestMove;
}

/**
 * Implements the minimax algorithm with alpha-beta pruning to determine the best move for the AI.
 *
 * The minimax algorithm is a recursive algorithm used for decision-making and game theory. It provides
 * an optimal move for the player assuming that the opponent is also playing optimally. The algorithm
 * explores all possible moves and their outcomes to find the best move for the AI.
 *
 * Alpha-beta pruning is an optimization technique for the minimax algorithm that reduces the number of
 * nodes evaluated in the search tree. It keeps track of two values, alpha and beta, which represent the
 * minimum score that the maximizing player is assured of and the maximum score that the minimizing player
 * is assured of, respectively. If at any point the current move is worse than the previously examined move,
 * it stops evaluating that move.
 *
 * @param board The game board represented as a 2D array of characters.
 * @param depth The current depth of the search tree.
 * @param alpha The best value that the maximizer currently can guarantee at that level or above.
 * @param beta The best value that the minimizer currently can guarantee at that level or above.
 * @param isMaximizing A flag indicating whether the current move is maximizing (1) or minimizing (0).
 * @return The evaluation score of the board.
 */
int minimax(char board[ROWS][COLS], int depth, int alpha, int beta, int isMaximizing) {
    if (checkWin(board, 'O')) return 1000 - depth;
    if (checkWin(board, 'X')) return -1000 + depth;
    if (depth == 0) return evaluateBoard(board); // Stop at max depth

    if (isMaximizing) {
        int maxEval = -10000;
        for (int col = 0; col < COLS; col++) {
            for (int row = ROWS - 1; row >= 0; row--) {
                if (board[row][col] == ' ') {
                    board[row][col] = 'O';
                    int eval = minimax(board, depth - 1, alpha, beta, 0);
                    board[row][col] = ' ';
                    maxEval = (eval > maxEval) ? eval : maxEval;
                    alpha = (eval > alpha) ? eval : alpha;
                    if (beta <= alpha) break;
                }
            }
        }
        return maxEval;
    } else {
        int minEval = 10000;
        for (int col = 0; col < COLS; col++) {
            for (int row = ROWS - 1; row >= 0; row--) {
                if (board[row][col] == ' ') {
                    board[row][col] = 'X';
                    int eval = minimax(board, depth - 1, alpha, beta, 1);
                    board[row][col] = ' ';
                    minEval = (eval < minEval) ? eval : minEval;
                    beta = (eval < beta) ? eval : beta;
                    if (beta <= alpha) break;
                }
            }
        }
        return minEval;
    }
}

/**
 * Evaluates the current state of the game board and returns a score.
 * This function assigns a score to the board based on the positions of the pieces.
 * It favors center column placements and evaluates all possible alignments (horizontal, vertical, diagonal).
 * The AI's pieces are given positive scores, while the player's pieces are given negative scores.
 *
 * @param board The game board represented as a 2D array of characters.
 * @return The evaluation score of the board.
 */
int evaluateBoard(char board[ROWS][COLS]) {
    int score = 0;

    // Center column preference
    for (int i = 0; i < ROWS; i++) {
        if (board[i][COLS / 2] == 'O') score += 5;  // Favor AI center placement
        if (board[i][COLS / 2] == 'X') score -= 5;  // Discourage player center control
    }

    // Check all possible alignments
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == ' ') continue;

            char piece = board[i][j];
            int pieceValue = (piece == 'O') ? 1 : -1; // AI is positive, Player is negative

            // Evaluate all four possible directions
            int horizontal = 0, vertical = 0, diagonal1 = 0, diagonal2 = 0;

            // Horizontal (→)
            if (j + 3 < COLS) {
                for (int k = 0; k < 4; k++) 
                    if (board[i][j + k] == piece) horizontal++;
            }

            // Vertical (↓)
            if (i + 3 < ROWS) {
                for (int k = 0; k < 4; k++) 
                    if (board[i + k][j] == piece) vertical++;
            }

            // Diagonal (\)
            if (i + 3 < ROWS && j + 3 < COLS) {
                for (int k = 0; k < 4; k++) 
                    if (board[i + k][j + k] == piece) diagonal1++;
            }

            // Diagonal (/)
            if (i - 3 >= 0 && j + 3 < COLS) {
                for (int k = 0; k < 4; k++) 
                    if (board[i - k][j + k] == piece) diagonal2++;
            }

            // Assign scores based on alignment length
            int alignments[] = {horizontal, vertical, diagonal1, diagonal2};
            for (int a = 0; a < 4; a++) {
                switch (alignments[a]) {
                    case 4: score += 1000 * pieceValue; break; // Win condition
                    case 3: score += 50 * pieceValue; break;   // Strong threat
                    case 2: score += 10 * pieceValue; break;   // Weak threat
                }
            }
        }
    }

    return score;
}

/**
 * Determines the best move for the player by evaluating potential moves.
 *
 * This function evaluates the board to determine the best column for the player to place their piece.
 * It first checks if the player can win in the next move, then checks if the opponent can win in the next move
 * and suggests blocking. It also prioritizes forming 3-in-a-row with an open space for future 4-in-a-row.
 * If no strategic move is found, it picks a random valid column.
 *
 * @param board The game board represented as a 2D array of characters.
 * @param player The player making the move ('X' or 'O').
 * @return The column index (0-based) where the player should place their piece.
 */
int getBestMove(char board[ROWS][COLS], char player) {
    int bestCol = -1, maxAlignment = 0;

    // 1. Check if the player can win in the next move
    for (int col = 0; col < COLS; col++) {
        for (int row = ROWS - 1; row >= 0; row--) {
            if (board[row][col] == ' ') {
                board[row][col] = player;
                if (checkWin(board, player)) {
                    board[row][col] = ' ';
                    return col;  // Best move is the winning move
                }
                board[row][col] = ' ';
                break;
            }
        }
    }

    // 2. Check if the opponent can win in the next move and block it
    char opponent = (player == 'X') ? 'O' : 'X';
    for (int col = 0; col < COLS; col++) {
        for (int row = ROWS - 1; row >= 0; row--) {
            if (board[row][col] == ' ') {
                board[row][col] = opponent;
                if (checkWin(board, opponent)) {
                    board[row][col] = ' ';
                    return col;  // Block the opponent's win
                }
                board[row][col] = ' ';
                break;
            }
        }
    }

    // 3. Prevent opponent from setting up a double-attack (two winning options)
    for (int col = 0; col < COLS; col++) {
        for (int row = ROWS - 1; row >= 0; row--) {
            if (board[row][col] == ' ') {
                board[row][col] = opponent;
                int threats = 0;
                for (int nextCol = 0; nextCol < COLS; nextCol++) {
                    for (int nextRow = ROWS - 1; nextRow >= 0; nextRow--) {
                        if (board[nextRow][nextCol] == ' ') {
                            board[nextRow][nextCol] = opponent;
                            if (checkWin(board, opponent)) {
                                threats++;
                            }
                            board[nextRow][nextCol] = ' ';
                            break;
                        }
                    }
                }
                board[row][col] = ' ';
                if (threats > 1) {
                    return col;  // Block a double attack setup
                }
                break;
            }
        }
    }

    // 4. Prioritize forming 3-in-a-row with an open space for a future win
    for (int col = 0; col < COLS; col++) {
        for (int row = ROWS - 1; row >= 0; row--) {
            if (board[row][col] == ' ') {
                int alignment = getAlignmentLength(board, row, col, player);
                if (alignment == 3) {
                    bestCol = col;  // Setup a win next turn
                    break;
                }
                if (alignment > maxAlignment) {
                    maxAlignment = alignment;
                    bestCol = col;
                }
                break;
            }
        }
    }

    // 5. If no strategic move is found, pick a random valid column
    if (bestCol == -1) {
        do {
            bestCol = rand() % COLS;
        } while (board[0][bestCol] != ' ');
    }

    return bestCol;
}




// TEST FUNCTIONS



/**
 * Tests the initBoard function to ensure it correctly initializes the game board.
 * Creates a board and calls initBoard to set all positions to spaces (' ').
 * Verifies that every position in the board is a space by checking each cell.
 * Prints "initBoard PASSED" if all cells are spaces, "initBoard FAILED" otherwise.
 */
void testinitBoard() {
    char board[ROWS][COLS];
    initBoard(board);

    int pass = 1;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] != ' ') {
                pass = 0;
            }
        }
    }

    printf(pass ? "initBoard PASSED\n" : "initBoard FAILED\n");
}

/**
 * Tests the functionality of the dropPiece function through multiple scenarios.
 * This function performs four tests:
 * 1. Drops a single 'X' in column 3 and verifies it lands at the bottom.
 * 2. Stacks 'O' and 'X' in column 3, checking correct stacking order.
 * 3. Attempts to drop pieces in invalid columns (-1 and 10), expecting failure.
 * 4. Fills column 2 with 'O' pieces and tests that an additional 'X' is rejected.
 * Prints detailed test results for each step and a final "PASSED" or "FAILED" message.
 * If any test fails, the function exits early with a failure message.
 */
void testDropPiece() {
    char board[ROWS][COLS];
    initBoard(board);

    // Test 1
    int success = dropPiece(board, 3, 'X');
    printf("Test 1: success=%d, board[5][3]=%c\n", success, board[5][3]);
    if (!success || board[ROWS - 1][3] != 'X') {
        printf("dropPiece FAILED (Expected 'X' in row %d, col 3)\n", ROWS - 1);
        return;
    }

    // Test 2
    dropPiece(board, 3, 'O');
    dropPiece(board, 3, 'X');
    printf("Test 2: board[4][3]=%c, board[3][3]=%c\n", board[4][3], board[3][3]);
    if (board[ROWS - 2][3] != 'O' || board[ROWS - 3][3] != 'X') {
        printf("dropPiece FAILED (Stacking pieces incorrectly)\n");
        return;
    }

    // Test 3
    int fail = dropPiece(board, -1, 'X') || dropPiece(board, 10, 'X');
    printf("Test 3: fail=%d\n", fail);
    if (fail) {
        printf("dropPiece FAILED (Did not handle invalid columns correctly)\n");
        return;
    }

    // Test 4
    for (int i = 0; i < ROWS; i++) {
        dropPiece(board, 2, 'O');
        printf("Filling column 2, row %d: %c\n", 5 - i, board[5 - i][2]);
    }
    int full = dropPiece(board, 2, 'X');
    printf("Test 4: full=%d\n", full);
    if (full) {
        printf("dropPiece FAILED (Allowed piece to be placed in a full column)\n");
        return;
    }

    printf("dropPiece PASSED\n");
}

/**
 * Tests the checkWin function for detecting winning conditions in Connect Four.
 * This function performs three tests:
 * 1. Places four 'X' pieces horizontally in columns 0-3 and checks for a win.
 * 2. Places four 'X' pieces vertically in column 3 and checks for a win.
 * 3. Places four 'X' pieces diagonally (bottom-left to top-right) and checks for a win.
 * Prints "PASSED" or "FAILED" for each test case (Horizontal, Vertical, Diagonal).
 * Resets the board between tests to ensure independence.
 */
void testCheckWin() {
    char board[ROWS][COLS];
    initBoard(board);

    // Horizontal Win
    dropPiece(board, 0, 'X');
    dropPiece(board, 1, 'X');
    dropPiece(board, 2, 'X');
    dropPiece(board, 3, 'X');

    if (checkWin(board, 'X')) {
        printf("checkWin (Horizontal) PASSED\n");
    } else {
        printf("checkWin (Horizontal) FAILED\n");
    }

    initBoard(board);

    // Vertical Win
    dropPiece(board, 3, 'X');
    dropPiece(board, 3, 'X');
    dropPiece(board, 3, 'X');
    dropPiece(board, 3, 'X');

    if (checkWin(board, 'X')) {
        printf("checkWin (Vertical) PASSED\n");
    } else {
        printf("checkWin (Vertical) FAILED\n");
    }

    initBoard(board);

    // Diagonal Win
    board[5][0] = 'X';
    board[4][1] = 'X';
    board[3][2] = 'X';
    board[2][3] = 'X';

    if (checkWin(board, 'X')) {
        printf("checkWin (Diagonal) PASSED\n");
    } else {
        printf("checkWin (Diagonal) FAILED\n");
    }
}

/**
 * Tests the getAlignmentLength function for calculating the longest sequence of pieces.
 * Sets up a board with three 'X' pieces horizontally at the bottom (columns 0-2).
 * Checks if the alignment length from the middle position (row 5, column 1) is 3.
 * Prints "PASSED" if the length is 3, "FAILED" otherwise.
 */
void testGetAlignmentLength() {
    char board[ROWS][COLS];
    initBoard(board);

    board[5][0] = 'X';
    board[5][1] = 'X';
    board[5][2] = 'X';
    
    int length = getAlignmentLength(board, 5, 1, 'X');
    
    if (length == 3) {
        printf("getAlignmentLength PASSED\n");
    } else {
        printf("getAlignmentLength FAILED\n");
    }
}

/**
 * Tests the getBestMove function for identifying a winning move.
 * Sets up a board with three 'X' pieces in columns 0-2.
 * Checks if the best move for 'X' is column 3, which would complete a horizontal win.
 * Prints "PASSED" if the best move is 3, "FAILED" with the returned value otherwise.
 */
void testGetBestMove() {
    char board[ROWS][COLS];
    initBoard(board);

    dropPiece(board, 0, 'X');
    dropPiece(board, 1, 'X');
    dropPiece(board, 2, 'X');

    int bestMove = getBestMove(board, 'X');

    if (bestMove == 3) {
        printf("getBestMove PASSED\n");
    } else {
        printf("getBestMove FAILED (Expected 3, got %d)\n", bestMove);
    }
}

/**
 * Tests the getAIChoice function for selecting a winning move for the AI.
 * Sets up a board with three 'O' pieces in columns 0-2.
 * Checks if the AI chooses column 3 to complete a horizontal win for 'O'.
 * Prints "PASSED" if the AI move is 3, "FAILED" with the returned value otherwise.
 * Uses -1 as the last player move to indicate no prior move influence.
 */
/**
 * Tests the getAIChoice function for various scenarios.
 * Verifies that the AI makes the correct move to win, block, or play strategically.
 */
void testGetAIChoice() {
    char board[ROWS][COLS];
    initBoard(board);

    dropPiece(board, 0, 'O');
    dropPiece(board, 1, 'O');
    dropPiece(board, 2, 'O');

    int aiMove = getAIChoice(board, -1);

    if (aiMove == 3) {
        printf("getAIChoice PASSED\n");
    } else {
        printf("getAIChoice FAILED (Expected 3, got %d)\n", aiMove);
    }
}
