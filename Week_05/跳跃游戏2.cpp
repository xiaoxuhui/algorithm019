#include <vector>
using namespace std;

class Solution {
public:
    int jump(vector<int>& nums) {
        int len = nums.size();
        int maxPos = 0;
        int end = 0;
        int stepCount = 0;
        for (int i = 0;i < len - 1; ++i) {
            maxPos = max(maxPos,i + nums[i]);
            if(end == i){// 到达最远位置步数加1
                end = maxPos;
                stepCount++;
            }
        }
        return stepCount;
    }
};