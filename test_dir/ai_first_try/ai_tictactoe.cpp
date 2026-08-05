#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const char HUMAN = 'X';
const char AI = 'O';
const char EMPTY = ' ';

void printBoard(const vector<char>& board) {
    cout << "\n";
    for (int i = 0; i < 9; i++) {
        cout << board[i];
        if ((i + 1) % 3 == 0) cout << "\n";
        else cout << " | ";
    }
    cout << "\n";
}

bool isMovesLeft(const vector<char>& board) {
    for (char c : board) if (c == EMPTY) return true;
    return false;
}

int evaluate(const vector<char>& b) {
    // Rows
    for (int row = 0; row < 3; row++) {
        if (b[row*3] == b[row*3+1] && b[row*3+1] == b[row*3+2]) {
            if (b[row*3] == AI) return +10;
            else if (b[row*3] == HUMAN) return -10;
        }
    }
    // Columns
    for (int col = 0; col < 3; col++) {
        if (b[col] == b[col+3] && b[col+3] == b[col+6]) {
            if (b[col] == AI) return +10;
            else if (b[col] == HUMAN) return -10;
        }
    }
    // Diagonals
    if (b[0] == b[4] && b[4] == b[8]) {
        if (b[0] == AI) return +10;
        else if (b[0] == HUMAN) return -10;
    }
    if (b[2] == b[4] && b[4] == b[6]) {
        if (b[2] == AI) return +10;
        else if (b[2] == HUMAN) return -10;
    }
    return 0;
}

int minimax(vector<char>& board, int depth, bool isMax) {
    int score = evaluate(board);
    if (score == 10 || score == -10) return score;
    if (!isMovesLeft(board)) return 0;
    if (isMax) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < 9; i++) {
            if (board[i] == EMPTY) {
                board[i] = AI;
                best = max(best, minimax(board, depth+1, false));
                board[i] = EMPTY;
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < 9; i++) {
            if (board[i] == EMPTY) {
                board[i] = HUMAN;
                best = min(best, minimax(board, depth+1, true));
                board[i] = EMPTY;
            }
        }
        return best;
    }
}

int findBestMove(vector<char>& board) {
    int bestVal = numeric_limits<int>::min();
    int bestMove = -1;
    for (int i = 0; i < 9; i++) {
        if (board[i] == EMPTY) {
            board[i] = AI;
            int moveVal = minimax(board, 0, false);
            board[i] = EMPTY;
            if (moveVal > bestVal) {
                bestMove = i;
                bestVal = moveVal;
            }
        }
    }
    return bestMove;
}

int main() {
    vector<char> board(9, EMPTY);
    cout << "Tic-Tac-Toe AI (You: O, AI: X)\n";
    printBoard(board);
    while (true) {

        int aiMove = findBestMove(board);
        board[aiMove] = AI;
        cout << "AI moves to " << (aiMove+1) << "\n";
        printBoard(board);
        if (evaluate(board) == 10) {
            cout << "AI wins!\n";
            break;
        }
        if (!isMovesLeft(board)) {
            cout << "Draw!\n";
            break;
        }
                int move;
        cout << "Enter your move (1-9): ";
        cin >> move;
        move--;
        if (move < 0 || move >= 9 || board[move] != EMPTY) {
            cout << "Invalid move. Try again.\n";
            continue;
        }
        board[move] = HUMAN;
        printBoard(board);
        if (evaluate(board) == -10) {
            cout << "You win!\n";
            break;
        }
        if (!isMovesLeft(board)) {
            cout << "Draw!\n";
            break;
        }
    }
    return 0;
}
