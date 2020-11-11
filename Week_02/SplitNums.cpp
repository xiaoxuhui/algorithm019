#include <vector>
using namespace std;

/*
题目：
一个数组里面有混序的正负数，希望对它进行排序，要求是：
1、按照正负间隔的顺序排序
2、同一个符号的数相对顺序不变
3、若某一个符号的数较多，按原顺序放在最后。

例如输入：
1,2,-5,-3,-4,3,-7,7,9,-8,-11,-3,-2

期望输出：
1,-5,2,-3,3,-4,7,-7,9,-8,-11,-3,-2
*/

class Solution
{
public:
    // 1.先找到一个正数，再找到一个负数放到正数后面。
    void sortNums(vector<int> &nums)
    {
        vector<int> positiveList;
        vector<int> inpositiveList;
        for(int &num : nums) {
            if(num >= 0) {
                positiveList.push_back(num);
            }else{
                inpositiveList.push_back(num);
            }
        }

        int minNum = min(positiveList.size(),inpositiveList.size());
        for (int i = 0; i < minNum; i++)
        {
            nums[i * 2] = positiveList[i];
            nums[i * 2 + 1] = inpositiveList[i];
        }

        if(positiveList.size() > inpositiveList.size()) {
            for (size_t i = minNum; i < positiveList.size(); i++)
            {
                nums[minNum + i] = positiveList[i];
            }
        }else
        {
            for (size_t i = minNum; i < inpositiveList.size(); i++)
            {
                nums[minNum + i] = inpositiveList[i];
            }
        }
        
                

        // int len = nums.size();
        // int i = 0; // 正数下标
        // int j = 0; // 负数下标
        // int count = 0;
        // while (i < len && j < len)
        // {
        //     if (nums[i] >= 0 && nums[i + 1] < 0)
        //     {
        //         i += 2;
        //         j += 2;
        //         continue;
        //     }
        //     if (nums[i] < 0)
        //     { // 找到下一个负数
        //         i++;
        //         continue;
        //     }
        //     j++;
        //     if (nums[j] < 0)
        //     { // 找到一个负数，进行交换
        //         int tmp = nums[j];
        //         nums[j] = nums[i + 1];
        //         nums[i + 1] = tmp;
        //         i++; // 移到下一个目标位置
        //     }
        // }
    }
};

int main(void)
{
    Solution solution;
    {
        int szNums[] = {-1,-2,-3, 2, 0, -5, -3, -4, 3, -7, 7, 9, -8, -11, -3, -2};
        vector<int> nums(begin(szNums), end(szNums));
        solution.sortNums(nums);
        printf("nums:");
        for (int &num : nums)
        {
            printf(" %d", num);
        }

        printf("\r\n");
    }
    {
        int szNums[] = {1, 2, 5, 3, 0, 3, 7, 7, 9, 8, 11, 3, 2};
        vector<int> nums(begin(szNums), end(szNums));
        solution.sortNums(nums);
        printf("nums:");
        for (int &num : nums)
        {
            printf(" %d", num);
        }

        printf("\r\n");
    }
    return 0;
}
