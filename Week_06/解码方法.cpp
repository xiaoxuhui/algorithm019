#include <string>
using namespace std;
class Solution
{
public:
    int numDecodings(string s)
    {
        if (s[0] == '0')
            return 0;
        int pre = 1, curr = 1; //dp[-1] = dp[0] = 1
        for (int i = 1; i < s.size(); i++)
        {
            int tmp = curr;
            if (s[i] == '0')
                if (s[i - 1] == '1' || s[i - 1] == '2')
                    curr = pre;
                else
                    return 0;
            else if (s[i - 1] == '1' || (s[i - 1] == '2' && s[i] >= '1' && s[i] <= '6'))
                curr = curr + pre;
            pre = tmp;
        }
        return curr;
    }
    // int numDecodings(string s) {
    //     if(s == "0" || s.size() == 0){
    //         return 0;
    //     }
    //     if(s.size() == 1){
    //         return (s[0]>='1' && s[0] <='9') ? 1 : 0;
    //     }
    //     vector<int> dp(s.size(),1);
    //     dp[0] = 1;
    //      if(s[1] == '0') {
    //             if(s[0] == '1' || s[0] == '2'){
    //                 dp[1] = 1;
    //             }
    //             else{
    //                 return 0;
    //             }
    //         }
    //         else if(s[0] == '1' || (s[0] == '2' && s[1] >= '1' && s[1] <= '6')) {
    //             dp[1] = 1;
    //         }

    //     for (int i = 2; i < s.size(); ++i) {
    //         if(s[i] == '0') {
    //             if(s[i - 1] == '1' || s[i - 1] == '2'){
    //                 dp[i] = dp[i-2];
    //             }
    //             else{
    //                 return 0;
    //             }
    //         }
    //         else if(s[i - 1] == '1' || (s[i - 1] == '2' && s[i] >= '1' && s[i] <= '6')) {
    //             dp[i] = dp[i-1]+dp[i-2];
    //         }
    //     }
    //     return dp[s.size() - 1];
    // }
};
