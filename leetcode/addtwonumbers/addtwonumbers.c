/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
#include <stdio.h>
#include <math.h>

struct ListNode {
    int val;
    struct ListNode *next;
};


int list2int(struct ListNode*lx){

    int tens = 0;
    int number = 0;
    int temp = 0;


    struct ListNode*trav = lx;

    while(trav != NULL){
        temp = trav->val;
        temp = pow(10, tens) * temp;
        number = number + temp;
        tens++;
        trav = trav->next;
    }



    return number;


}
int addTwoNumbers(struct ListNode* l1, struct ListNode* l2){

    return list2int(l1);
}

int main(void)
{
    int a = 5;
    
    printf("hello world \n");
}