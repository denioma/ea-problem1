#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <sstream>

// typedef std::pair<int, int> index;
typedef std::array<int, 2> index;
typedef struct _rotation {
    std::vector<int> rotation;
    std::vector<index> right;
    std::vector<index> down;
} rotation;

typedef std::array<rotation, 4> piece;

std::array<int, 999> colors {};
std::vector<piece> pieces;
std::array<bool, 2500> used;
std::array<std::array<index, 50>, 50> board;

int n, r, c;

inline
std::vector<int> rotate(std::vector<int> vec) {
  vec.push_back(*vec.begin());
  vec.erase(vec.begin());
  return vec;
}

bool process() {
    int odds = 0;
    // Check that there are at most 4 colors with an odd number of pieces - four corners of the board
    for (const auto& count : colors) {
        if (count % 2 != 0) odds++;
        if (odds > 4) return false;
    }

    // For every piece in the piece set
    for (int i = 0; i < n; i++) {
        std::vector<int> possibleSet;
        auto& piece = pieces[i];
        // Copy piece and sort
        auto vec = piece[0].rotation;
        // Sorting is necessary for set_intersection to work
        std::sort(vec.begin(), vec.end());
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            // Copy comparison piece and sort
            auto other = pieces[j][0].rotation;
            // Find common numbers
            std::sort(other.begin(), other.end());
            std::vector<int> set;
            // Find common numbers
            std::set_intersection(vec.begin(), vec.end(), other.begin(), other.end(), std::back_inserter(set));
            if (set.size() >= 2) {
                possibleSet.push_back(j);
            }
        }

        // If a piece has no possible matches, the puzzle is impossible
        if (possibleSet.size() == 0) return false;

        // From possible pieces set, find those that can be placed on the right and down
        for (const auto& idx : possibleSet) {
            auto& possiblePiece = pieces[idx];
            for (int m = 0; m < 4; m++) {
                auto& self = piece[m];
                for (int k = 0; k < 4; k++) {
                    auto& other = possiblePiece[k].rotation;

                    if (other[0] == self.rotation[1] && other[3] == self.rotation[2]) { // Match left
                        self.right.push_back({idx, k});
                    }
                    if (other[0] == self.rotation[3] && other[1] == self.rotation[2]) { // Match up
                        self.down.push_back({idx, k});
                    }
                }
            }
        }
    }
    return true;
}

void printBoard() {
    for (int i = 0; i < r; i++) {
        std::stringstream topss, botss;
        for (int j = 0; j < c; j++) {
            auto& idx = board[i][j];
            auto& rotation = pieces[idx[0]][idx[1]].rotation;
            topss << rotation[0] << " " << rotation[1] << "  ";
            botss << rotation[3] << " " << rotation[2] << "  ";
        }
        std::string top = topss.str(), bot = botss.str();
        std::cout << top.substr(0, top.size() - 2) << "\n"
                  << bot.substr(0, bot.size() - 2) << "\n";
        if (i != r - 1) std::cout << "\n";
    }
}

// Backtracking approach
bool solve(int row, int col) {
    // Scan the board from left to right, and downwards
    if (col == c) { // Wrap around
        row++;
        col = 0;
    }

    if (row == r) { // Puzzle board filled
        return true;
    }

    std::vector<index> tmp;
    std::vector<index> *possible = nullptr;

    if (col > 0) {
        if (row > 0) { // Need to match the new piece with the one above and to the left
            auto& lidx = board[row][col - 1];
            auto& uidx = board[row - 1][col];
            auto& left = pieces[lidx[0]][lidx[1]].right;
            auto& up = pieces[uidx[0]][uidx[1]].down;
            std::set_intersection(left.begin(), left.end(), up.begin(), up.end(), std::back_inserter(tmp));
            possible = &tmp;
        } else { // Need to match the new piece with the one to the left
            auto& idx = board[row][col - 1];
            possible = &pieces[idx[0]][idx[1]].right;
        }
    } else if (row > 0) { // Need to match the new piece with the one above
        auto& idx = board[row - 1][col];
        possible = &pieces[idx[0]][idx[1]].down;
    }

    // For each possible new piece
    if (possible) {
        for (const auto& idx : *possible) {
            if (used[idx[0]]) continue; // Skip piece if already on the board
            used[idx[0]] = true; // Mark piece as used
            board[row][col] = idx;
            if (solve(row, col+1)) {
                return true;
            }
            used[idx[0]] = false; // Unmark piece as used
        }
    }

    // No valid partial solution found, backtrack
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
        for (auto& count : colors) count = 0;
        for (int j = 0; j < n; j++) {
            int p1, p2, p3, p4;
            piece p;
            std::cin >> p1 >> p2 >> p3 >> p4;
            // Count colors
            colors[p1-1]++;
            colors[p2-1]++;
            colors[p3-1]++;
            colors[p4-1]++;
            // Store rotations
            p[0].rotation = { p1, p2, p3, p4 };
            p[1].rotation = rotate(p[0].rotation);
            p[2].rotation = rotate(p[1].rotation);
            p[3].rotation = rotate(p[2].rotation);
            used[j] = (j == 0) ? true : false; // Mark only the first piece as used
            pieces.push_back(p);
        }
        if (!process()) { // Preprocessing
            std::cout << "impossible puzzle!\n";
        } else {
            board[0][0] = {0, 0};
            if (solve(0, 1)) { // Solve the board
                printBoard();
            } else {
                std::cout << "impossible puzzle!\n";
            }
        }
    }
    return 0;
}