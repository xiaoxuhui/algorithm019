/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

 struct ListNode {
      int val;
      ListNode *next;
      ListNode() : val(0), next(nullptr) {}
      ListNode(int x) : val(x), next(nullptr) {}
      ListNode(int x, ListNode *next) : val(x), next(next) {}
 };
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode *tmp = new ListNode(0);
        tmp->next=head;
        ListNode *first = tmp;
        while(first->next!=nullptr && first->next->next != nullptr) {
            ListNode *node1 = first->next;
            ListNode *node2 = first->next->next;
            first->next = node2;
            node1->next = node2->next;
            node2->next = node1;
            first = node1;
        }
        return tmp->next;
    }

    // ListNode* swapPairs(ListNode* head) {
    //         if(head == nullptr ||head->next == nullptr) {
    //             return head;
    //         }
    //         ListNode *next = head->next;
    //         head->next = swapPairs(next->next);
    //         next->next = head;
    //         return next;
    // }
};
