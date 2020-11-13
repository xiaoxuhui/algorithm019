/**
 * Definition for singly-linked list.
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
    // ListNode* oddEvenList(ListNode* head) {
    //     if (!head || !head->next) {
    //         return head;
    //     }
    //     ListNode *oddStart  = head;
    //     ListNode *evenStart = head->next;
    //     ListNode *evenEnd   = head->next;
    //     ListNode *curr      = evenEnd->next;

    //     while (curr != nullptr) {
    //         oddStart->next = curr;
    //         evenEnd->next  = curr->next;
    //         evenEnd    = curr->next;
    //         curr->next = evenStart;

    //         oddStart = curr;
    //         curr = !evenEnd ? nullptr : evenEnd->next;
    //     }

    //     return head;
    // }

    ListNode* oddEvenList(ListNode* head) {
        if(head == nullptr || head->next == nullptr) {
            return head;
        }
        ListNode oddNode(0);
        ListNode evenNode(0);
        ListNode *evenNode_tmp = &evenNode;
        ListNode *oddNode_tmp = &oddNode;
        ListNode *tmpNode = head;
        int i = 0;
        while(tmpNode != nullptr){
            if(i % 2 == 0) {
                evenNode_tmp->next = tmpNode;
                evenNode_tmp = evenNode_tmp->next;
            }
            else {
                oddNode_tmp->next = tmpNode;
                oddNode_tmp = oddNode_tmp->next;
            }
            ++i;
            tmpNode = tmpNode->next;
        };
        if(i%2 == 1){
            oddNode_tmp->next = nullptr;
        }
        evenNode_tmp->next = oddNode.next;
        return evenNode.next;
    }
};
