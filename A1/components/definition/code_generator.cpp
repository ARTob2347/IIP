#include "../declaration/code_generator.h"
#include <sstream>
#include <string>
std::string generate_cpp(const SimpleForm& form) {
	std::ostringstream out;
	out << "#include <iostream>\n\n";
	out << "int main() {\n";
	out << "    std::cout << \"form: " << form.name << ";\n";
	out << "    return 0;\n";
	out << "}\n";
	return out.str();
}