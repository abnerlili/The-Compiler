#include "interpreter.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    Interpreter interpreter;

    if (argc < 2) { 
        std::cout << "Need a source file\n"; 
        return -1;
    }

    try {
        interpreter.Run(argv[1]);
    } catch (Error::IOError e) {
        std::cout << "IO Error    : " << e.error << '\n';
    } catch (Error::SyntaxError e) {
        std::cout << "Syntax Error: " << e.error << '\n';
    }

    return 0;
}