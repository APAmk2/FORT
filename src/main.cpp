// main.cpp : Defines the entry point for the application.
//
#include <filesystem>
#include <vector>
#include "FORT.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

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

#include "options.h"

#define PROGRAM_LABEL "FORT:A FallOut Resources Tools"

static SDL_Window* MainWin;
SDL_Renderer* Renderer;
static ImGuiIO* Io;
//static SDL_GLContext GlContext;
static std::vector<BaseWindow*> Windows;
ProgramSettings progSettings;

void ApplyImGUIStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.14f, 0.16f, 0.11f, 0.52f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.30f, 0.23f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.34f, 0.26f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.81f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.32f, 0.24f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.30f, 0.22f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.23f, 0.27f, 0.21f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.29f, 0.34f, 0.26f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
    colors[ImGuiCol_Header] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
    colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.11f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.19f, 0.23f, 0.18f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.54f, 0.57f, 0.51f, 0.78f);
    colors[ImGuiCol_TabActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.78f, 0.28f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.73f, 0.67f, 0.24f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);

    style.FrameBorderSize = 1.0f;
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
}

int Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        ImGui::DebugLog("Error: %s\n", SDL_GetError());
        return -1;
    }

    #ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    SDL_WindowFlags winFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    MainWin = SDL_CreateWindow(PROGRAM_LABEL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, winFlags);
    if (MainWin == nullptr)
    {
        ImGui::DebugLog("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
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
    Io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ApplyImGUIStyle();
    ImGui_ImplSDL2_InitForSDLRenderer(MainWin, Renderer);
    ImGui_ImplSDLRenderer2_Init(Renderer);

#ifdef NEW_FONLINE
    Fo2DWindow* Fo2DTool = new Fo2DWindow(false);
    Windows.push_back((BaseWindow*)Fo2DTool);

    FoTxtbWindow* FoTxtbTool = new FoTxtbWindow(false);
    Windows.push_back((BaseWindow*)FoTxtbTool);
#endif // NEW_FONLINE

#ifdef FALLOUT
    FallProtoWindow* FallProtoTool = new FallProtoWindow(false);
    Windows.push_back((BaseWindow*)FallProtoTool);

    FallMapWindow* FallMapTool = new FallMapWindow(false);
    Windows.push_back((BaseWindow*)FallMapTool);

    FallFRMWindow* FallFRMTool = new FallFRMWindow(false);
    Windows.push_back((BaseWindow*)FallFRMTool);

    FallRIXWindow* FallRIXTool = new FallRIXWindow(false);
    Windows.push_back((BaseWindow*)FallRIXTool);

    FallMSKWindow* FallMSKTool = new FallMSKWindow(false);
    Windows.push_back((BaseWindow*)FallMSKTool);
#endif // FALLOUT

#ifdef FALLOUT_TACTICS
    FTZarWindow* FTZarTool = new FTZarWindow(false);
    Windows.push_back((BaseWindow*)FTZarTool);

    FTTilWindow* FTTilTool = new FTTilWindow(false);
    Windows.push_back((BaseWindow*)FTTilTool);

#endif // FALLOUT_TACTICS

    OptionsWindow* Options = new OptionsWindow(false);
    Windows.push_back((BaseWindow*)Options);

    for (size_t i = 0, len = Windows.size(); i < len; i++)
    {
        Windows[i]->InitWin();
    }

    return 0;
}

void Destroy()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    for (size_t i = 0, len = Windows.size(); i < len; i++)
    {
        Windows[i]->DestroyWin();
        delete Windows[i];
    }
    
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(MainWin);
    SDL_Quit();
}

void DrawMainWin()
{
    ImGui::ShowDebugLogWindow();
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
        SDL_SetRenderDrawColor(Renderer, 0x4A, 0x56, 0x42, 0xFF);
        SDL_RenderClear(Renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), Renderer);
        SDL_RenderPresent(Renderer);
    }

    ImGui::DebugLog("progSettings.callExit == true...\n");

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
