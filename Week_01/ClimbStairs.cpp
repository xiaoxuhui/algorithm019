class Solution {
public:

    Solution() {

    }
    int climbStairs(int n) {
        if( n <= 2) {
            return n;
        }
        int v1 = 1;
        int v2 = 2;
        int v3 = 3;
        for (int i = 3; i < n+1; ++i) {
            v3 = v1 + v2;
            v1 = v2;
            v2 = v3;
        }
        return v3;
    }
};