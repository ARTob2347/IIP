#include "components/declaration/form_model.h"
#include "components/declaration/code_generator.h"
#include <iostream>
#include <fstream>
int main() {
	SimpleForm form;
	form.name = "test";
	form.width = 500;
	form height = 300;
	std::string code=generate_cpp(form);
	std::ofstream out("generated_main.cpp");
	out << code;
	std::cout << "+";
	return 0;

}