#include <iostream>
#include <array>
#include <optional>
#include <limits>

constexpr int SIZE = 3;
using Board = std::array<std::array<char, SIZE>, SIZE>;

void print_board(const Board& board) {
    std::cout << "\n";
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << (board[i][j] ? board[i][j] : ' ');
            if (j < SIZE - 1) std::cout << " | ";
        }
        std::cout << "\n";
        if (i < SIZE - 1) std::cout << "--+---+--\n";
    }
    std::cout << "\n";
}

std::optional<char> check_winner(const Board& board) {
    for (int i = 0; i < SIZE; ++i) {
        if (board[i][0] && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    if (board[0][0] && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];
    return {};
}

bool is_full(const Board& board) {
    for (auto& row : board)
        for (char cell : row)
            if (!cell) return false;
    return true;
}

int evaluate(const Board& board) {
    if (auto winner = check_winner(board)) {
        if (*winner == 'X') return +10;
        if (*winner == 'O') return -10;
    }
    return 0;
}

int minimax(Board& board, int depth, bool isMax) {
    int score = evaluate(board);
    if (score == 10 || score == -10) return score;
    if (is_full(board)) return 0;

    if (isMax) {
        int best = std::numeric_limits<int>::min();
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (!board[i][j]) {
                    board[i][j] = 'X';
                    best = std::max(best, minimax(board, depth + 1, false));
                    board[i][j] = 0;
                }
            }
        }
        return best;
    } else {
        int best = std::numeric_limits<int>::max();
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (!board[i][j]) {
                    board[i][j] = 'O';
                    best = std::min(best, minimax(board, depth + 1, true));
                    board[i][j] = 0;
                }
            }
        }
        return best;
    }
}

void find_best_move(Board& board, char player) {
    int bestVal = (player == 'X') ? std::numeric_limits<int>::min()
                                   : std::numeric_limits<int>::max();
    int row = -1, col = -1;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (!board[i][j]) {
                board[i][j] = player;
                int moveVal = minimax(board, 0, player == 'O');
                board[i][j] = 0;

                if ((player == 'X' && moveVal > bestVal) ||
                    (player == 'O' && moveVal < bestVal)) {
                    bestVal = moveVal;
                    row = i;
                    col = j;
                }
            }
        }
    }
    if (row != -1 && col != -1) {
        board[row][col] = player;
    }
}

void human_move(Board& board, char player) {
    while (true) {
        std::cout << "Player " << player << ", enter row and column (0-2): ";
        int row, col;
        if (!(std::cin >> row >> col) || row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col]) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid move. Try again.\n";
            continue;
        }
        board[row][col] = player;
        break;
    }
}

void play_game(int mode) {
    Board board{};
    char player = 'X';

    while (true) {
        print_board(board);

        if ((mode == 0) || (mode == 1 && player == 'O')) {
            std::cout << "Computer " << player << "'s turn...\n";
            find_best_move(board, player);
        } else {
            human_move(board, player);
        }

        if (auto winner = check_winner(board)) {
            print_board(board);
            std::cout << "Player " << *winner << " wins!\n";
            break;
        }

        if (is_full(board)) {
            print_board(board);
            std::cout << "It's a draw!\n";
            break;
        }

        player = (player == 'X') ? 'O' : 'X';
    }
}

int get_mode() {
    while (true) {
        std::cout << "\nSelect mode:\n";
        std::cout << "0 - Computer vs Computer\n";
        std::cout << "1 - Human vs Computer\n";
        std::cout << "2 - Human vs Human\n";
        std::cout << "Choice: ";
        int mode;
        if (std::cin >> mode && mode >= 0 && mode <= 2) return mode;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Try again.\n";
    }
}

bool ask_play_again() {
    std::cout << "\nPlay again? (y/n): ";
    char response;
    while (std::cin >> response) {
        if (response == 'y' || response == 'Y') return true;
        if (response == 'n' || response == 'N') return false;
        std::cout << "Invalid input. Enter 'y' or 'n': ";
    }
    return false;
}

int main() {
    std::cout << "TIC-TAC-TOE\n";
    do {
        int mode = get_mode();
        play_game(mode);
    } while (ask_play_again());
    std::cout << "GAME OVER.\n";
}
