/**
 * Definition for singly-linked list.
 */
#include <vector>
using namespace std;

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
    ListNode *reverseKGroup(ListNode *head, int k)
    {
        ListNode prev(0);
        prev.next = head;
        ListNode *last = &prev, *cur = &prev;
        int count = 0;
        while (cur)
        {
            ++count; // 负责记录是否达到反转点
            cur = cur->next;
            if (count == k && cur)
            {
                ListNode *bak = last->next; //暂存尾部节点
                while (--count)
                { // 每次将last->next插入到cur->next，且last->next指向last->next->next
                    ListNode *tmp = last->next;
                    last->next = tmp->next;
                    tmp->next = cur->next;
                    cur->next = tmp;
                }
                last = cur = bak;
            }
        }
        return prev.next;
    }
};

//[1,2,3,4,5]
//2
ListNode *createNodeList(vector<int> &valueList)
{
    ListNode head(0);
    ListNode *prev = &head;
    for (size_t i = 0; i < valueList.size(); i++)
    {
        ListNode *node = new ListNode(valueList[i]);
        prev->next = node;
        prev = node;
    }
    return head.next;     
}

void printNodeList(ListNode *nodeList)
{
    ListNode *cur = nodeList;
    printf("list:");
    while (cur != nullptr)
    {
        printf("%d ",cur->val);
        cur = cur->next;
    }
    printf("\r\n");
}

int main(void)
{
    Solution solution;
    int a[5]={1,2,3,4,5};
    vector<int> valueList(a,a+5);
    ListNode *nodeList = createNodeList(valueList);
    printNodeList(nodeList);
    ListNode *reserseNode = solution.reverseKGroup(nodeList, 2);
    printNodeList(reserseNode);
    return 0;
}
