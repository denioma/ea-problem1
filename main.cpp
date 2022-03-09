#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <sstream>

typedef std::array<std::vector<int>, 4> piece;

std::array<std::array<std::pair<int, int>, 50>, 50> board;
std::map<int, piece> pieces;
std::vector<bool> used = {false};
int n, r, c;
int currRow, currCol;

std::vector<int> rotate(std::vector<int> vec) {
    vec.push_back(vec[0]);
    vec.erase(vec.begin());
    return vec;
}

bool match() {
    if (currRow == 0) {
        // Only match to the left
        auto index = board[0][currCol-1];
        auto leftPiece = pieces[index.first][index.second];
        int a = leftPiece[1], b = leftPiece[2];
        for (auto& piece : pieces) {
            if (used[piece.first]) continue;
            for (int i = 0; i < 4; i++) {
                int c = piece.second[i][0], d = piece.second[i][3];
                if (a == c && b == d) { // Found match
                    used[piece.first] = true;
                    board[currRow][currCol] = {piece.first, i};
                    return true;
                }
            }
        }
    }
    return false;
}

bool solve() {
    if (r != 1) return false;
    if (currRow == r) return true;
    else if (currCol == c) {
        currRow++;
        currCol = 0;
    } else {
        if (!match()) return false;
        currCol++;
    }
    return solve();
}

void printBoard() {
    for (int row = 0; row < r; row++) {
        if (row != 0) std::cout << "\n";
        std::stringstream topStream, botStream;
        for (int col = 0; col < c; col++) {
            auto piece = board[row][col];
            auto rotation = pieces[piece.first][piece.second];
            topStream << rotation[0] << " " << rotation[1] << "  ";
            botStream << rotation[3] << " " << rotation[2] << "  ";
        }
        std::string top = topStream.str(), bot = botStream.str();
        top.erase(top.end() - 2, top.end());
        bot.erase(bot.end() - 2, bot.end());
        std::cout << top << "\n" << bot << "\n";
    }
}

int main() {
    // std::ios_base::sync_with_stdio(0);
    // std::cin.tie(0);

    // Get number of testcases from cin
    int nTestcases;
    std::cin >> nTestcases;

    int p1, p2, p3, p4;
    for (int i = 0; i < nTestcases; i++) {
        pieces.clear();
        // Get number of pieces and board size from cin
        std::cin >> n >> r >> c;
        for (int j = 0; j < n; j++) {
            piece p;
            // Get pieces from cin
            std::cin >> p1 >> p2 >> p3 >> p4;

            p[0] = { p1, p2, p3, p4 };
            if (j > 0) {
                p[1] = rotate(p[0]);
                p[2] = rotate(p[1]);
                p[3] = rotate(p[2]);
            }
            pieces.insert({j, p});
        }

        used[0] = true;
        board[0][0] = {0,0};
        currRow = 0; currCol = 1;
        if (r > 1) {
            std::cout << "r > 1: Not yet implemented!\n";
        } else if (solve()) {
            printBoard();
        } else {
            std::cout << "Impossible\n";
        }
    }

    return 0;
}