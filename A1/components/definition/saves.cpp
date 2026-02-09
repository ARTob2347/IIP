#include "..\declaration\saves.h"
#include <iostream>
#include <fstream>
void save(const SimpleForm form, const std::string save_name) {
	setlocale(LC_ALL, "Russian");
	std::ofstream out(save_name + ".txt");
	out << form.height << "\n" << form.width << "\n" << form.name<< "\n";
	out << form.a.size() << "\n";
	for (const auto& comp : form.a) {
		out << comp.type << "\n" << comp.name << "\n" << comp.x << "\n" << comp.y << "\n";
	}

}
SimpleForm load(SimpleForm form, const std::string save_name) {
	std::ifstream in(save_name + ".txt");
	form.a.clear();
	in >> form.height;
	in >> form.width;
	int k = 0;
	in >> form.name;
	in >> k;
	for (int i = 0; i < k; i++) {
		BaseComponent p;
		in >> p.type;
		in >> p.name;
		in >> p.x;
		in >> p.y;
		form.a.push_back(p);
	}
	return form;
}