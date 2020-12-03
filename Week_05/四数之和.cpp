#include <vector>
using namespace std;

class Solution
{
private:
    // 寻找所有满足nums[i] + nums[b] + nums[c] + nums[a] = target 的组合，a为已知条件
    void twoSum(vector<int> &nums, int target, int i, int a, vector<vector<int>> &ans)
    {

        int c = nums.size() - 1;
        int lastBNum = (1 << 31);
        for (int b = a + 1; b < nums.size(); ++b)
        {
            if (nums[b] == lastBNum)
            { // 过滤重复数
                continue;
            }
            lastBNum = nums[b];
            while (b < c && nums[b] + nums[c] > target)
            { // 从后往前找到满足b+c<=target为止
                --c;
            }

            if (b == c)
            { //若已经两个指针碰到一起，则肯定不会再满足a+b+c=0的情况
                break;
            }
            if (nums[b] + nums[c] == target)
            {
                ans.push_back({nums[i], nums[a], nums[b], nums[c]});
            }
        }
    }

    vector<vector<int>> threeSum(vector<int> &nums, int target, int i, vector<vector<int>> &result)
    {
        int lastNum = (1 << 31);
        for (int a = i + 1; a < nums.size() - 2; ++a)
        {
            if (nums[a] == lastNum)
            { // 过滤重复数
                continue;
            }
            twoSum(nums, target - nums[a], i, a, result);
            lastNum = nums[a];
        }
        return result;
    }

public:
    vector<vector<int>> fourSum(vector<int> &nums, int target)
    {
        if (nums.size() < 4)
        {
            return {};
        }
        vector<vector<int>> result;
        sort(nums.begin(), nums.end());
        int lastA = (1 << 31);
        for (int i = 0; i < nums.size() - 3; ++i)
        {
            if (nums[i] == lastA)
            {
                continue;
            }
            lastA = nums[i];
            threeSum(nums, target - nums[i], i, result);
        }
        return result;
    }
};