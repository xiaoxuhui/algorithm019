/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */

 struct ListNode {
      int val;
      ListNode *next;
      ListNode(int x) : val(x), next(nullptr) {}
 };

class Solution
{
public:
    ListNode *detectCycle(ListNode *head)
    {
        // if(head==NULL || head->next == NULL) {
        //     return NULL;
        // }
        // ListNode *fastNode = head->next;
        // ListNode *slowNode = head;
        // while(fastNode != slowNode) {
        //     if(fastNode == NULL || fastNode->next == NULL) {
        //         return NULL;
        //     }
        //     fastNode = fastNode->next->next;
        //     slowNode = slowNode->next;
        // }
        // ListNode *pNode = head;
        // while(pNode != slowNode) {
        //     pNode=pNode->next;
        //     slowNode=slowNode->next;
        // }
        // return pNode;
        ListNode *slow = head, *fast = head;
        while (fast != nullptr)
        {
            slow = slow->next;
            if (fast->next == nullptr)
            {
                return nullptr;
            }
            fast = fast->next->next;
            if (fast == slow)
            {
                ListNode *ptr = head;
                while (ptr != slow)
                {
                    ptr = ptr->next;
                    slow = slow->next;
                }
                return ptr;
            }
        }
        return nullptr;
    }
};
