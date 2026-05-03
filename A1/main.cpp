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
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
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
	static int selected = -1;
	static bool isdraw = false;
	static ImVec2 start(0,0);
	static ImVec2 current(0,0);
	static bool open_insert_modal = false;
	static const char* types[] = {
		"button","label","edit","checkbox"
	};
	BaseComponent e{"button","ALAH", "clickk","1",false,50,50,100,100};
	BaseComponent f{ "label","HALAL", "LABEL","1",false,200,200,100,100 };
	SimpleForm form;
	form.a.push_back(e);
	form.a.push_back(f);
	auto makeid = [](const BaseComponent& c, int i)->std::string {return "##" + c.name + std::to_string(i); };
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Save as", "Ctrl+S")) {
						open_save_dialog = true;
					}
					if (ImGui::MenuItem("Upload")) {
						std::string file = OpenFileDialog();
						std::cout << file;
						if (!file.empty()) {
							form = load(form, file);
							form_open = true;
						}
					}
					if (ImGui::MenuItem("Create", "Ctrl+N")) {
						temp_okno = true;
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Insert")) {
					open_insert_modal = true;
				}
				if (ImGui::MenuItem("Run")) {
					std::string code = generate_cpp(form);
					std::ofstream out("generated_main.cpp", std::ios::binary);
					out << code;
					irun("generated_main.cpp");
				}
				ImGui::EndMainMenuBar();
			}
			if (open_save_dialog) {
				ImGui::OpenPopup("Save as");
				open_save_dialog = false;
			}
			if (ImGui::BeginPopupModal("Save as", NULL)) {
				ImGui::Text("File name:");
				ImGui::InputText("##savename", save_dialog_name, IM_ARRAYSIZE(save_dialog_name));
				if (ImGui::Button("OK", ImVec2(80, 60))) {
					save(form, save_dialog_name);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if (temp_okno) {
				ImGui::OpenPopup("Creating a new project");
				temp_okno = false;
			}
			if (ImGui::BeginPopupModal("Creating a new project", NULL)) {
				ImGui::Text("Write width");
				ImGui::InputInt("##sh", &temp_sh);
				ImGui::Text("write height");
				ImGui::InputInt("##vi", &temp_vi);
				ImGui::Text("Write name");
				ImGui::InputText("##natrij", temp_na, IM_ARRAYSIZE(temp_na));
				if (ImGui::Button("OK", ImVec2(100, 60))) {
					form.width = temp_sh;
					form.height = temp_vi;
					form.name = temp_na;
					form.a.clear();
					form.a.push_back(e);
					form.a.push_back(f);
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
			if (open_insert_modal) {
				ImGui::OpenPopup("Creating new element");
				open_insert_modal = false;
			}
			if (ImGui::BeginPopupModal("Creating new element", NULL)) {
				ImGui::Text("Write type");
				int selectedtype = 0;
				//ImGui::Combo("##type", &selectedtype, types, IM_ARRAYSIZE(types));
				//ImGui::Text("write name");
				//ImGui::InputText("##vi", &temp_vi);
				//ImGui::Text("Write caption");
				//ImGui::Text("Write ");
	
				//int x, y;
				//int height;
				//int width;
				ImGui::InputText("##natrij", temp_na, IM_ARRAYSIZE(temp_na));
				if (ImGui::Button("OK", ImVec2(100, 60))) {
					form.width = temp_sh;
					form.height = temp_vi;
					form.name = temp_na;
					form.a.clear();
					form.a.push_back(e);
					form.a.push_back(f);
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
				ImDrawList* drawlist = ImGui::GetWindowDrawList();
				ImVec2 convertcord = ImGui::GetCursorScreenPos();
				ImVec2 convertsize = ImGui::GetContentRegionAvail();
				if (convertsize.x < 50) {
					convertsize.x = 50;
				}
				if (convertsize.y < 50) {
					convertsize.y = 50;
				}
				ImVec2 convertend(convertcord.x + convertsize.x, convertcord.y + convertsize.y);
				ImGui::InvisibleButton("Convert", convertsize);///Флаг!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				drawlist->AddRectFilled(convertcord,convertend,IM_COL32(251, 238, 223,255));
				bool converthovered = ImGui::IsItemHovered();
				bool clickonexist = false;
				ImVec2 mishpos = ImGui::GetIO().MousePos;
				for (int i = 0; i < form.a.size(); i++) {
					auto& b = form.a[i];
					std::string id = makeid(b,i);
					ImVec2 P1(convertcord.x + (float)b.x, convertcord.y + (float)b.y);
					ImVec2 P2(P1.x + (float)b.width, P1.y + (float)b.height);
					bool hovered = mishpos.x >= P1.x && mishpos.x <= P2.x && mishpos.y >= P1.y && mishpos.y <= P2.y;
					if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)){
						clickonexist = true;
						selected = i;
					}
					ImGui::SetCursorScreenPos(P1);
					if (b.type == "button"){
						ImGui::Button(b.caption.c_str(), ImVec2(b.width, b.height));
					}
					else if (b.type == "label") {
						ImGui::Text(b.caption.c_str());
					}
					//else if (b.caption == "edit") {
					//	ImGui::SetNextItemWidth(b.width);
						//ImGui::InputText(id.c_str(), b.text.c_str(), );//FdfdfdsffAHGFJKUDIRBNGVTFUIJHYKDSHYBGVR
					//}
					else if (b.type=="checkbox"){
						ImGui::Checkbox(b.caption.c_str(),&b.checked);
					}
					else {
						drawlist->AddRectFilled(P1, P2, IM_COL32(200, 65, 200, 255));
						drawlist->AddText(ImVec2(P1.x + 4,P1.y+4),IM_COL32(0,0,0,255),b.name.c_str());
					}
					drawlist->AddRect(P1, P2,
						selected==i ? IM_COL32(0,0,0,255): IM_COL32(5, 99, 255, 0),
						0.0f,0,2.0f);
				}
				if (converthovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !clickonexist) {
					isdraw = true;
					selected = -1;
					start = mishpos;
					current = mishpos;

				}
				if (isdraw && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
					current = mishpos;
					ImVec2 P1((start.x < current.x) ? start.x : current.x, 
						(start.y<current.y)?start.y:current.y);
					ImVec2 P2((start.x > current.x) ? start.x : current.x,
						(start.y > current.y) ? start.y : current.y);
					///if (P2.x<convertend.x && P)
					drawlist->AddRectFilled(P1, P2, IM_COL32(0, 0, 0, 0));
					drawlist->AddRect(P1, P2, IM_COL32(0, 0, 0, 255), 0.0f, 0, 2.0f);
				}
				if (isdraw && ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
					isdraw = false;
					ImVec2 P1((start.x < current.x) ? start.x : current.x,
						(start.y < current.y) ? start.y : current.y);
					ImVec2 P2((start.x > current.x) ? start.x : current.x,
						(start.y > current.y) ? start.y : current.y);
					int nx = (int)(P1.x - convertcord.x);
					int ny = (int)(P1.y - convertcord.y);
					int nw = (int)(P2.x - P1.x);
					int nh = (int)(P2.y - P1.y);
					if (nh >= 10 && nw >= 10) {
						BaseComponent pr;
						pr.type = "Pramougolnic";
						pr.name = "number"+std::to_string(form.a.size());
						pr.x = nx;
						pr.y = ny;
						pr.height = nh;
						pr.width = nw;
						form.a.push_back(pr);
						selected =(int) form.a.size() - 1;
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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);// БЕЛАЯ ТЕМА ТУТ
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