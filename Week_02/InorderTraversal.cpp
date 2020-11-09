/**
 * Definition for a binary tree node.
 */
#include <vector>
#include <stack>
using namespace std;

 struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode() : val(0), left(nullptr), right(nullptr) {}
      TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
      TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 };
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> stack;
        TreeNode *tmp = root;
        while(tmp != nullptr || !stack.empty()) {
            while(tmp != nullptr){// 一直往左压栈
                stack.push(tmp);
                tmp = tmp->left;
            }
            tmp = stack.top();
            stack.pop();// 退栈
            result.push_back(tmp->val);
            tmp = tmp->right;
        }
        return result;
    }
};
