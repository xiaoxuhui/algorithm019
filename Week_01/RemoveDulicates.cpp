#include <vector>
using namespace std;

class Solution
{
public:
    int removeDuplicates(vector<int> &nums)
    {
        if (nums.size() <= 1)
        {
            return nums.size();
        }
        int count = 1;
        int value = nums[0];
        for (int i = 1; i < nums.size(); ++i)
        {
            if (value != nums[i])
            {
                nums[count] = nums[i];
                count++;
                value = nums[i];
            }
        }
        return count;
    }

    // int removeDuplicates(vector<int>& nums) {
    //     if(nums.size()<=1){
    //         return nums.size();
    //     }
    //     int count = 0;
    //     for(int  i = 1;i<nums.size();++i) {
    //         if(nums[count] != nums[i]) {
    //             count++;
    //             nums[count]=nums[i];
    //         }
    //     }
    //     return count+1;
    // }    
};