#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <string>
#include <vector>

#include "../../include/lexer.hpp"

Test(empty, test) {
	std::vector<std::string> res = Lexer::split("     \r\n\t ");
	cr_assert(res.empty());
}
