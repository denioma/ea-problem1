#include <iostream>
#include <sstream>
#include <array>
#include <vector>

typedef std::vector<int> piece;
typedef std::array<piece, 4> rotations;

std::vector<rotations> pieces;
std::array<bool, 2500> used;
std::array<std::array<std::pair<int, int>, 50>, 50> board;

int n, r, c;
int currRow, currCol;

inline
std::vector<int> rotate(std::vector<int> vec) {
  vec.push_back(*vec.begin());
  vec.erase(vec.begin());
  return vec;
}

void printBoard() {
    for (int i = 0; i < r; i++) {
        std::stringstream topss, botss;
        for (int j = 0; j < c; j++) {
            auto& idx = board[i][j];
            piece& piece = pieces[idx.first][idx.second];
            topss << piece[0] << " " << piece[1] << "  ";
            botss << piece[3] << " " << piece[2] << "  ";
        }
        std::string top = topss.str(), bot = botss.str();
        std::cout << top.substr(0, top.size() - 2) << "\n"
                  << bot.substr(0, bot.size() - 2) << "\n";
        if (i != r - 1) std::cout << "\n";
    }
}

bool solve() {
    if (currCol == c) { // Wrap around
        currRow++;
        currCol = 0;
    }

    if (currRow == r) { // Puzzle board filled
        return true;
    }

    int lup = -1, ldown = -1, uleft = -1, uright = -1;
    if (currCol > 0) { // Match left
        auto& idx = board[currRow][currCol - 1];
        auto& piece = pieces[idx.first][idx.second];
        lup = piece[1];
        ldown = piece[2];
    }

    if (currRow > 0) { // Match up
        auto& idx = board[currRow - 1][currCol];
        auto& piece = pieces[idx.first][idx.second];
        uleft = piece[3];
        uright = piece[2];
    }

    // TODO Improvement: check if lup, ldown, uleft and uright are on the piece, else skip
    for (int i = 1; i < n; i++) { // Start from the second piece onwards
        if (used[i]) continue; // Skip piece if already on the board
        for (int j = 0; j < 4; j++) { // Check all rotations
            piece& current = pieces[i][j];
            bool matchedLeft = (current[0] == lup && current[3] == ldown) ? true : false;
            bool matchedUp = (current[0] == uleft && current[1] == uright) ? true : false;
            bool matched = false;

            if (currCol > 0) {
                if (currRow > 0) matched = matchedLeft && matchedUp;
                else matched = matchedLeft;
            } else if (currRow > 0) matched = matchedUp;

            if (matched) {
                used[i] = true;
                board[currRow][currCol] = {i, j};
                currCol++;
                if (solve()) {
                    return true;
                }
                used[i] = false;
                break; // No point in checking other rotations of the same piece
            }
        }
    }

    currCol--;
    if (currCol < 0) {
        if (currRow > 0) currRow--;
        currCol = c - 1;
    }
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int testcases;
    std::cin >> testcases;
    for (int i = 0; i < testcases; i++) {
        pieces.clear();
        std::cin >> n >> r >> c;
        for (int j = 0; j < n; j++) {
            int p1, p2, p3, p4;
            std::cin >> p1 >> p2 >> p3 >> p4;
            rotations p;
            p[0] = { p1, p2, p3, p4 };
            p[1] = rotate(p[0]);
            p[2] = rotate(p[1]);
            p[3] = rotate(p[2]);
            used[j] = (j == 0) ? true : false; // Mark only the first piece as used
            pieces.push_back(p);
        }
        board[0][0] = {0, 0};
        currRow = 0;
        currCol = 1;
        if (solve()) {
            printBoard();
        } else {
            std::cout << "impossible puzzle!\n";
        }
    }
    return 0;
}