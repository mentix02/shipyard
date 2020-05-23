#include "shipyard.hpp"

void valid_unordered_map()
{
	std::unordered_map<std::string, int8_t> um1 = {
		{"a", 1},
		{"b", 2},
		{"c", 3},
	};
	std::unordered_map<std::string, int8_t> um2 = {
		{"b", 2},
		{"a", 1},
		{"c", 3},
	};
	sy::Assert(um1, um2);
}

void invalid_unordered_map_keys()
{
	std::unordered_map<std::string, int8_t> um1 = {
		{"a", 2},
		{"b", 3},
	};
	std::unordered_map<std::string, int8_t> um2 = {
		{"c", 2},
		{"b", 3},
	};
	sy::Assert(um1, um2);
}

void invalid_unordered_map_values()
{
	std::unordered_map<std::string, int8_t> um1 = {
		{"a", 2},
		{"b", 3},
	};
	std::unordered_map<std::string, int8_t> um2 = {
		{"a", 2},
		{"b", 5},
	};
	sy::Assert(um1, um2);
}

void test_valid_unordered_map()
{
	sy::AssertPasses(TESTFUNC(valid_unordered_map));
}

void test_invalid_unordered_map_keys()
{
	AssertThrows(invalid_unordered_map_keys, sy::AssertionError);
}

void test_invalid_unordered_map_values()
{
	AssertThrows(invalid_unordered_map_values, sy::AssertionError);
}
