#pragma once
#include <string>
#include <vector>
struct BaseComponent {
	std::string type;
	std::string name;
	int x, y;
};
struct SimpleForm {
	int height;
	int width;
	std::string name;
	std::vector <BaseComponent> a;
};
