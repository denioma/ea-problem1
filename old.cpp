#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <sstream>
#include <algorithm>

typedef std::vector<int> piece;
typedef std::array<piece, 4> rotations;
typedef std::pair<int, int> index;

std::map<index, std::vector<index>> blacklist;

std::array<std::array<std::pair<int, int>, 50>, 50> board;
std::array<std::array<std::vector<int>, 50>, 50> history;
std::map<int, rotations> pieces;
std::vector<bool> used = {false};
int n, r, c;
int currRow, currCol;

std::vector<int> rotate(std::vector<int> vec) {
    vec.push_back(vec[0]);
    vec.erase(vec.begin());
    return vec;
}

bool backtrack(int& x, int&y, int id) {
    auto& vec = history[x][y];
    auto it = std::find(vec.begin(), vec.end(), id);
    if (it != vec.end()) {
        used[*it] = false;
        return true;
    }
    return false;
}

bool match() {
    index idx;
    piece leftPiece, topPiece;
    int a = -1, b = -1, c = -1, d = -1;
    if (currCol != 0) {
        idx = board[currRow][currCol - 1];
        leftPiece = pieces[idx.first][idx.second];
        a = leftPiece[1];
        b = leftPiece[2];
    }
    if (currRow != 0) {
        idx = board[currRow - 1][currCol];
        topPiece = pieces[idx.first][idx.second];
        c = topPiece[2];
        d = topPiece[3];
    }

    for (auto& piece : pieces) {
        if (used[piece.first] || backtrack(currRow, currCol, piece.first))
            continue;
        for (int i = 0; i < 4; i++) {
            auto current = piece.second[i];
            bool isMatch = false;
            if (currCol != 0) { // Match to the left
                if (a == current[0] && b == current[3])
                    isMatch = true;
            }
            if (currRow != 0) { // Match up
                if (d == current[0] && c == current[1])
                    isMatch = true;
            }
            if (isMatch) {
                used[piece.first] = true;
                board[currRow][currCol] = {piece.first, i};
                history[currRow][currCol].push_back(piece.first);
                return true;
            }
        }
    }
}

bool solve() {
    if (currRow == r) return true;
    else if (currCol == c) {
        currRow++;
        currCol = 0;
    } else {
        if (!match()) {
            return false;
        }
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
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    // Get number of testcases from cin
    int nTestcases;
    std::cin >> nTestcases;

    int p1, p2, p3, p4;
    for (int i = 0; i < nTestcases; i++) {
        pieces.clear();
        // Get number of pieces and board size from cin
        std::cin >> n >> r >> c;
        for (int j = 0; j < n; j++) {
            rotations p;
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
        if (solve()) {
            printBoard();
        } else {
            std::cout << "Impossible\n";
        }
    }

    return 0;
}