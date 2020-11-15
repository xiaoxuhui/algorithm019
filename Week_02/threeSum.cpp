#include <vector>
using namespace std;

class Solution {
public:
    // 寻找所有满足nums[b] + nums[c] + nums[a] = 0 的组合，a为已知条件 
    void twoSum(vector<int>& nums,int a, vector<vector<int>> &ans) {
        int target = -nums[a];
        int c = nums.size() - 1;
        int lastBNum = (1 << 31);
        for (int b = a + 1; b <nums.size(); ++b) {
            if(nums[b] == lastBNum) {// 过滤重复数
                continue;
            }
            lastBNum = nums[b];
            while (b < c && nums[b] + nums[c] > target) {// 从后往前找到满足b+c<=target为止
                --c;
            }

            if(b == c) {//若已经两个指针碰到一起，则肯定不会再满足a+b+c=0的情况
                break;
            }
            if(nums[b] + nums[c] == target) {
                ans.push_back({-target,nums[b],nums[c]});
            }
        }
    }

    vector<vector<int>> threeSum(vector<int>& nums) {
        if (nums.size() < 3) {
            return {};
        }
        sort(nums.begin(), nums.end());//先从小到大排序 

        vector<vector<int>> result;
        int lastNum = (1<<31);
        for (int a = 0;a <nums.size()- 2;++a) {
            if(nums[a] > 0){// 大于0说明不再可能出现 a + b + c = 0
                break;
            }
            if(nums[a] == lastNum) {// 过滤重复数
                continue;
            }
            twoSum(nums, a, result);
            lastNum = nums[a];
        }
        return result;
    }
};
