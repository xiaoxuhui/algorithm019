#include <vector>
using namespace std;

class Solution {
public:

    int minCostClimbingStairs(vector<int>& cost) {
        const int len = cost.size();
        if(len == 0){
            return 0;
        }
        if(len <= 2){
            return cost[len-1];
        }
        vector<int> dp(len + 1,0);
        dp[1] = min(cost[1],cost[0]);
        for(int i = 2;i < cost.size();++i) {
            dp[i] = min(dp[i - 1] + cost[i],dp[i-2] + cost[i-1]);
        }
        return dp[len-1];
    }
};
