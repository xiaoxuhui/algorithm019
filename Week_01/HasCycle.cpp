#include <stdlib.h>
/**
 * Definition for singly-linked list.
 */
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution
{
public:
    bool hasCycle(ListNode *head)
    {
        if (head == NULL || head->next == NULL)
        {
            return false;
        }
        ListNode *fastNode = head->next;
        ListNode *slowNode = head;
        while (fastNode != slowNode)
        {
            if (fastNode == NULL || fastNode->next == NULL)
            {
                return false;
            }
            fastNode = fastNode->next->next;
            slowNode = slowNode->next;
        }
        return true;
    }
};