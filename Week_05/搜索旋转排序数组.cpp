#include <vector>
using namespace std;

class Solution
{
public:
    int search(vector<int> &nums, int target)
    {
        int len = nums.size();
        if (len == 0)
        {
            return -1;
        }
        if (len == 1)
        {
            return (nums[0] == target) ? 0 : -1;
        }
        int left = 0;
        int right = len - 1;
        while (left <= right)
        {
            int mid = (right + left) / 2;
            if (nums[mid] == target)
                return mid;
            if (nums[0] <= nums[mid])
            { // 说明当前点在左侧升序段中
                if (nums[0] <= target && target < nums[mid])
                { // 往左半段的左侧搜索
                    right = mid - 1;
                }
                else
                { // 往左半段的右侧搜索
                    left = mid + 1;
                }
            }
            else
            { // 在右侧可能的旋转段中
                if (nums[mid] < target && target <= nums[len - 1])
                { // 在右侧的后半段搜索
                    left = mid + 1;
                }
                else
                { // 在右侧的左半段搜索
                    right = mid - 1;
                }
            }
        }
        return -1;
    }
};