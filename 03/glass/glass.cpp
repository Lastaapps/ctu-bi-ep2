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

    std::set<uint32_t> valid;
    for (uint32_t i = 0; i < len; ++i) {
        valid.insert(i);
    }

    for (const char * ptr = lineRaw; ptr < lineEnd; ++ptr) {
        // printf("new round\n");

        auto worst = valid.begin();
        for (auto itr = valid.begin(); itr != valid.end();) {

            const char c = *(ptr + *itr);
            const char w = *(ptr + *worst);
            // printf("check: %2d %c\n", *itr,   c);
            // printf("worst: %2d %c\n", *worst, w);

            if (c < w) {
                valid.erase(valid.begin(), worst);
                valid.erase(worst);
                worst = itr++;
            } else if (c > w) {
                auto oldItr = itr++;
                valid.erase(oldItr);
            } else if (c == w) {
                worst = itr++;
            }
        }
        // printf("%c wins\n[", *(ptr + *worst));
        // for (uint32_t item : valid) {
        //     printf("%2d, ", item);
        // }
        // printf("]\n");
    }

    printf("%u\n", *valid.begin() + 1);
}

int main(void) {
    size_t cases;
    scanf("%lu\n", &cases);
    for (size_t i = 0; i < cases; ++i) {
        solve();
    }
    return 0;
}

