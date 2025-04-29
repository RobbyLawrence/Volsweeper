#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

// Apply one operation; if division by zero is attempted, mark valid = false.
double apply_op(double lhs, double rhs, char op, bool &valid) {
    switch (op) {
        case '+': return lhs + rhs;
        case '-': return lhs - rhs;
        case '*': return lhs * rhs;
        case '/':
            if (rhs == 0) {
                valid = false;
                return 0;
            }
            return lhs / rhs;
    }
    valid = false;
    return 0;
}

int main() {
    int a, b, c, d;
    std::cout << "Enter four positive integers (a b c d): ";
    if (!(std::cin >> a >> b >> c >> d)) {
        std::cerr << "Invalid input.\n";
        return 1;
    }

    std::vector<int> nums = {a, b, c, d};
    std::sort(nums.begin(), nums.end());

    std::vector<char> ops = {'+', '-', '*', '/'};

    // Map from result → all distinct expressions that yield it
    std::map<double, std::vector<std::string>> results;

    do {
        // for each operator triple
        for (char op1 : ops) for (char op2 : ops) for (char op3 : ops) {
            // 1) Plain left‐to‐right: ((n0 op1 n1) op2 n2) op3 n3
            {
                bool ok = true;
                double r1 = apply_op(nums[0], nums[1], op1, ok);
                if (ok) {
                    double r2 = apply_op(r1, nums[2], op2, ok);
                    if (ok) {
                        double r3 = apply_op(r2, nums[3], op3, ok);
                        if (ok) {
                            std::ostringstream e;
                            e << nums[0] << ' ' << op1 << ' '
                              << nums[1] << ' ' << op2 << ' '
                              << nums[2] << ' ' << op3 << ' '
                              << nums[3];
                            auto &vec = results[r3];
                            if (std::find(vec.begin(), vec.end(), e.str()) == vec.end())
                                vec.push_back(e.str());
                        }
                    }
                }
            }

            // 2) a op1 (b op2 c) op3 d
            {
                bool ok = true;
                double t1 = apply_op(nums[1], nums[2], op2, ok);
                if (ok) {
                    double t2 = apply_op(nums[0], t1,       op1, ok);
                    if (ok) {
                        double t3 = apply_op(t2,       nums[3], op3, ok);
                        if (ok) {
                            std::ostringstream e;
                            e << nums[0] << ' ' << op1 << " ("
                              << nums[1] << ' ' << op2 << ' ' << nums[2]
                              << ") " << op3 << ' ' << nums[3];
                            auto &vec = results[t3];
                            if (std::find(vec.begin(), vec.end(), e.str()) == vec.end())
                                vec.push_back(e.str());
                        }
                    }
                }
            }

            // 3) a op1 b op2 (c op3 d)
            {
                bool ok = true;
                double t1 = apply_op(nums[2], nums[3], op3, ok);
                if (ok) {
                    double t2 = apply_op(nums[0], nums[1], op1, ok);
                    if (ok) {
                        double t3 = apply_op(t2,       t1,     op2, ok);
                        if (ok) {
                            std::ostringstream e;
                            e << nums[0] << ' ' << op1 << ' '
                              << nums[1] << ' ' << op2 << " ("
                              << nums[2] << ' ' << op3 << ' ' << nums[3]
                              << ")";
                            auto &vec = results[t3];
                            if (std::find(vec.begin(), vec.end(), e.str()) == vec.end())
                                vec.push_back(e.str());
                        }
                    }
                }
            }
        }
    } while (std::next_permutation(nums.begin(), nums.end()));

    // Print them out
    std::cout << "\nAll distinct results:\n"
              << std::fixed << std::setprecision(4);
    for (auto &kv : results) {
        std::cout << kv.first << ":\n";
        for (auto &expr : kv.second)
            std::cout << "  " << expr << "\n";
    }
    for (auto num : results) {
        if (num.first == 10) {
            std::cout << "Solution:";
            for (auto k : num.second) {
                std::cout << k << '\n';
            }
        }
    }
    return 0;
}
