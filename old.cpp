#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <sstream>

typedef std::vector<int> piece;
// typedef std::array<piece, 4> rotations;
typedef piece rotations[4];
typedef std::pair<int, int> index;

// std::array<std::array<std::pair<int, int>, 50>, 50> board;
index board[50][50];

rotations pieces[2500];
bool used[2500];
int n, r, c;
int currRow, currCol;

std::vector<int> rotate(std::vector<int> vec) {
    vec.push_back(vec[0]);
    vec.erase(vec.begin());
    return vec;
}

bool solve() {
    std::cout << currRow << " " << currCol << "\n";
    if (currRow <= 0 && currCol == 0) {
        // Backtracked too far, puzzle is impossible
        std::cout << "END\n";
        return false;
    }

    if (currCol == c) {
        currRow++;
        currCol = 0;
    }

    if (currRow == r) {
        // Solution was found
        return true;
    }

    // index idx;
    int a = -1, b = -1, c1 = -1, d = -1;
    if (currCol != 0) { // Match left
        index &idx = board[currRow][currCol - 1];
        piece &leftPiece = pieces[idx.first][idx.second];
        a = leftPiece[1];
        b = leftPiece[2];
    }

    if (currRow != 0) { // Match up
        index &idx = board[currRow - 1][currCol];
        piece &topPiece = pieces[idx.first][idx.second];
        c1 = topPiece[2];
        d = topPiece[3];
    }

    for (int i = 0; i < n; i++) {
        if (used[i])
            continue;
        for (int j = 0; j < 4; j++) {
            auto &current = pieces[i][j];
            bool isMatch = false;
            if (currCol != 0) { // Match to the left
                if (a == current[0] && b == current[3])
                    isMatch = true;
            }
            if (currRow != 0) { // Match up
                if (d == current[0] && c1 == current[1])
                    isMatch = true;
            }
            if (isMatch) {
                used[i] = true;
                board[currRow][currCol] = {i, j};
                // history[currRow][currCol].push_back(piece.first);
                currCol++;
                if (solve()) return true;
                currCol--;
                board[currRow][currCol] = {-1, -1};
                used[i] = false;
            }
        }
    }

    // Backtracking point
    currCol--;
    if (currCol < 0) {
        if (currRow > 0) currRow--;
        currCol = c - 1;
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
        // Get number of pieces and board size from cin
        std::cin >> n >> r >> c;
        for (int j = 0; j < n; j++) {
            rotations p;
            // Get pieces from cin
            std::cin >> p1 >> p2 >> p3 >> p4;

            pieces[j][0] = { p1, p2, p3, p4 };
            if (j > 0) {
                pieces[j][1] = rotate(pieces[j][0]);
                pieces[j][2] = rotate(pieces[j][1]);
                pieces[j][3] = rotate(pieces[j][2]);
            }
            used[j] = false;
        }

        used[0] = true;
        board[0][0] = {0,0};
        currRow = 0; currCol = 1;
        if (solve()) {
            printBoard();
        } else {
            std::cout << "impossible puzzle!\n";
        }
    }

    return 0;
}