/**
 * Definition for a binary tree node.
 */
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
#include <vector>
using namespace std;

class Solution {
public:
    void dfs(TreeNode *root,int level,vector<int> &result) {
        if(root == nullptr) {
            return;
        }
        if(level == result.size()) {
            result.push_back(root->val);
        }else{
            result[level] = max(result[level],root->val);
        }        
        dfs(root->left,level+1,result);
        dfs(root->right,level+1,result);
    }

    vector<int> largestValues(TreeNode* root) {
        vector<int> result;
        dfs(root,0,result);
        return result;
    }
};