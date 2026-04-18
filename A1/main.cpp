#include "components/declaration/form_model.h"
#include "components/declaration/code_generator.h"
#include "components/declaration/saves.h"
#include "components/declaration/commands.h"
#include <iostream>
#include <fstream>
#include "external/imgui/imgui.h"
#include "external/imgui/backends/imgui_impl_glfw.h"
#include "external/imgui/backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include <windows.h>
#include <commdlg.h>

std::string OpenFileDialog() {
	OPENFILENAMEA ofn;
	char szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetForegroundWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.txt\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn)) {
		return std::string(szFile);
	}
	return "";
}

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
	static char man[256] = "$";
	int n = 10;
	static int temp_sh = 800;
	static int temp_vi = 600;
	static char temp_na[128] = "Untitled";
	static bool temp_okno = false;
	static char* filebuttons[] = { "Сохранить как","Загрузить","Создать" };
	static int selectedindex = 0;
	static bool form_open = false;
	static bool open_save_dialog = false;
	static bool open_load_dialog = false;
	static char save_dialog_name[128] = "";
	SimpleForm form;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("Fignya")) {
					if (ImGui::MenuItem("Сохранить как", "Ctrl+S")) {
						open_save_dialog = true;
					}
					if (ImGui::MenuItem("Загрузить")) {
						std::string file = OpenFileDialog();
						std::cout << file;
						if (!file.empty()) {
							form = load(form, file);
							form_open = true;
						}
					}
					if (ImGui::MenuItem("Создать", "Ctrl+N")) {
						temp_okno = true;
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Бегать")) {
					std::string code = generate_cpp(form);
					std::ofstream out("generated_main.cpp", std::ios::binary);
					out << code;
					irun("generated_main.cpp");
				}
				ImGui::EndMainMenuBar();
			}
			if (open_save_dialog) {
				ImGui::OpenPopup("Сохранить как");
				open_save_dialog = false;
			}
			if (ImGui::BeginPopupModal("Сохранить как", NULL)) {
				ImGui::Text("Имя файла");
				ImGui::InputText("##savename", save_dialog_name, IM_ARRAYSIZE(save_dialog_name));
				if (ImGui::Button("OK", ImVec2(80, 60))) {
					save(form, save_dialog_name);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if (temp_okno) {
				ImGui::OpenPopup("Создание нового проекта");
				temp_okno = false;
			}
			if (ImGui::BeginPopupModal("Создание нового проекта", NULL)) {
				ImGui::Text("Напиши ширину");
				ImGui::InputInt("##sh", &temp_sh);
				ImGui::Text("Напиши высоту");
				ImGui::InputInt("##vi", &temp_vi);
				ImGui::Text("Напиши название");
				ImGui::InputText("##natrij", temp_na, IM_ARRAYSIZE(temp_na));
				if (ImGui::Button("OK", ImVec2(100, 60))) {
					form.width = temp_sh;
					form.height = temp_vi;
					form.name = temp_na;
					form.a.clear();
					temp_okno = false;
					ImGui::CloseCurrentPopup();
					form_open = true;
				}
				ImGui::SameLine();
				if (ImGui::Button("X", ImVec2(100, 60))) {
					ImGui::CloseCurrentPopup();
					temp_okno = false;
				}
				ImGui::EndPopup();
			}
			if (form_open == true && form.width > 0 && form.height > 0) {
				ImGui::SetNextWindowSize(ImVec2(form.width, form.height), ImGuiCond_FirstUseEver);
				ImGui::Begin(form.name.c_str(), nullptr);
				for (auto& v : form.a) {
					ImGui::SetCursorPos(ImVec2(v.x, v.y));
					if (v.type == "Button") {
						if (ImGui::Button(v.name.c_str(), ImVec2(120, 30))) {

						}
					}
				}
				ImGui::End();
			}
			ImGui::Separator();
			ImGui::SetNextWindowSize(ImVec2(200, 100));
			ImGui::Begin("Что-то там");
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
}