// main.cpp : Defines the entry point for the application.
//
#include "FORT.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"

#include "Fonline/foart.h"
#include <filesystem>
#include <string>
#include <vector>

using namespace std;

SDL_Window* window;
ImGuiIO* io;
SDL_GLContext gl_context;
GLuint foart_tex;
static int Fo2Dwidth = 0, Fo2Dheight = 0;
static string Fo2Dfilename = "test.png";

int initProgram()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    #ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("FORT:A FallOut Resources Tools", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    glGenTextures(1, &foart_tex);
    glBindTexture(GL_TEXTURE_2D, foart_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return 0;
}

void destroyProgram()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void updateFonline2D(string filename)
{
    filesystem::path filepath = filename;
    if (!decodeFonline2D(filepath, Fo2Dwidth, Fo2Dheight))
    {
        decodeOldFonline2D(filepath, Fo2Dwidth, Fo2Dheight);
    }
}

void drawWindow(ImVec4& clear_color)
{
    ImGui::Begin("Fonline 2D Format");
    ImGui::Text("Width:%i", Fo2Dwidth);
    ImGui::Text("Height:%i", Fo2Dheight);
    ImGui::Text("Text:%s", Fo2Dfilename.c_str());
    ImGui::InputText("FOnline 2D Graphics file path", &Fo2Dfilename);
    if (ImGui::Button("Load File"))
    {
        updateFonline2D(Fo2Dfilename);
    }
    ImGui::Image((void*)(intptr_t)foart_tex, ImVec2(Fo2Dwidth, Fo2Dheight));
    ImGui::End();
}

void mainLoop()
{
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        drawWindow(clear_color);

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    destroyProgram();
}

int main(int argc, char* argv[])
{
    if (initProgram() == -1)
    {
        return -1;
    }
    mainLoop();
	return 0;
}
