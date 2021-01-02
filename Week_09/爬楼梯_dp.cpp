#include <vector>
using namespace std;

class Solution {
public:

    Solution() {

    }
    int climbStairs(int n) {
        if( n <= 2) {
            return n;
        }
        vector<int> dp(n+1,0);
        dp[1] = 1;
        dp[2] = 2;
        for ( int i = 3; i < n + 1; ++i ) {  
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        return dp[n];

        // int c1 = 1;
        // int c2 = 2;
        // int c3 = 3;
        // for ( int i = 3; i < n + 1; ++i ) {  
        //     c3 = c1 + c2;
        //     c1 = c2;
        //     c2 = c3;
        // }
        // return c3;
    }
};
