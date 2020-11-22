#include <vector>
using namespace std;
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        if(nums.size() == 0) {
            return -1;
        }
        // 计数法，复用多数元素-其它元素个数>0的原理
        int element = nums[0];
        int count = 1;
        for (int i = 1; i < nums.size(); ++i) {
            if(nums[i] == element) {
                ++count;
                continue;
            }
            --count;
            if(count == 0) {//减为0，则
                element = nums[i];
                count = 1;
            }
        }
        return (count > 0)?element:-1;
    }
};

