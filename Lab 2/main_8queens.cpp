
#include <iostream>
#include <vector>
#include <string>

// Function to print the chessboard
void printBoard(const std::vector<std::string>& board) {
    for (const std::string& row : board) {
        std::cout << row << std::endl;
    }
    std::cout << std::endl;
}

// Function to check if it's safe to place a queen at board[row][col]
bool isSafe(const std::vector<std::string>& board, int row, int col, int N) {
    // Check this row on the left side
    for (int i = 0; i < col; ++i) {
        if (board[row][i] == 'Q') {
            return false;
        }
    }

    // Check upper diagonal on left side
    for (int i = row, j = col; i >= 0 && j >= 0; --i, --j) {
        if (board[i][j] == 'Q') {
            return false;
        }
    }

    // Check lower diagonal on left side
    for (int i = row, j = col; i < N && j >= 0; ++i, --j) {
        if (board[i][j] == 'Q') {
            return false;
        }
    }

    return true;
}

// Recursive function to solve the N-Queens problem using backtracking
bool solveEightQueens(std::vector<std::string>& board, int col, int numberOfQueens) {
    // Base case: If all queens are placed, then a solution is found
    if (col >= numberOfQueens) {
        printBoard(board);
        return true;
    }

    // Consider this column and try placing this queen in all rows one by one
    for (int i = 0; i < numberOfQueens; ++i) {
        if (isSafe(board, i, col, numberOfQueens)) {
            // Place queen
            board[i][col] = 'Q';

            // Recur to place the rest of the queens
            bool foundSolution = solveEightQueens(board, col + 1, numberOfQueens);
            if(foundSolution){
                return true;
            } else {
                // Backtrack: Remove queen from board[i][col]
                board[i][col] = '-';
            }
           
        }
    }
}

int main() {
    const int numberOfQueens = 8; // For the 8 Queens problem
    std::vector<std::string> board(numberOfQueens, std::string(numberOfQueens, '-')); // Initialize empty board

    std::cout << "Solving the " << numberOfQueens << " Queens Problem..." << std::endl;
    solveEightQueens(board, 0, numberOfQueens);

    return 0;
}
