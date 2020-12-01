class Solution {
public:
    bool isPerfectSquare(int num) {
        if(num <= 1){
            return true;
        }
        int left = 1;
        int right = num;
        long mid = left + (right - left) / 2;
        while(left <= right) {
            mid = left + (right - left) / 2;
            long a = mid * mid;
            if(a == num) {
                return true;
            }
            if(a < num) {
                left = mid + 1;
            }
            else{
                right = mid - 1;
            }
        }
        return false;
    }
};

class Solution2 {
public:
    bool dfs(int left,int right,int num) {
        if(left > right){
            return false;
        }
        long mid = left + (right - left) / 2;
        long a = mid * mid;
        if(a == num) {
            return true;
        }
        if(a < num){
            return dfs(mid + 1,right,num);
        }
        return dfs(left,mid - 1,num);
    }

    bool isPerfectSquare(int num) {
        if(num <= 1){
            return true;
        }        
        return dfs(1,num,num);
    }
};