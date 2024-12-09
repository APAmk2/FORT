﻿// main.cpp : Defines the entry point for the application.
//
#include <filesystem>
#include <vector>
#include <string>

#include "FORT.h"
#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "imgui_stdlib.h"
#include "Utils/BaseToolWindow.h"

#ifdef NEW_FONLINE
    #include "Fonline/Fo2D_win.h"
    #include "Fonline/FoTxtb_win.h"
#endif // NEW_FONLINE

#ifdef FALLOUT
    #include "Fallout/FallMap_win.h"
    #include "Fallout/FallProto_win.h"
    #include "Fallout/FallFRM_win.h"
    #include "Fallout/FallRIX_win.h"
    #include "Fallout/FallMSK_win.h"
#endif // FALLOUT

#ifdef FALLOUT_TACTICS
    #include "FT/FTZar_win.h"
    #include "FT/FTTil_win.h"
#endif // FALLOUT_TACTICS

#define PROGRAM_LABEL "FORT:A FallOut Resources Tools"

static SDL_Window* MainWin;
static SDL_Renderer* Renderer;
static ImGuiIO* Io;
static SDL_GLContext GlContext;
static std::vector<BaseToolWindow*> Windows;

int Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    #ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    SDL_WindowFlags winFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    MainWin = SDL_CreateWindow(PROGRAM_LABEL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, winFlags);
    if (MainWin == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    Renderer = SDL_CreateRenderer(MainWin, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (Renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    Io = &ImGui::GetIO();
    Io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(MainWin, Renderer);
    ImGui_ImplSDLRenderer2_Init(Renderer);

#ifdef NEW_FONLINE
    static Fo2DWindow Fo2DTool(false);
    Windows.push_back((BaseToolWindow*)&Fo2DTool);

    static FoTxtbWindow FoTxtbTool(false);
    Windows.push_back((BaseToolWindow*)&FoTxtbTool);
#endif // NEW_FONLINE

#ifdef FALLOUT
    static FallProtoWindow FallProtoTool(false);
    Windows.push_back((BaseToolWindow*)&FallProtoTool);

    static FallMapWindow FallMapTool(false);
    Windows.push_back((BaseToolWindow*)&FallMapTool);

    static FallFRMWindow FallFRMTool(false);
    Windows.push_back((BaseToolWindow*)&FallFRMTool);

    static FallRIXWindow FallRIXTool(false);
    Windows.push_back((BaseToolWindow*)&FallRIXTool);

    static FallMSKWindow FallMSKTool(false);
    Windows.push_back((BaseToolWindow*)&FallMSKTool);
#endif // FALLOUT

#ifdef FALLOUT_TACTICS
    static FTZarWindow FTZarTool(false);
    Windows.push_back((BaseToolWindow*)&FTZarTool);

    static FTTilWindow FTTilTool(false);
    Windows.push_back((BaseToolWindow*)&FTTilTool);
#endif // FALLOUT_TACTICS

    for (size_t i = 0, len = Windows.size(); i < len; i++)
    {
        Windows[i]->Renderer = Renderer;
        Windows[i]->InitWin();
    }

    return 0;
}

void Destroy()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(MainWin);
    SDL_Quit();
}

void DrawMainWin()
{
    if (ImGui::BeginMainMenuBar())
    {
        for (size_t i = 0, len = Windows.size(); i < len; i++)
        {
            Windows[i]->ProcessMenuBtn();
        }

        if (ImGui::MenuItem("Exit")) { progSettings.callExit = true; }
        ImGui::EndMainMenuBar();
    }
}

void MainLoop()
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
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(MainWin))
                progSettings.callExit = true;
        }
        if (SDL_GetWindowFlags(MainWin) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        DrawMainWin();

        for (size_t i = 0, len = Windows.size(); i < len; i++)
        {
            Windows[i]->DrawWin();
        }

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(Renderer, Io->DisplayFramebufferScale.x, Io->DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(Renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(Renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), Renderer);
        SDL_RenderPresent(Renderer);
    }

    Destroy();
}

int main(int argc, char* argv[])
{
    if (Init() == -1)
    {
        return -1;
    }
    MainLoop();
	return 0;
}
