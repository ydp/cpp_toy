#include <iostream>

using namespace std;

struct Node{
    int data;
    Node* next;
};

void print(Node* head)
{
    cout << "---------" << endl;
    Node *p = head;
    while(p != NULL)
    {
        cout << p->data << endl;
        p = p->next;
    }
}

void reverse(Node*& head)
{
    Node* pre=NULL;
    Node* next = NULL;
    Node* cur = head;
    while(cur!=NULL)
    {
        next = cur->next;
        cur->next=pre;
        pre = cur;
        cur = next;
    }
    head = pre;
}

Node* recursiveReverse(Node * node)
{
    if (node == NULL) return NULL;
    if (node->next == NULL) return node;
    Node* pre = node;
    Node* cur = node->next;
    Node* res = recursiveReverse(node->next);
    cur->next = pre;
    /// 最难的一步
    pre->next = NULL;
    return res;
}

void insert(Node*& head, int v)
{
    Node* t = head;
    Node * p = new Node();
    p->data = v;
    p->next = t;
    head = p;
}

int main(int argc, char*argv[])
{
    Node * head = new Node();
    head->data = 2;
    head->next =NULL;
    insert(head, 3);
    insert(head, 4);
    insert(head, 5);
    insert(head, 6);
    print(head);
    reverse(head);
    print(head);
    Node* res = recursiveReverse(head);
    print(res);

    return 0;
}
