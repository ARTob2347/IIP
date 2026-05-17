#include "..\declaration\saves.h"
#include <iostream>
#include <fstream>
void save(const SimpleForm form, const std::string save_name) {
	setlocale(LC_ALL, "Russian");
	std::ofstream out(save_name + ".txt");
	out << form.height << "\n" << form.width << "\n" << form.name << "\n";
	out << form.a.size() << "\n";
	for (const auto& comp : form.a) {
		out << comp.type << "\n" << comp.name << "\n" << comp.x << "\n" << comp.y << "\n";
		out << comp.caption << "\n" << comp.height << "\n" << comp.text << "\n" << comp.width << "\n" << comp.checked << "\n";
	}
}
SimpleForm load(SimpleForm form, const std::string save_name) {
	std::ifstream in(save_name);
	form.a.clear();
	in >> form.height;
	in >> form.width;
	in.ignore();
	std::getline(in, form.name);
	int k = 0;
	in >> k;
	in.ignore();
	for (int i = 0; i < k; i++) {
		BaseComponent p;
		std::getline(in, p.type);
		std::getline(in, p.name);
		in >> p.x;
		in >> p.y;
		in.ignore();
		std::getline(in, p.caption);
		in >> p.height;
		in.ignore();
		std::getline(in, p.text);
		in >> p.width;
		in >> p.checked;
		in.ignore();
		form.a.push_back(p);
	}
	return form;
}
