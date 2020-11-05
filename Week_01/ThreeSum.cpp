#include <stdlib.h>
#include <vector>
using namespace std;

class Solution
{
public:
    void twoSum(vector<int> &nums, int a, int target, vector<vector<int>> &ans)
    {
        // c 对应的指针初始指向数组的最右端
        int c = nums.size() - 1;
        // 枚举 b
        for (int b = a + 1; b < nums.size(); ++b)
        {
            // 需要和上一次枚举的数不相同
            if (b > a + 1 && nums[b] == nums[b - 1])
            {
                continue;
            }
            // 需要保证 b 的指针在 c 的指针的左侧
            while (b < c && nums[b] + nums[c] > target)
            {
                --c;
            }
            // 如果指针重合，随着 b 后续的增加
            // 就不会有满足 a+b+c=0 并且 b<c 的 c 了，可以退出循环
            if (b == c)
            {
                break;
            }
            if (nums[b] + nums[c] == target)
            {
                ans.push_back({-target, nums[b], nums[c]});
            }
        }
    }

    vector<vector<int>> threeSum(vector<int> &nums)
    {
        if (nums.size() < 3)
        {
            return {};
        }
        sort(nums.begin(), nums.end());
        vector<vector<int>> ans;
        // 枚举 a
        for (int a = 0; a < nums.size() - 2; ++a)
        {
            // 且第一个数小于等于0
            if (nums[a] > 0)
            {
                continue;
            }
            //需要和上一次枚举的数不相同
            if (a > 0 && nums[a] == nums[a - 1])
            {
                continue;
            }
            twoSum(nums, a, -nums[a], ans);
        }
        return ans;
    }
};
