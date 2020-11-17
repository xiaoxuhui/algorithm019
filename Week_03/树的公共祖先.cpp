/**
 * Definition for a binary tree node.
 */
 struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 };

class Solution
{

public:
    // unordered_map<int, TreeNode *> fa;
    // set<int> vis;
    // void dfs(TreeNode *root)
    // {
    //     if (root->left != nullptr)
    //     {
    //         fa[root->left->val] = root;
    //         dfs(root->left);
    //     }
    //     if (root->right != nullptr)
    //     {
    //         fa[root->right->val] = root;
    //         dfs(root->right);
    //     }
    // }
    // TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q)
    // {
    //     fa[root->val] = nullptr;
    //     dfs(root);
    //     while (p != nullptr)
    //     {
    //         vis.insert(p->val);
    //         p = fa[p->val];
    //     }
    //     while (q != nullptr)
    //     {
    //         if (vis.count(q->val) > 0)
    //             return q;
    //         q = fa[q->val];
    //     }
    //     return nullptr;
    // }

    TreeNode *ans;
    bool dfs(TreeNode *root, TreeNode *p, TreeNode *q)
    {
        if (root == nullptr)
            return false;
        bool lson = dfs(root->left, p, q);
        bool rson = dfs(root->right, p, q);
        if ((lson && rson) || ((root->val == p->val || root->val == q->val) && (lson || rson)))
        {
            ans = root;
        }
        return lson || rson || (root->val == p->val || root->val == q->val);
    }
    TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q)
    {
        dfs(root, p, q);
        return ans;
    }
};