#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;

class Solution
{
public:
    int rob_1(vector<int> &nums, int begin, int end)
    {
        int n = end + 1 - begin;
        if (n <= 0)
        {
            return 0;
        }
        if (n == 1)
        {
            return nums[begin];
        }

        vector<int> dp(n, 0);
        dp[0] = nums[begin];
        dp[1] = max(nums[begin], nums[begin + 1]);
        for (int i = 2; i < n; ++i)
        {
            dp[i] = max(dp[i - 2] + nums[i + begin], dp[i - 1]);
        }

        return dp[n - 1];
    }

    int rob(vector<int> &nums)
    {
        int n = nums.size();
        if (n == 0)
        {
            return 0;
        }
        if (n == 1)
        {
            return nums[0];
        }
        return max(rob_1(nums, 0, n - 2), rob_1(nums, 1, n - 1));
    }
};


void trimLeftTrailingSpaces(string &input) {
    input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
        return !isspace(ch);
    }));
}

void trimRightTrailingSpaces(string &input) {
    input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
        return !isspace(ch);
    }).base(), input.end());
}

vector<int> stringToIntegerVector(string input) {
    vector<int> output;
    trimLeftTrailingSpaces(input);
    trimRightTrailingSpaces(input);
    input = input.substr(1, input.length() - 2);
    stringstream ss;
    ss.str(input);
    string item;
    char delim = ',';
    while (getline(ss, item, delim)) {
        output.push_back(stoi(item));
    }
    return output;
}

int main() {
    string line;
    while (getline(cin, line)) {
        vector<int> nums = stringToIntegerVector(line);
        
        int ret = Solution().rob(nums);

        string out = to_string(ret);
        cout << out << endl;
    }
    return 0;
}
