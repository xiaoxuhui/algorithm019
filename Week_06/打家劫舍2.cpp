#include <vector>
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