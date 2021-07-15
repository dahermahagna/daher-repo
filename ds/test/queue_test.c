#include "../utils/test.h"
#include "queue.h"



test_status_t QCreateTest();
test_status_t QDestroyTest();
test_status_t QSizeTest();
test_status_t QueueIsEmptyTest();
test_status_t QEnqueueTest();
test_status_t QDequeueTest();
test_status_t QPeekTest();
test_status_t QAppendTest();


int main()
{
	RUNTEST(QCreateTest);
	RUNTEST(QDestroyTest);
	RUNTEST(QSizeTest);
	RUNTEST(QueueIsEmptyTest);
	RUNTEST(QEnqueueTest);
	RUNTEST(QDequeueTest);
	RUNTEST(QPeekTest);
	RUNTEST(QAppendTest);


return 0; 
}

test_status_t QCreateTest()
{

	Queue_t *user_queue = QCreate();
	QEnqueue(user_queue,(void*)5);
	REQUIRE(1 == QSize(user_queue));
	QDestroy(user_queue);

	return PASSED;
}

test_status_t QDestroyTest()
{
	return PASSED;
}

test_status_t QSizeTest()
{

	Queue_t *user_queue = QCreate();
	QEnqueue(user_queue,(void*)5);
	QEnqueue(user_queue,(void*)6);
	REQUIRE(2 == QSize(user_queue));
	QDestroy(user_queue);

	return PASSED;
}

test_status_t QueueIsEmptyTest()
{

	Queue_t *user_queue = QCreate();
	REQUIRE(1 == QueueIsEmpty(user_queue));
	QEnqueue(user_queue,(void*)6);
	REQUIRE(0 == QueueIsEmpty(user_queue));
	QDestroy(user_queue);
	
	return PASSED;
}

test_status_t QEnqueueTest()
{

	Queue_t *user_queue = QCreate();
	QEnqueue(user_queue,(void*)5);
	QEnqueue(user_queue,(void*)6);
	REQUIRE(2 == QSize(user_queue));
	QDestroy(user_queue);
	

	return PASSED;
}

test_status_t QDequeueTest()
{

	Queue_t *user_queue = QCreate();
	QEnqueue(user_queue,(void*)5);
	QEnqueue(user_queue,(void*)6);
	QEnqueue(user_queue,(void*)7);

	REQUIRE(3 == QSize(user_queue));
	QDequeue(user_queue);
	REQUIRE(2 == QSize(user_queue));
	QDequeue(user_queue);
	REQUIRE(1 == QSize(user_queue));
	QDequeue(user_queue);
	REQUIRE(0 == QSize(user_queue));
	QDestroy(user_queue);

	return PASSED;
}

test_status_t QPeekTest()
{
	Queue_t *user_queue = QCreate();
	QEnqueue(user_queue,(void*)5);
	REQUIRE((void*)5 == QPeek(user_queue));

	QEnqueue(user_queue,(void*)6);
	REQUIRE((void*)5 == QPeek(user_queue));

	QDequeue(user_queue);
	REQUIRE((void*)6 == QPeek(user_queue));

	QDestroy(user_queue);
	return PASSED;
}

test_status_t QAppendTest()
{
	Queue_t *user_queue1 = QCreate();
	Queue_t *user_queue2 = QCreate();

	QAppend(user_queue1,user_queue2);
	REQUIRE(0 == QSize(user_queue1));
	REQUIRE(0 == QSize(user_queue2));

	QEnqueue(user_queue1,(void*)5);
	QEnqueue(user_queue1,(void*)6);
	QEnqueue(user_queue1,(void*)7);
	QEnqueue(user_queue1,(void*)8);
	QEnqueue(user_queue1,(void*)9);
	QEnqueue(user_queue1,(void*)10);

	QAppend(user_queue1,user_queue2);

	REQUIRE(6 == QSize(user_queue1));
	REQUIRE(0 == QSize(user_queue2));

	QEnqueue(user_queue2,(void*)5);
	QEnqueue(user_queue2,(void*)6);
	QEnqueue(user_queue2,(void*)7);
	QEnqueue(user_queue2,(void*)8);
	QEnqueue(user_queue2,(void*)9);
	QEnqueue(user_queue2,(void*)10);
	QEnqueue(user_queue2,(void*)5);
	QEnqueue(user_queue2,(void*)6);
	QEnqueue(user_queue2,(void*)7);
	QEnqueue(user_queue2,(void*)8);
	QEnqueue(user_queue2,(void*)9);
	QEnqueue(user_queue2,(void*)10);

	REQUIRE(12 == QSize(user_queue2));

	QAppend(user_queue1,user_queue2);
	REQUIRE (0 == QSize(user_queue2));
	REQUIRE (18 == QSize(user_queue1));

	QEnqueue(user_queue2,(void*)5);
	REQUIRE (1 == QSize(user_queue2));

	QDequeue(user_queue2);
	REQUIRE (0 == QSize(user_queue2));

	QAppend(user_queue2,user_queue1);
	REQUIRE (18 == QSize(user_queue2));
	REQUIRE (0 == QSize(user_queue1));


	QDestroy(user_queue1);
	QDestroy(user_queue2);


	return PASSED;
}

