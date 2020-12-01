#include <vector>
using namespace std;
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if(matrix.size() == 0){
            return false;
        }
        int m = matrix.size();
        int n = matrix[0].size();
        int end = m * n;
        int left = 0;
        int right = end - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            int curValue = matrix[mid / n][mid % n];
            if(curValue == target){
                return true;
            }
            if(curValue < target){
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        return false;
    }
};