#include <iostream>
#include <vector>

void rotate(std::vector<int>& vec) {
    vec.push_back(vec[0]);
    vec.erase(vec.begin());
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    // Get number of testcases from cin
    int nTestcases;
    std::cin >> nTestcases;

    std::vector<std::vector<int>> pieces;

    int n, r, c;
    int p1, p2, p3, p4;
    for (int i = 0; i < nTestcases; i++) {
        pieces.clear();
        // Get number of pieces and board size from cin
        std::cin >> n >> r >> c;
        for (int j = 0; j < n; j++) {
            // Get pieces from cin
            std::cin >> p1 >> p2 >> p3 >> p4;
            pieces.push_back({ p1, p2, p3, p4 });
        }
    }

    return 0;
}