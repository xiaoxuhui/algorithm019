#include <stdlib.h>
/**
 * Definition for singly-linked list.
**/
struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution
{
public:

    ListNode* reverseList(ListNode* head) {
       ListNode *pre = nullptr, *cur = head, *next = nullptr;
        while (cur != nullptr)
        {
            next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    }

    // 原地反转
    ListNode *reverseList(ListNode *head)
    {
        ListNode *cur = nullptr, *pre = head;
        while (pre != nullptr)
        {
            ListNode *t = pre->next;
            pre->next = cur;
            cur = pre;
            pre = t;
        }
        return cur;
    }

    //  递归算法
    // ListNode* reverseList(ListNode* head) {
    //     if (head == NULL || head->next == NULL) {
    //         return head;
    //     }
    //     ListNode* ret = reverseList(head->next);
    //     head->next->next = head;
    //     head->next = NULL;
    //     return ret;
    // }

    //  妖魔化的双指针
    // ListNode *reverseList(ListNode *head)
    // {
    //     if (head == NULL)
    //     {
    //         return NULL;
    //     }
    //     ListNode *cur = head;
    //     while (head->next != NULL)
    //     {
    //         ListNode *t = head->next->next;
    //         head->next->next = cur;
    //         cur = head->next;
    //         head->next = t;
    //     }
    //     return cur;
    // }
};