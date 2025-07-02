
#include <iostream>
#include <vector>
#include <string>

const char queenChar = 'Q';
// Function that displays the chess board. we use - to "make it"
void printBoard(const std::vector<std::string>& board) {
    for (const std::string& row : board) {
        std::cout << row << std::endl;
    }
    std::cout << std::endl;
}

// Function to check if it's safe to place a queen at board[row][column]
bool isSafe(const std::vector<std::string>& board, int row, int column, int N) {
    // Check this row on the left side
    for (int i = 0; i < column; ++i) {
        if (board[row][i] == queenChar) {
            return false;
        }
    }

    // Check upper diagonal on left side
    for (int i = row, j = column; i >= 0 && j >= 0; --i, --j) {
        if (board[i][j] == queenChar) {
            return false;
        }
    }

    // Check lower diagonal on left side
    for (int i = row, j = column; i < N && j >= 0; ++i, --j) {
        if (board[i][j] == queenChar) {
            return false;
        }
    }

    return true;
}

// Recursive function to solve the N-Queens problem using backtracking
bool solveEightQueens(std::vector<std::string>& board, int column, int numberOfQueens) {
    // Base case: If all queens are placed, then a solution is found
    if (column >= numberOfQueens) {
        printBoard(board);
        return true;
    }

    // Consider this column and try placing this queen in all rows one by one
    for (int i = 0; i < numberOfQueens; ++i) {
        if (isSafe(board, i, column, numberOfQueens)) {
            // Place queen
            board[i][column] = queenChar;

            // This is the recursive step
            bool foundSolution = solveEightQueens(board, column + 1, numberOfQueens);
            if(foundSolution){
                return true;
            } else {
                // Backtrack: Remove queen from board[i][column] // also creates the board
                board[i][column] = '-';
            }
           
        }
    }
}

int main() {
    const int numberOfQueens = 8; // For the 8 Queens problem
    std::vector<std::string> board(numberOfQueens, std::string(numberOfQueens, '-')); // Initialize empty board // creates the board
    solveEightQueens(board, 0, numberOfQueens);
    return 0;
}
