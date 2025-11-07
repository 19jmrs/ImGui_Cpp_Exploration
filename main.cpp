#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

static std::string filePath;
static std::vector<std::vector<std::string>> csvData;

// Opens Windows file picker
std::string OpenFileDialog(const char* filter = "CSV Files\0*.csv\0All Files\0*.*\0")
{
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn) == TRUE)
        return ofn.lpstrFile;

    return "";
}

// Load CSV into a 2D vector
void LoadCSV(const std::string& path)
{
    csvData.clear();
    std::ifstream file(path);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ','))
            row.push_back(cell);

        csvData.push_back(row);
    }
    file.close();
}

void ShowCSVWindow()
{
    if (ImGui::Button("Open CSV File"))
    {
        filePath = OpenFileDialog();
        if (!filePath.empty())
            LoadCSV(filePath);
    }

    if (!filePath.empty())
        ImGui::Text("Loaded: %s", filePath.c_str());

    if (!csvData.empty())
    {
        ImGui::Separator();
        ImGui::BeginChild("CSVTable", ImVec2(0, 300), true);

        for (const auto& row : csvData)
        {
            for (const auto& cell : row)
            {
                ImGui::Text("%s", cell.c_str());
                ImGui::SameLine();
            }
            ImGui::NewLine();
        }

        ImGui::EndChild();
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) return 1;

    // Create window with OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Initialize ImGui for GLFW and OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    char inputText[128] = "";
    char outputText[128] = "";
    static bool showText = false;
    static float alpha = 0.0f;
    static bool showSecondaryWindow = false;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a window
        ImGui::Begin("FullScreen", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus
            );  

        // Now this "window" covers the entire app
        ImGui::InputText("Input", inputText, IM_ARRAYSIZE(inputText));


        if (ImGui::Button(showText ? "Hide" : "Show")){
            showText = !showText;
            if(showText)
            strcpy(outputText, inputText);
        }

        // handle button state change to hide text
        float target = showText ? 1.0f : 0.0f;
        alpha += (target - alpha) * 0.1f;

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        if (alpha > 0.01f)
            ImGui::Text("Output: %s", outputText);
        ImGui::PopStyleVar();
        
       
        if(ImGui::Button("Open New Window")){
            showSecondaryWindow = true;
        }
        if(showSecondaryWindow){
            ImGui::Begin("Second Window", &showSecondaryWindow);
            ImGui::Text("This is a new window!");
            if(ImGui::Button("Close"))
                showSecondaryWindow = false;
            ImGui::End();
        }

        ShowCSVWindow();
        
        
        ImGui::End();

        // Render everything
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
