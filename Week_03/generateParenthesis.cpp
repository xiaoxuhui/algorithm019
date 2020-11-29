#include <vector>
#include <string>
using namespace std;
#include <vector>
#include <string>
using namespace std;

class Solution {
private:    
    void backtrack(vector<string>& ans, string& cur, int open, int close, int n) {
        if (cur.size() == n * 2) {
            ans.push_back(cur);
            return;
        }
        if (open < n) {//加入左括号
            cur.push_back('(');
            backtrack(ans, cur, open + 1, close, n);
            cur.pop_back();//回退尝试
        }
        if (close < open) {
            cur.push_back(')');
            backtrack(ans, cur, open, close + 1, n);
            cur.pop_back();
        }
    }


    /**
     * @param curStr 当前递归得到的结果
     * @param left   左括号还有几个可以使用
     * @param right  右括号还有几个可以使用
     * @param res    结果集
     */
     void dfs(string curStr, int left, int right, vector<string> &res) {
        // 因为每一次尝试，都使用新的字符串变量，所以无需回溯
        // 在递归终止的时候，直接把它添加到结果集即可，注意与「力扣」第 46 题、第 39 题区分
        if (left == 0 && right == 0) {
            res.push_back(curStr);
            return;
        }

        // 剪枝（如图，左括号可以使用的个数严格大于右括号可以使用的个数，才剪枝，注意这个细节）
        if (left > right) {
            return;
        }

        if (left > 0) {
            dfs(curStr+"(", left - 1, right, res);
        }

        if (right > 0) {
            dfs(curStr+")", left, right - 1, res);
        }
        
    }

public:
    vector<string> generateParenthesis(int n) {
        vector<string> result;
        if(n == 0){
            return result;
        }
        string current;
        //backtrack(result, current, 0, 0, n);

        dfs(current, n, n, result);
        return result;
    }

};