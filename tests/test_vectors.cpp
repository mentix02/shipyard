#include "shipyard.hpp"

void valid_vectors()
{
	std::vector<int8_t> v1 = {1, 2, 3, 4};
	std::vector<int8_t> v2 = {1, 2, 3, 4};
	sy::Assert(v1, v2);
}

void invalid_vectors()
{
	std::vector<int8_t> v1 = {2, 3, 1, 4};
	std::vector<int8_t> v2 = {1, 3, 0, 4};
	sy::Assert(v1, v2);
}

void invalid_vectors_size()
{
	std::vector<int8_t> v1 = {2, 3, 1, 4, 6};
	std::vector<int8_t> v2 = {1, 3, 0, 4, 5};
	sy::Assert(v1, v2);
}

void test_valid_vectors()
{
	sy::AssertPasses(TESTFUNC(valid_vectors));
}

void test_invalid_vectors()
{
	AssertThrows(invalid_vectors, sy::AssertionError);
}

void test_invalid_vectors_size()
{
	AssertThrows(invalid_vectors_size, sy::AssertionError);
}
