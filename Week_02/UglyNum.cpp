#include <vector>
using namespace std;

class Solution
{
private:
public:
    // int nthUglyNumber(int n)
    // {
    //     vector<int> dp(n, 0);
    //     dp[0] = 1;
    //     int p2 = 0, p3 = 0, p5 = 0;
    //     for (int i = 1; i < n; i++)
    //     {
    //         dp[i] = min(min(dp[p2] * 2, dp[p3] * 3), dp[p5] * 5);
    //         if (dp[i] == dp[p2] * 2)
    //             p2++;
    //         if (dp[i] == dp[p3] * 3)
    //             p3++;
    //         if (dp[i] == dp[p5] * 5)
    //             p5++;
    //     }
    //     return dp[n - 1];
    // }

	int nthUglyNumber(int n) {
        int *uglyNum = new int[n];
        uglyNum[0]=1;
        int p2 = 0;
        int p3 = 0;
        int p5 = 0;
        for (int i = 1;i < n; ++i) {
            uglyNum[i] = min(min(uglyNum[p2] * 2,uglyNum[p3] * 3),uglyNum[p5] * 5);
            if(uglyNum[i] == uglyNum[p2] * 2){
                p2++;
            }
            if(uglyNum[i] == uglyNum[p3] * 3){
                p3++;
            }
            if(uglyNum[i] == uglyNum[p5] * 5){
                p5++;
            }
        }

        int uglyNum_n = uglyNum[n-1];
        delete []uglyNum;
        return uglyNum_n;
	}
};
