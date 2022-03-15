#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <algorithm>


typedef std::vector<int> piece;
typedef std::array<piece, 4> rotations;
typedef struct {
    std::array<std::vector<int>, 4> rotations;
    std::vector<int> possible;
} test;

// std::vector<rotations> pieces;
std::vector<test> pieces;
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

void printPiece(std::vector<int> vec) {
    std::cout << vec[0] << " " << vec[1]
              << "\n"
              << vec[3] << " " << vec[2]
              << "\n";
}

void process() {
    for (int i = 0; i < n; i++) {
        auto& piece = pieces[i];
        auto vec = piece.rotations[0];
        std::sort(vec.begin(), vec.end());
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            auto other = pieces[j].rotations[0];
            std::sort(other.begin(), other.end());
            std::vector<int> set;
            std::set_intersection(vec.begin(), vec.end(), other.begin(), other.end(), std::back_inserter(set));
            if (set.size() >= 2) {
                piece.possible.push_back(j);
            }
        }
        // std::cout << piece.possible.size() << "\n";
    }
}

void printBoard() {
    for (int i = 0; i < r; i++) {
        std::stringstream topss, botss;
        for (int j = 0; j < c; j++) {
            auto& idx = board[i][j];
            piece& piece = pieces[idx.first].rotations[idx.second];
            topss << piece[0] << " " << piece[1] << "  ";
            botss << piece[3] << " " << piece[2] << "  ";
        }
        std::string top = topss.str(), bot = botss.str();
        std::cout << top.substr(0, top.size() - 2) << "\n"
                  << bot.substr(0, bot.size() - 2) << "\n";
        if (i != r - 1) std::cout << "\n";
    }
}

bool solve(int row, int col) {
    if (col == c) { // Wrap around
        row++;
        col = 0;
    }

    if (row == r) { // Puzzle board filled
        return true;
    }

    std::vector<int> possible;
    int lup = -1, ldown = -1, uleft = -1, uright = -1;
    if (col > 0) { // Match left
        auto& idx = board[row][col - 1];
        auto& piece = pieces[idx.first].rotations[idx.second];
        lup = piece[1];
        ldown = piece[2];
    }

    if (row > 0) { // Match up
        auto& idx = board[row - 1][col];
        auto& piece = pieces[idx.first].rotations[idx.second];
        uleft = piece[3];
        uright = piece[2];
    }

    if (col > 0) {
        if (row > 0) {
            auto& left = pieces[board[row][col - 1].first].possible;
            auto& up = pieces[board[row - 1][col].first].possible;
            std::set_intersection(left.begin(), left.end(), up.begin(), up.end(), std::back_inserter(possible));
        } else {
            possible = pieces[board[row][col - 1].first].possible;
        }
    } else if (row > 0) {
        possible = pieces[board[row - 1][col].first].possible;
    }

    // TODO Improvement: check if lup, ldown, uleft and uright are on the piece, else skip
    for (int i = 0; i < possible.size(); i++) { // Start from the second piece onwards
        if (used[possible[i]]) continue; // Skip piece if already on the board
        auto& current = pieces[possible[i]];
        for (int j = 0; j < 4; j++) { // Check all rotations
            // piece& rotation = current.rotations[j];
            piece& rotation = current.rotations[j];
            bool matchedLeft = (rotation[0] == lup && rotation[3] == ldown) ? true : false;
            bool matchedUp = (rotation[0] == uleft && rotation[1] == uright) ? true : false;
            bool matched = false;

            if (col > 0) {
                if (row > 0) matched = matchedLeft && matchedUp;
                else matched = matchedLeft;
            } else if (row > 0) matched = matchedUp;

            if (matched) {
                used[possible[i]] = true;
                board[row][col] = {possible[i], j};
                if (solve(row, col+1)) {
                    return true;
                }
                used[possible[i]] = false;
                break; // No point in checking other rotations of the same piece
            }
        }
    }

    col--;
    if (col < 0) {
        if (row > 0) {
            row--;
            col = c - 1;
        } else col = 0;
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
            test t;
            rotations p;
            p[0] = { p1, p2, p3, p4 };
            p[1] = rotate(p[0]);
            p[2] = rotate(p[1]);
            p[3] = rotate(p[2]);
            used[j] = (j == 0) ? true : false; // Mark only the first piece as used
            t.rotations = p;
            pieces.push_back(t);
        }
        process();
        board[0][0] = {0, 0};
        currRow = 0;
        currCol = 1;
        if (solve(0, 1)) {
            printBoard();
        } else {
            std::cout << "impossible puzzle!\n";
        }
    }
    return 0;
}