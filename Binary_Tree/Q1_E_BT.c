//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section E - Binary Trees Questions
Purpose: Implementing the required functions for Question 1 */

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////
typedef struct _btnode{
	int item;
	struct _btnode *left;
	struct _btnode *right;
} BTNode;   // You should not change the definition of BTNode

/////////////////////////////////////////////////////////////////////////////////

typedef struct _stackNode{
    BTNode *btnode;
    struct _stackNode *next;
}StackNode;

typedef struct _stack{
    StackNode *top;
}Stack;

///////////////////////// function prototypes ////////////////////////////////////

// You should not change the prototypes of these functions
int identical(BTNode *tree1, BTNode *tree2);

BTNode* createBTNode(int item);

BTNode* createTree();
void push( Stack *stk, BTNode *node);
BTNode* pop(Stack *stk);

void printTree(BTNode *node);
void removeAll(BTNode **node);

///////////////////////////// main() /////////////////////////////////////////////

int main()
{
    int c, s;
    char e;
    BTNode *root1, *root2;

    root1 = NULL;
    root2 = NULL;
    c = 1;

    printf("1: Create a binary tree1.\n");
    printf("2: Create a binary tree2.\n");
    printf("3: Check whether two trees are structurally identical.\n");
    printf("0: Quit;\n");

    while(c != 0){
        printf("Please input your choice(1/2/3/0): ");
        if(scanf("%d", &c) > 0)

        {

            switch(c)
            {
            case 1:
                removeAll(&root1);
                printf("Creating tree1:\n");
                root1 = createTree();
                printf("The resulting tree1 is: ");
                printTree(root1);
                printf("\n");
                break;
            case 2:
                removeAll(&root2);
                printf("Creating tree2:\n");
                root2 = createTree();
                printf("The resulting tree2 is: ");
                printTree(root2);
                printf("\n");
                break;
            case 3:
                s = identical(root1, root2);
                if(s){
                printf("Both trees are structurally identical.\n");
                }
                else{
                printf("Both trees are different.\n");
                }
                removeAll(&root1);
                removeAll(&root2);
                break;
            case 0:
                removeAll(&root1);
                removeAll(&root2);
                break;
            default:
                printf("Choice unknown;\n");
                break;
            }
		}
        else
        {
            scanf("%c",&e);
        }

    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

int identical(BTNode *tree1, BTNode *tree2)

{
   /* add your code here */
   BTNode * temp1 = tree1; //temp1에 1번 트리의 루트 노드 포인터 저장
   BTNode * temp2 = tree2; //temp2에 2번 트리의 루트 노드 포인터 저장
   BTNode * top1;
   BTNode * top2;
   Stack * s1 = malloc(sizeof(Stack)); //1번 트리 탐색 전용 스택 선언
   Stack * s2 = malloc(sizeof(Stack)); //2번 트리 탐색 전용 스택 선언
   s1->top = NULL;
   s2->top = NULL;

   //현재 1번 트리의 중앙 노드와 2번 트리의 중앙 노드를 스택에 저장
   push(s1, temp1);
   push(s2, temp2);

   while (temp1 != NULL || temp2 != NULL)
   {
    if (temp1 -> item != temp2 -> item) //현재 temp1에 담긴 값과 temp2에 담긴 값이 다르다면
        return 0; //FALSE

    if (temp1 -> left != NULL) //temp1의 왼쪽 노드가 NULL이 아니라면
    {
        if (temp2 -> left == NULL) //temp2의 왼쪽 노드는 NULL이라면
            return 0; //FALSE
        //현재 노드 top으로 잠시 이관
        top1 = temp1;
        top2 = temp2;
        //왼쪽으로 이동시키기
        temp1 = temp1 -> left;
        temp2 = temp2 -> left; 
        //현재 1번 트리의 중앙 노드와 2번 트리의 중앙 노드를 스택에 저장
        push(s1, temp1);
        push(s2, temp2);
        //왼쪽 방문 처리(NULL로 만들기)
        top1 -> left = NULL;
        top2 -> left = NULL;
    }
    else if (temp1 -> left == NULL && temp1 -> right != NULL) //왼쪽 노드는 NULL인데 오른쪽 노드는 NULL이 아니라면
    {
        if (!(temp2 -> left == NULL && temp2 -> right != NULL)) //오른쪽 노드는 다른 상황이라면
            return 0; //FALSE
        //각각 스택에서 pop시키기 : 만약 두 값이 서로 다르면 FALSE
        top1 = pop(s1);
        top2 = pop(s2);

        if (top1 -> item != top2 -> item)
            return 0;
        
        //top 변수들에 temp 포인터들을 저장
        top1 = temp1;
        top2 = temp2;
        temp1 = temp1 -> right; //오른쪽으로 이동
        temp2 = temp2 -> right; //오른쪽으로 이동
        //현재 1번 트리의 중앙 노드와 2번 트리의 중앙 노드를 스택에 저장
        push(s1, temp1);
        push(s2, temp2);
        top1 -> right = NULL; //오른쪽 노드 NULL처리 (=방문 완료 처리)
        top2 -> right = NULL;
        
    }
    else if (temp1 -> left == NULL && temp1 -> right == NULL) //단말 노드라면 (양쪽 다 NULL이라면)
    {
        if (!(temp2 -> left == NULL && temp2 -> right == NULL)) //오른쪽 노드는 다른 상황이라면
            return 0;
        //중앙 노드 pop하기
        top1 = pop(s1);
        top2 = pop(s2);
        if (top1 -> item != top2 -> item)
            return 0;
        //temp 노드들을 stack의 top 노드로 이동시키기 : 마지막 노드를 탐색했을 때 메모리 참조 오류 발생
        if (s1->top == NULL || s2->top == NULL) //스택에 node가 안남아있다면
        {
            break;
        }
        else //남아있다면 (한 쪽만 남아있는 상황은 발생할 수 없음. 위 쪽의 조건문들을 통해 다 걸렀기 때문)
        {
            //기존 pop함수가 스택이 비어있을 시 쓰레기 값을 반환함 : 스택 선언할 때 top을 NULL로 해줘야...
            temp1 = s1->top->btnode;
            temp2 = s2->top->btnode;
        }
        
    }

   }

   return 1;

}

/////////////////////////////////////////////////////////////////////////////////

BTNode *createBTNode(int item){
    BTNode *newNode = malloc(sizeof(BTNode));
    newNode->item = item;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

//////////////////////////////////////////////////////////////////////////////////


BTNode *createTree()
{
    Stack stk;
    BTNode *root, *temp;
    char s;
    int item;

    stk.top = NULL;
    root = NULL;

    printf("Input an integer that you want to add to the binary tree. Any Alpha value will be treated as NULL.\n");
    printf("Enter an integer value for the root: ");
    if(scanf("%d",&item) > 0)
    {
        root = createBTNode(item);
        push(&stk,root);
    }
    else
    {
        scanf("%c",&s);
    }

    while((temp =pop(&stk)) != NULL)
    {

        printf("Enter an integer value for the Left child of %d: ", temp->item);

        if(scanf("%d",&item)> 0)
        {
            temp->left = createBTNode(item);
        }
        else
        {
            scanf("%c",&s);
        }

        printf("Enter an integer value for the Right child of %d: ", temp->item);
        if(scanf("%d",&item)>0)
        {
            temp->right = createBTNode(item);
        }
        else
        {
            scanf("%c",&s);
        }

        if(temp->right != NULL)
            push(&stk,temp->right);
        if(temp->left != NULL)
            push(&stk,temp->left);
    }
    return root;
}

void push( Stack *stk, BTNode *node){
    StackNode *temp;

    temp = malloc(sizeof(StackNode));
    if(temp == NULL)
        return;
    temp->btnode = node;
    if(stk->top == NULL){
        stk->top = temp;
        temp->next = NULL;
    }
    else{
        temp->next = stk->top;
        stk->top = temp;
    }
}

BTNode* pop(Stack *stk){
   StackNode *temp, *top;
   BTNode *ptr;
   ptr = NULL;

   top = stk->top;
   if(top != NULL){
        temp = top->next;
        ptr = top->btnode;

        stk->top = temp;
        free(top);
        top = NULL;
   }
   return ptr;
}

void printTree(BTNode *node){
    if(node == NULL) return;

    printTree(node->left);
    printf("%d ",node->item);
    printTree(node->right);
}

void removeAll(BTNode **node){
    if(*node != NULL){
        removeAll(&((*node)->left));
        removeAll(&((*node)->right));
        free(*node);
        *node = NULL;
    }
}
