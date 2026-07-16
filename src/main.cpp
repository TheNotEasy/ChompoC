#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "parser/ast_printer.h"
#include "interpreter/interpreter.h"

#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <filesystem>

constexpr std::string read_file(const std::string& path) {
    std::ifstream file(path);

    if (!file) {
        throw std::runtime_error(
            "Failed to open source file: " +
            std::filesystem::absolute(path).string()
        );
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

int main() {
    try {
        std::string source;
        try {
            source = read_file("./src/test_code.chmp");
        } catch (...) {
            std::cerr << "Failed to read test_code.chmp\nPlease enter the way to file: ";
            std::string way;
            std::cin >> way;
            source = read_file(way);
        }

        Lexer lexer(source);
        const auto tokens = lexer.scan_tokens();

        std::cout << "====== Lexer ======\n";
        for (const Token& token : tokens) {
            std::cout
                << token.position.line
                << ':'
                << token.position.column
                << "  "
                << std::left
                << std::setw(14)
                << token_type_name(token.type)
                << std::quoted(token.lexeme)
                << '\n';
        }
        std::cout << "====== Parser ======\n";

        Parser parser(std::move(tokens));
        Program program = parser.parse();

        std::cout << "Parsed " << program.size() << " top-level statements\n";
        AstPrinter printer;
        std::cout << printer.print(program);
        std::cout << "======= Output =======\n";

        Interpreter interpreter(std::cout);
        interpreter.interpret(program);

    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }

    return 0;
}