#include <cstdlib>
#include <iostream>
#include <cstring>
#include <array>
#include <string>
#include <cstdio>
#include <memory>
#include <set>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

std::string readLine() {
    // char* array;
    // size_t len;
    // getline(&array, &len, stdin);

    // std::array<char, MAX_CASE * 2> arr;
    // std::copy(array, array + len, arr.begin());
    // std::copy(array, array + len, arr.begin() + len);

    // free(array);

    std::string str;
    std::getline(std::cin, str);
    if (str.back() == '\n') {
        str.pop_back();
    }
    str += str;
    // std::cout << str << std::endl;
    return str;
}

void solve() {
    std::string line = readLine();
    const size_t len = line.size() / 2;
    const char* lineRaw = line.c_str();
    const char* lineEnd = line.c_str() + len;

    std::vector<uint32_t> valid;
    std::vector<uint32_t> nextValid;
    for (uint32_t i = 0; i < len; ++i) {
        valid.emplace_back(i);
    }

    for (const char * ptr = lineRaw; ptr < lineEnd && valid.size() > 1; ++ptr) {
        // printf("new round\n");

        // the best so far
        char best = 127;
        for (auto itr = valid.begin(); itr != valid.end(); ++itr) {

            const char c = *(ptr + *itr);
            // printf("check: %2d %c\n", *itr, c);
            // printf("best:     %c\n", best);

            if (c < best) {
                best = c;
                nextValid.clear();
            } else if (c > best) {
                continue;
            } else if (c == best) {
            }
            nextValid.emplace_back(*itr);
        }

        // nextValid will be cleared when it hits the next conflict
        std::swap(valid, nextValid);
        // printf("["); for(auto item : valid) { printf("%u, ", item); } printf("]\n");

        const size_t dist = ptr - lineRaw;
        const size_t modifiedFirst = valid[0] + (lineEnd - lineRaw);
        valid.push_back(modifiedFirst);

        size_t atMostEaten = 0;
        for (size_t i = 0; i < valid.size() - 1; ++i) {
            size_t eaten = 0;

            while (i < valid.size() - 1 && valid[i] + dist == valid[i + 1]) {
                // printf("Is valid for index %lu and dist %lu\n", i, dist);
                ++eaten;
                valid.erase(valid.begin() + i + 1);
            }
            if (eaten < atMostEaten) {
                valid.erase(valid.begin() + i);
                --i;
            } else {
                atMostEaten = eaten;
            }
        }
        ptr += atMostEaten * dist;

        if (*valid.rbegin() == modifiedFirst) {
            valid.pop_back();
        }


        // printf("["); for(auto item : valid) { printf("%u, ", item); } printf("]\n");

        // printf("%c wins\n[", *(ptr + *worst));
        // for (uint32_t item : valid) {
        //     printf("%2d, ", item);
        // }
        // printf("]\n");
    }

    printf("%u\n", valid[0] + 1);
}

int main(void) {
    size_t cases;
    scanf("%lu\n", &cases);
    for (size_t i = 0; i < cases; ++i) {
        solve();
    }
    return 0;
}

