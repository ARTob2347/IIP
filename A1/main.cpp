#include "components/declaration/form_model.h"
#include "components/declaration/code_generator.h"
#include "components/declaration/saves.h"
#include "components/declaration/commands.h"
#include <iostream>
#include <fstream>
#include "external/imgui/imgui.h"
#include "external/imgui/backends/imgui_impl_glfw.h"
#include "external/imgui/backends/imgui_impl_opengl3.h"
#include "external/imgui/backends/imgui_stdlib.h"
#include "GLFW/glfw3.h"
void nekopiruj(SimpleForm& form) {
	setlocale(LC_ALL, "Russian");
	while (true) {
		std::string choice;
		std::cout << "Добавить новый компонент? Y/N\n";
		std::cin >> choice;
		if (choice == "Y") {
			BaseComponent p;
			std::cin >> p.type;
			std::cin >> p.name;
			std::cin >> p.x;
			std::cin >> p.y;
			form.a.push_back(p);
		}
		else if (choice == "N") {
			std::cout << "Хочешь ли ты сохранить проект? Y/N\n";
			std::cin >> choice;
			if (choice == "Y") {
				std::cout << "Как назвать файл?\n";
				std::string filename;
				std::cin >> filename;
				save(form, filename);
			}
			else if (choice == "N") {
				break;
			}
			else {
				std::cout << "не то, напиши Y — да, N — нет\n";
			}
			break;
		}
		else {
			std::cout << "не то, напиши Y — да, N — нет\n";
		}
	}
}
int main() {
	if (!glfwInit()) return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Dear ImGui Test", NULL, NULL);
	if (window == NULL) return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	static char man[256]="$";
	int n = 10;
	static int temp_sh = 800;
	static int temp_vi = 600;
	static std::string temp_na = "Untitled";
	static bool temp_okno = false;
	static char* filebuttons[] = {"Сохранить как","Загрузить","Создать"};
	static int selectedindex = 0;
	SimpleForm form;


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("Файл")) {
					if (ImGui::MenuItem("Сохранить как", "Ctrl+S")) {
						/*save(form, save_name);*/
					}
					if (ImGui::MenuItem("Загрузить")) {
						/*load(form, save_name);*/
					}
					if (ImGui::MenuItem("Создать", "Ctrl+N")) {
						temp_okno = true;

					}
					temp_okno = false;
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Бегать")) {
					std::string code = generate_cpp(form);
					std::ofstream out("generated_main.cpp", std::ios::binary);
					out << code;
					irun("generated_main.cpp");
				}
				ImGui::EndMenuBar();
			}
			if (temp_okno) {
				ImGui::OpenPopup("Создание нового проекта");
			}
			if (ImGui::BeginPopupModal("Создание нового проекта", NULL)) {
				ImGui::Text("Напиши ширину");
				ImGui::InputInt("",&temp_sh);
				ImGui::Text("Напиши высоту");
				ImGui::InputInt("", &temp_vi);
				ImGui::Text("Напиши название");
				ImGui::InputText("", &temp_na);
				if (ImGui::Button("ОК",ImVec2(30,60))) {
					form.width = temp_sh;
					form.height = temp_vi;
					form.name = temp_na;
					form.a.clear();
					temp_okno = false;
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Отмена", ImVec2(30, 60))) {
					ImGui::CloseCurrentPopup();
					temp_okno = false;
				}
				ImGui::EndPopup();
			}
			ImGui::Separator();
			ImGui::SetNextWindowSize(ImVec2(200, 100));
			ImGui::Begin("Что-то там");
			/*Ввод ширины и высоты
			кнопки ок и отмена*/
			ImGui::End();
		}
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	/*setlocale(LC_ALL, "Russian");
	SimpleForm form;
	form.name = "test";
	form.width = 500;
	form.height = 300;
	std::string choice;
	std::cout << "Хотите ли вы загрузить проект? Y/N";
	std::cout << "\n";
	std::cin >> choice;
	if (choice == "N") {
		nekopiruj(form);
	}
	else if (choice == "Y") {
		std::cout << "Как называется файл сохранения?";
		std::cout << "\n";
		std::string save_name;
		std::cin >> save_name;
		form=load(form, save_name);
		nekopiruj(form);
	}
	else {
		std::cout << "не то, напиши Y — да, N — нет\n";
	}
	std::string code=generate_cpp(form);
	std::ofstream out("generated_main.cpp", std::ios::binary);
	out << code;
	std::cout << "+";*/

	return 0;
}
//ПРИМЕРЫ
/*ImGui::Text("");
ImGui::InputText("Ввод:", &man);
if (ImGui::Button("Кнопка")) {
	man = "";
}
ImGui::Text("номер %d",n)*/