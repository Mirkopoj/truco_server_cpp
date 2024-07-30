#pragma once

#include <string>
#include <vector>

namespace Lexer {

/**
 * @brief Divide una línea de texto en palabras individuales.
 * @param line La línea de texto.
 * @return Vector de palabras identificadas.
 */
std::vector<std::string> split(std::string line);

/**
 * @brief convierte un string con un numero repersentado en
 * base decimal, a su valor numerico.
 * @param s es el string a procesar.
 * @return el numero represtado por el string.
 */
long long parse_int(std::string s);

}; // namespace Lexer
