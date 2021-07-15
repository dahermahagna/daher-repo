#include <stddef.h> /*NULL*/
#include <stdio.h> /*printf*/


#include "../../utils/test.h"

#define IS_THE_SAME(x, y) (x == y)

typedef struct node
{
	void* data;
	struct node *next;
}node_t;

node_t *Flip(node_t *head);
int HasLoop(const node_t *head);
node_t *FindIntersection(node_t *head_1, node_t *head_2);
node_t *FindIntersection2(node_t *head_1, node_t *head_2);

test_status_t FlipTest();
test_status_t HasLoopTest();
test_status_t FindIntersectionTest();


int main()
{
	RUNTEST(FlipTest);
	RUNTEST(HasLoopTest);
	RUNTEST(FindIntersectionTest);
	return 0;

}

test_status_t FlipTest()
{
	node_t *head;	
	node_t node1,node2,node3,node4,node5;
	int res = 0;
	
	node1.data = (void*)1;
	node1.next = &node2;
	node2.data = (void*)2;
	node2.next = &node3;
	node3.data = (void*)3;
	node3.next = &node4;
	node4.data = (void*)4;
	node4.next = &node5;
	node5.data = (void*)5;
	node5.next = NULL;

	head = &node1;
	head = Flip(head);

	while(NULL != head)
	{
		res = res *10 +*(int*)&head->data;
		head = head->next;
	}
	REQUIRE(res == 54321);
	return PASSED;
}


test_status_t HasLoopTest()
{
	node_t *head;	
	node_t node1,node2,node3,node4,node5;
	
	node1.data = (void*)1;
	node1.next = &node2;
	node2.data = (void*)2;
	node2.next = &node3;
	node3.data = (void*)3;
	node3.next = &node4;
	node4.data = (void*)4;
	node4.next = &node5;
	node5.data = (void*)5;
	node5.next = NULL;

	head = &node1;
	REQUIRE(0 == HasLoop(head));
	return PASSED;
}

test_status_t FindIntersectionTest()
{

	node_t node1,node2,node3,node4,node5;
	
	node1.data = (void*)1;
	node1.next = &node3;
	node2.data = (void*)2;
	node2.next = &node3;
	node3.data = (void*)3;
	node3.next = &node4;
	node4.data = (void*)4;
	node4.next = &node5;
	node5.data = (void*)5;
	node5.next = NULL;

	REQUIRE(&node3 == FindIntersection(&node1, &node2));
	
	return PASSED;
}



node_t *Flip(node_t *head)
{
	node_t *next_one = NULL;
	node_t *previus_one = NULL; 

	while (NULL != head)
	{
		next_one = head->next;
		head->next = previus_one;
		previus_one = head;
		head = next_one;
	}

	return previus_one;
}

int HasLoop(const node_t *head)
{
	const node_t *slow_runner = head;
	node_t *usain_runner = head->next;

	while (!IS_THE_SAME(slow_runner, usain_runner) && NULL != usain_runner->next && NULL != usain_runner->next->next)
	{
		slow_runner = slow_runner->next;
		usain_runner = usain_runner->next->next;
	}
	return IS_THE_SAME(slow_runner, usain_runner);
}

node_t *FindIntersection(node_t *head_1, node_t *head_2)
{

	while(head_1->next != NULL)
	{
		head_1->data = (void*)(-1);
		head_1 = head_1->next;
	}

	while(head_2->next != NULL && head_2->data != (void*)(-1))
	{
		head_2 = head_2->next;
	}

	return (head_2);
}









