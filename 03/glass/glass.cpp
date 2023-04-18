#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>

std::string readLine() {
    std::string str;
    std::getline(std::cin, str);
    if (str.back() == '\n') {
        str.pop_back();
    }
    str += str;
    return str;
}

void solve() {
    std::string line = readLine();
    const size_t len = line.size() / 2;

    size_t bestFit = 0;
    size_t oponent = bestFit + 1;

    while(bestFit < len) {
        // std::cout << "Start at " <<  bestFit << " - " <<  oponent << std::endl;
        size_t offset  = 0;
        while(line[bestFit + offset] == line[oponent + offset]) {
            offset++;
            // std::cout << "Incing to " << offset << std::endl;
            if (offset == len) {
                goto endit;
            }
        }

        if (line[bestFit + offset] > line[oponent + offset]) {
            // std::cout << "New found" << std::endl;
            bestFit = oponent;
            oponent = bestFit + 1;
        } else {
            // std::cout << "Still better" << std::endl;
            ++oponent;
            if (oponent - bestFit == len) {
                goto endit;
            }
        }
    }

endit:
    printf("%lu\n", bestFit + 1);
}

int main(void) {
    size_t cases;
    scanf("%lu\n", &cases);
    for (size_t i = 0; i < cases; ++i) {
        solve();
    }
    return 0;
}

