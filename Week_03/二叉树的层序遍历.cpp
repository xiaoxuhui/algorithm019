/**
 * Definition for a binary tree node.
 */

#include <vector>
#include <queue>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    // 广度优先搜索实现方法
    vector<vector<int>> bfs(TreeNode* root) {
        vector <vector <int>> ret;
        if (nullptr == root) {
            return ret;
        }

        queue <TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int currentLevelSize = q.size();
            vector <int> levelResult;
            for (int i = 1; i <= currentLevelSize; ++i) {
                TreeNode* node = q.front(); 
                q.pop();
                levelResult.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            ret.push_back(levelResult);
        }
        
        return ret;
    }

    // 深度优先搜索实现方法
    void dfs(vector<vector<int>> &result,int level,TreeNode* root) {
        if (nullptr == root) {
            return;
        }
        if(level == result.size()) {// 加入新的一层
            result.push_back(vector<int>());
        }
        result[level].push_back(root->val);
        dfs(result,level+1,root->left);
        dfs(result,level+1,root->right);
    }

    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        dfs(result,0,root);
        return result;
        //return bfs(root);
    }
};
