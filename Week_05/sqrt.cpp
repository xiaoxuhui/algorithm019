// class Solution {
// public:
//     int mySqrt(int x) {
//         if(x <= 1){
//             return x;
//         }
//         int left = 1;
//         int right = x;
//         long mid =  left + (right - left) / 2;
//         while(left <= right) {
//             mid = left + (right - left) / 2;
//             if((mid * mid) <= x) {
//                 left = mid + 1;
//             }
//             else{
//                 right = mid - 1;
//             }
//         }
//         return right;
//     }
// };

class Solution {
public:
    int mySqrt(int x) {
        if (x == 0) {
            return 0;
        }

        long C = x, x0 = x;
        while (true) {
            long xi = ((x0 + C / x0) >> 1);
            if (x0 <= xi) {
                break;
            }
            x0 = xi;
        }
        return int(x0);
    }
};
