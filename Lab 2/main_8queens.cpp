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
void solveNQueens(std::vector<std::string>& board, int col, int N, int& solutionCount) {
    // Base case: If all queens are placed, then a solution is found
    if (col >= N) {
        solutionCount++;
        std::cout << "Solution " << solutionCount << ":" << std::endl;
        printBoard(board);
        return;
    }

    // Consider this column and try placing this queen in all rows one by one
    for (int i = 0; i < N; ++i) {
        if (isSafe(board, i, col, N)) {
            // Place queen
            board[i][col] = 'Q';

            // Recur to place the rest of the queens
            solveNQueens(board, col + 1, N, solutionCount);

            // Backtrack: Remove queen from board[i][col]
            board[i][col] = '.';
        }
    }
}

int main() {
    const int N = 8; // For the 8 Queens problem
    std::vector<std::string> board(N, std::string(N, '.')); // Initialize empty board
    int solutionCount = 0;

    std::cout << "Solving the " << N << " Queens Problem..." << std::endl;
    solveNQueens(board, 0, N, solutionCount);

    if (solutionCount == 0) {
        std::cout << "No solutions found for " << N << " Queens." << std::endl;
    } else {
        std::cout << "Total solutions found for " << N << " Queens: " << solutionCount << std::endl;
    }

    return 0;
}
