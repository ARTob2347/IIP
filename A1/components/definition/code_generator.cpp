#include "../declaration/code_generator.h"
#include <sstream>
#include <string>
std::string generate_cpp(const SimpleForm& form) {
	std::ostringstream out;
	out << "#include <iostream>\n\n";
	out << "int main() {\n";
	out << "    std::cout << \"form: " << form.name <<"\";\n";
	for (const auto & comp : form.a) {
		out << "    std::cout << \"x: " << comp.x << " , y: " << comp.y << " , name: " << comp.name << " , type: " << comp.type<< "\";\n";
	}
	out << "    return 0;\n";
	out << "}\n";
	return out.str();
}
