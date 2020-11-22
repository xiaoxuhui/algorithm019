#include <map>
#include <unordered_map>
#include <vector>
using  namespace std;

class Solution {
public:
    int majorityElement(vector<int>& nums) {
        unordered_map<int,int> map;
        int halfCount = nums.size() / 2;
        for(int num:nums) {
            map[num]++;
            if(map[num] > halfCount){
                return num;
            }
        }
        return -1;
        // for(int i = 0;i < nums.size(); ++i) {
        //         map[nums[i]]++;
        //     // if(map.find(nums[i]) == map.end()){
        //     //     map[nums[i]] = 1;
        //     // }else{
        //     //     map[nums[i]]++;
        //     // }

        //     if(map[nums[i]] > nums.size() / 2) {
        //         return nums[i];
        //     }
        // }        
        // return -1;
    }
};

// 用map记录数量
