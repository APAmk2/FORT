// main.cpp : Defines the entry point for the application.
//
#include "FORT.h"
#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "imgui_stdlib.h"
#include "Utils/BaseToolWindow.h"

#include "Fonline/Fo2D_win.h"
#include "Fonline/fotxtb_win.h"
#include "FT/FTSprite_win.h"

#include <filesystem>
#include <vector>
#include <iostream>
#include <string>

#define PROGRAM_LABEL "FORT:A FallOut Resources Tools"

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
ImGuiIO* io;
SDL_GLContext gl_context;
vector<BaseToolWindow*> windows;

int initProgram()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    #ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow(PROGRAM_LABEL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    static Fo2DWindow Fo2DTool(false);
    windows.push_back((BaseToolWindow*) &Fo2DTool);

    static FotxtbWindow FotxtbTool(false);
    windows.push_back((BaseToolWindow*)&FotxtbTool);

    static FTSpriteWindow FTSTool(false);
    windows.push_back((BaseToolWindow*) &FTSTool);

    for (size_t i = 0, len = windows.size(); i < len; i++)
    {
        windows[i]->renderer = renderer;
        windows[i]->initWindow();
    }

    return 0;
}

void destroyProgram()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawWindow()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::BeginMenu("New Fonline"))
            {
                if (ImGui::MenuItem("Fonline2D")) { windows[0]->setVisible(!windows[0]->getVisible()); }
                if (ImGui::MenuItem("Fonline txtb")) { windows[1]->setVisible(!windows[1]->getVisible()); }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Fallout:Tactics"))
            {
                if (ImGui::MenuItem("FTSprite")) { windows[2]->setVisible(!windows[2]->getVisible()); }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Fallout 1/2"))
            {
                if (ImGui::MenuItem("Placeholder")) { }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Exit")) { progSettings.callExit = true; }
        ImGui::EndMainMenuBar();
    }
}

void mainLoop()
{
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!progSettings.callExit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                progSettings.callExit = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                progSettings.callExit = true;
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        drawWindow();

        for (size_t i = 0, len = windows.size(); i < len; i++)
        {
            windows[i]->drawWindow();
        }

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
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
