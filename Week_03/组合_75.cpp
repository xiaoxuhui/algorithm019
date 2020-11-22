#include <vector>
using namespace std;

class Solution {
public:
    void dfs(vector<vector<int>> &results,int n,int index,int k, vector<int> &result) {
        if(result.size() == k) {
            results.push_back(result);
            return;
        }
        if (index == n + 1) {
            return;
        }
        result.push_back(index);
        dfs(results,n,index+1,k,result);
        result.pop_back();
        dfs(results,n,index+1,k,result);
    }
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> results;
        vector<int> result;
        dfs(results,n,1,k,result);
        return results;
    }
};
