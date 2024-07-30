#include "../include/lexer.hpp"
#include <string>

std::vector<std::string> Lexer::split(std::string line) {
    std::vector<std::string> ret;
    char prev = ' ';
    for (char &character : line) {
        if (!std::isspace(character)) {
            if (std::isspace(prev)) {
                std::string new_token;
                ret.push_back(new_token);
            }
            ret.back().push_back(character);
        }
        prev = character;
    }
    return ret;
}

long long Lexer::parse_int(std::string s){
	return std::stoll(s);
}
