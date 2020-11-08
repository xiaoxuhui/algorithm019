#include <vector>
using namespace std;
class Solution
{
public:
    int trap(vector<int> &height)
    {
        int left = 0, right = height.size() - 1;
        int ans = 0;
        int left_max = 0, right_max = 0;
        while (left < right)
        {
            if (height[left] < height[right])
            {
                height[left] >= left_max ? (left_max = height[left]) : ans += (left_max - height[left]);
                ++left;
            }
            else
            {
                height[right] >= right_max ? (right_max = height[right]) : ans += (right_max - height[right]);
                --right;
            }
        }
        return ans;
    }
};
int main(void)
{
    int a[] = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    vector<int> height(begin(a), end(a));
    Solution solution;
    int capacity = solution.trap(height);
    //nt capacity = 0;
    printf("capacity:%d\r\n", capacity);
    return 0;
}
