#include "types.h"
#include "graph.h"
#include "node.h"

#include <cstdio>
#include <ctime>
#include <cstring>

int run_test(const char *file);
int run_from_stdin();
int run_prebuilt_tests();

int main(int argc, char**argv)
{
	if(argc > 1)
	{
		if(_stricmp("tests", argv[1]) == 0)
		{
			return run_prebuilt_tests();
		}
	}

	return run_from_stdin();
}

int run_prebuilt_tests()
{
	int passed = 0;
	int run = 0;

	passed += run_test("Test_OnePathOptimizationDoesNotSkipStarers.txt"); run++;
	passed += run_test("test_2x1.txt"); run++;
	passed += run_test("test_2x2.txt"); run++;
	passed += run_test("Test_CutTest.txt"); run++;
	passed += run_test("test_3x1.txt"); run++;
	passed += run_test("test_4x3.txt"); run++;
	passed += run_test("test_5x6.txt"); run++;
	passed += run_test("test_7x6.txt"); run++;
	passed += run_test("test_snake.txt"); run++;
	passed += run_test("test_snake2.txt"); run++;
	passed += run_test("Test_NonEndNodeWithOnePathIsDead.txt"); run++;
	passed += run_test("test_massiveblockedboard.txt"); run++;
	passed += run_test("test_rule5.txt"); run++;
	passed += run_test("Test_ThereIsNotValidChoiceRightAway.txt"); run++;
	passed += run_test("Test_OnePathOptDoesNotFailOnStartNextToEnd.txt"); run++;
	passed += run_test("test_7x8.txt"); run++;

	printf("%d of %d passed", passed, run);

	return run - passed;
}

int run_from_stdin()
{
	graph *b = new graph();
	b->load(stdin, false);

	if(b->is_valid())
	{
		b->start_node->visit();
	}

	printf("%d\n", b->winners);

	return 0;
}

int run_test(const char *file)
{
	printf("%s\n", file);

	graph *b = new graph();
	b->load(file);

	time_t start = time(NULL);

	if(b->is_valid())
	{
		b->start_node->visit();
	}

	time_t end = time(NULL);

	printf("%d over %d seconds\n", b->winners, int(end - start));
	
	printf("Found %d and expected %d\n", b->winners, b->expected);

	if(b->winners == b->expected) 
	{
		printf("SUCCESS\n");
		return 1;
	}
	else
	{
		printf("ERROR (of %d)\n", b->expected - b->winners);
		return 0;
	}
}