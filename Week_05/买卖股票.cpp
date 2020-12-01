#include <vector>
using namespace std;

class Solution {
public:
    int maxProfit_1(vector<int>& prices) {// 贪心算法
        if(prices.size() <= 1){
            return 0;
        }
        int sum = 0;
        for(int i = 1;i < prices.size();++i){
            sum += max(0,prices[i] - prices[i - 1]);
        }
        return sum;
    }

    int maxProfit_2(vector<int>& prices) {//动态规划
        int n = prices.size();
        if(n <= 1){
            return 0;
        }
        int dp[n][2];
        dp[0][0] = 0;
        dp[0][1] = -prices[0];// 持有1只股票的价值
        int sum = 0;
        for(int i = 1;i < prices.size();++i){
            dp[i][0] = max(dp[i - 1][0],dp[i - 1][1] + prices[i]);// 拥有0只股票的剩余价值
            dp[i][1] = max(dp[i - 1][1],dp[i - 1][0] - prices[i]);// 拥有1只股票的剩余价值
        }
        return dp[n - 1][0];
    }

    int maxProfit(vector<int>& prices) {//动态规划
        //return  maxProfit_1(prices);
        return  maxProfit_2(prices);
    }
};