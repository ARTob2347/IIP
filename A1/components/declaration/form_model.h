#pragma once
#include <string>
#include <vector>
struct BaseComponent {
	std::string type;
	std::string name;
	std::string caption;
	std::string text;
	bool checked = false;
	int x, y;
	int height;
	int width;
};
struct SimpleForm {
	int height;
	int width;
	std::string name;
	std::vector <BaseComponent> a;
};
