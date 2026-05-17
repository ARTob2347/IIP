#include "../declaration/code_generator.h"
#include <sstream>
#include <string>

// ���������� ������ ��� ������� � C++ ���
static std::string esc(const std::string& s) {
    std::string r;
    for (char c : s) {
        if (c == '"')  r += "\\\"";
        else if (c == '\\') r += "\\\\";
        else r += c;
    }
    return r;
}

std::string generate_cpp(const SimpleForm& form) {
    std::ostringstream o;

    // --- ��������� ---
    o << "#include <windows.h>\n";
    o << "#include <string>\n\n";

    // --- ��������� ID ��� ������� ���������� ---
    int id = 1001;
    for (const auto& c : form.a) {
        o << "#define ID_" << c.name << " " << id++ << "\n";
    }
    o << "\n";

    // --- WndProc ---
    o << "LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {\n";
    o << "    switch (msg) {\n";

    // OnClick ��� ������
    bool has_buttons = false;
    for (const auto& c : form.a) {
        if (c.type == "button") {
            if (!has_buttons) {
                o << "    case WM_COMMAND:\n";
                o << "        switch (LOWORD(wp)) {\n";
                has_buttons = true;
            }
            o << "        case ID_" << c.name << ":\n";
            o << "            MessageBox(hwnd, \"" << esc(c.caption) << " clicked\", \"Event\", MB_OK);\n";
            o << "            break;\n";
        }
    }
    if (has_buttons) {
        o << "        }\n";
        o << "        break;\n";
    }

    o << "    case WM_DESTROY:\n";
    o << "        PostQuitMessage(0);\n";
    o << "        break;\n";
    o << "    default:\n";
    o << "        return DefWindowProc(hwnd, msg, wp, lp);\n";
    o << "    }\n";
    o << "    return 0;\n";
    o << "}\n\n";

    // --- WinMain ---
    o << "int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {\n";
    o << "    WNDCLASS wc = {};\n";
    o << "    wc.lpfnWndProc   = WndProc;\n";
    o << "    wc.hInstance     = hInst;\n";
    o << "    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);\n";
    o << "    wc.lpszClassName = \"FormClass\";\n";
    o << "    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);\n";
    o << "    RegisterClass(&wc);\n\n";

    // ������ ������� ����
    o << "    HWND hwnd = CreateWindowA(\n";
    o << "        \"FormClass\", \"" << esc(form.name) << "\",\n";
    o << "        WS_OVERLAPPEDWINDOW,\n";
    o << "        CW_USEDEFAULT, CW_USEDEFAULT,\n";
    o << "        " << form.width << ", " << form.height << ",\n";
    o << "        NULL, NULL, hInst, NULL\n";
    o << "    );\n\n";

    // ������ ����������
    for (const auto& c : form.a) {
        if (c.type == "button") {
            o << "    CreateWindowA(\"BUTTON\", \"" << esc(c.caption) << "\",\n";
            o << "        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,\n";
            o << "        " << c.x << ", " << c.y << ", "
                << c.width << ", " << c.height << ",\n";
            o << "        hwnd, (HMENU)ID_" << c.name << ", hInst, NULL);\n\n";
        }
        else if (c.type == "label") {
            o << "    CreateWindowA(\"STATIC\", \"" << esc(c.caption) << "\",\n";
            o << "        WS_CHILD | WS_VISIBLE,\n";
            o << "        " << c.x << ", " << c.y << ", "
                << c.width << ", " << c.height << ",\n";
            o << "        hwnd, NULL, hInst, NULL);\n\n";
        }
        else if (c.type == "edit") {
            o << "    CreateWindowA(\"EDIT\", \"" << esc(c.caption) << "\",\n";
            o << "        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,\n";
            o << "        " << c.x << ", " << c.y << ", "
                << c.width << ", " << c.height << ",\n";
            o << "        hwnd, NULL, hInst, NULL);\n\n";
        }
        else if (c.type == "checkbox") {
            o << "    CreateWindowA(\"BUTTON\", \"" << esc(c.caption) << "\",\n";
            o << "        WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_AUTOCHECKBOX,\n";
            o << "        " << c.x << ", " << c.y << ", "
                << c.width << ", " << c.height << ",\n";
            o << "        hwnd, (HMENU)ID_" << c.name << ", hInst, NULL);\n\n";
        }
    }

    // ���������� ���� � ��������� message loop
    o << "    ShowWindow(hwnd, nCmdShow);\n";
    o << "    UpdateWindow(hwnd);\n\n";
    o << "    MSG msg = {};\n";
    o << "    while (GetMessage(&msg, NULL, 0, 0)) {\n";
    o << "        TranslateMessage(&msg);\n";
    o << "        DispatchMessage(&msg);\n";
    o << "    }\n";
    o << "    return 0;\n";
    o << "}\n";

    return o.str();
}
