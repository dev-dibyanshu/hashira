#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

long long decode_value(const string& val, int base) {
    long long result = 0;
    for (char c : val) {
        int digit = 0;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = tolower(c) - 'a' + 10;
        result = result * base + digit;
    }
    return result;
}


long long lagrange_interpolation(const vector<pair<long long, long long>>& points) {
    long long result = 0;
    int k = points.size();

    for (int i = 0; i < k; ++i) {
        long long xi = points[i].first;
        long long yi = points[i].second;
        long long num = 1, den = 1;

        for (int j = 0; j < k; ++j) {
            if (i != j) {
                num *= -points[j].first;
                den *= (xi - points[j].first);
            }
        }

        result += (yi * num) / den;
    }

    return result;
}

long long process_test_case(const string& filename) {
    ifstream input(filename);
    json j;
    input >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<pair<long long, long long>> points;

    for (auto& [key, val] : j.items()) {
        if (key == "keys") continue;

        int x = stoi(key);
        int base = stoi(val["base"].get<string>());
        string value_str = val["value"].get<string>();
        long long y = decode_value(value_str, base);

        points.emplace_back(x, y);
    }

    sort(points.begin(), points.end());
    points.resize(k); // take first k points only

    return lagrange_interpolation(points);
}

int main() {
    string file1 = "testcase1.json";
    string file2 = "testcase2.json";

    long long secret1 = process_test_case(file1);
    long long secret2 = process_test_case(file2);

    cout << "Secret for Testcase 1: " << secret1 << endl;
    cout << "Secret for Testcase 2: " << secret2 << endl;

    return 0;
}
