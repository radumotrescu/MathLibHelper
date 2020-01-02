#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <obj.h>

TEST_CASE("testing")
{
	int b = f(1);
	CHECK(b == 2);
}