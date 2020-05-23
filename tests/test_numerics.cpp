#include "shipyard.hpp"

void valid_ints()
{
	sy::Assert(1, 1);
	sy::Assert(2, 2);
}

void valid_floats()
{
	sy::Assert(3.14, 3.14);
	sy::Assert(7.314314, 7.314314);
}

void invalid_ints()
{
	sy::Assert(1, 2);
	sy::Assert(2, 5);
}

/*
 * Special test due to some bugs enocountered with uints.
 */
void valid_uint32s()
{
	uint32_t n1 = 2;
	unsigned int n2 = 2;
	sy::Assert(n1, n2);
}

void invalid_floats()
{
	sy::Assert(3.14, 3.35);
	sy::Assert(8.3143314, 7.31431314);
}

void test_valid_numerics()
{
	sy::AssertPasses(TESTFUNC(valid_ints));
	sy::AssertPasses(TESTFUNC(valid_floats));
	sy::AssertPasses(TESTFUNC(valid_uint32s));
}

void test_invalid_numerics()
{
	AssertThrows(invalid_ints, sy::AssertionError);
	AssertThrows(invalid_floats, sy::AssertionError);
}
