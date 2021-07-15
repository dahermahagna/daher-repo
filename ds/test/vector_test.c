#include "../utils/test.h"
#include "vector.h"

test_status_t VectorCreateTest();
test_status_t VectorSizeTest();
test_status_t VectorPushBackTest();
test_status_t VectorCapacityTest();
test_status_t VectorPopBackTest();
test_status_t VectorGetElementTest();
test_status_t VectorSetElementTest();
test_status_t VectorReserveTest();
test_status_t VectorShrinkToFitTest();
test_status_t VectorDestroyTest();

static Vector_t *user_vector;

int main()
{
	
	RUNTEST(VectorCreateTest);
	RUNTEST(VectorSizeTest);
	RUNTEST(VectorPushBackTest);
	RUNTEST(VectorCapacityTest);
	RUNTEST(VectorPopBackTest);
	RUNTEST(VectorSetElementTest);
	RUNTEST(VectorGetElementTest);
	RUNTEST(VectorReserveTest);
	RUNTEST(VectorShrinkToFitTest);
	RUNTEST(VectorDestroyTest);


	return 0;
}

test_status_t VectorCreateTest()
{
	user_vector = VectorCreate(5);

	REQUIRE(NULL != user_vector);
	return PASSED;
}

test_status_t VectorSizeTest()
{
	REQUIRE(0 == VectorSize(user_vector));
	return PASSED;
}

test_status_t VectorPushBackTest()
{
	VectorPushBack(user_vector, (void *)15);
	VectorPushBack(user_vector, (void *)16);
	VectorPushBack(user_vector, (void *)17);
	VectorPushBack(user_vector, (void *)18);
	VectorPushBack(user_vector, (void *)19);
	VectorPushBack(user_vector, (void *)20);
	VectorPushBack(user_vector, (void *)21);
	VectorPushBack(user_vector, (void *)22);
	VectorPushBack(user_vector, (void *)23);


	REQUIRE(9 == VectorSize(user_vector));
	return PASSED;
}


test_status_t VectorCapacityTest()
{
	REQUIRE(10 == VectorCapacity(user_vector));
	return PASSED;
}

test_status_t VectorPopBackTest()
{
	VectorPopBack(user_vector);


	REQUIRE(8 == VectorSize(user_vector));
	return PASSED;
}

test_status_t VectorGetElementTest()
{	

	REQUIRE((void*)211 == VectorGetElement(user_vector,7));
	return PASSED;
}

test_status_t VectorSetElementTest()
{
	printf("%d\n", VectorGetElement(user_vector,7));
	VectorSetElement(user_vector,(void*)211,7);
	REQUIRE((void*)211 == VectorGetElement(user_vector,7));
	return PASSED;
}

test_status_t VectorReserveTest()
{
	VectorReserve(user_vector,20);
	REQUIRE(20 == VectorCapacity(user_vector));
	return PASSED;
}

test_status_t VectorShrinkToFitTest()
{
	size_t check = VectorSize(user_vector);
	VectorShrinkToFit(user_vector);
	REQUIRE(check == VectorCapacity(user_vector));
	return PASSED;
}

test_status_t VectorDestroyTest()
{
	VectorDestroy(user_vector);
	/*REQUIRE(NULL == user_vector);*/
	return PASSED;
}




