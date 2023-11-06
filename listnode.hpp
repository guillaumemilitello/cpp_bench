#include <vector>
#include <string>
#include <sstream>

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

ListNode* createList(std::vector<int>&& vals)
{
    ListNode* head = nullptr;
    ListNode* prev = nullptr;
    for (const auto& v : vals)
    {
        ListNode* node = new ListNode(v);
        if (head == nullptr)
        {
            head = node;
        }
        else
        {
            prev->next = node;
        }
        prev = node;
    }
    return head;
}

std::ostream& operator<<(std::ostream& os, ListNode* n)
{
    bool first = true;
    os << "[";
    ListNode* curr = n;
    while(curr != nullptr)
    {
        if (first)
        {
            os << curr->val;
            first = false;
        }
        else
        {
            os << "," << curr->val;
        }
        curr = curr->next;
    }
    os << "]";
    return os;
}