#include "options.h"
#include "FORT.h"

#define TOML_EXCEPTIONS 0
#include "toml.hpp"
#include <filesystem>
#include <fstream>

using namespace std::string_view_literals;

void SaveConfig()
{
    std::ofstream out;
    out.open("config.toml");

    if (out.is_open())
    {
        toml::table config = toml::table{
        { "paths", toml::table{
#ifdef NEW_FONLINE
            { "NewFonline", progSettings.newFonlinePath },
#endif // NEW_FONLINE
#ifdef FALLOUT
            { "Fallout", progSettings.falloutPath },
#endif // FALLOUT
#ifdef FALLOUT_TACTICS
            { "FalloutTactics", progSettings.falloutTacticsPath },
#endif // FALLOUT_TACTICS
            }}
        };
        out << config;
    }

    out.close();
}

void OptionsWindow::DrawWin()
{
	if (!GetVisible()) return;
	ImGui::Begin("Options", &Visible);

#ifdef NEW_FONLINE
	ImGui::InputText("New FOnline Path", &progSettings.newFonlinePath);
#endif // NEW_FONLINE

#ifdef FALLOUT
	ImGui::InputText("Fallout 1/2 Path", &progSettings.falloutPath);
#endif // FALLOUT

#ifdef FALLOUT_TACTICS
	ImGui::InputText("Fallout:Tactics Path", &progSettings.falloutTacticsPath);
#endif // FALLOUT_TACTICS

    if (ImGui::Button("Save Settings"))
    {
        SaveConfig();
    }

	ImGui::End();
}

void OptionsWindow::InitWin()
{
	ImGui::DebugLog("Initializing Options Window...\n");

    toml::parse_result result = toml::parse_file("config.toml");
    if (!result)
    {
        ImGui::DebugLog("Parsing failed:%s\n", result.error());
    }
    else 
    {
        toml::table config = result.table();
#ifdef NEW_FONLINE
        progSettings.newFonlinePath = config["paths"]["NewFonline"].value_or(""sv);
#endif // NEW_FONLINE
#ifdef FALLOUT
        progSettings.falloutPath = config["paths"]["Fallout"].value_or(""sv);
#endif // FALLOUT
#ifdef FALLOUT_TACTICS
        progSettings.falloutTacticsPath = config["paths"]["FalloutTactics"].value_or(""sv);
#endif // FALLOUT_TACTICS
    }

	ImGui::DebugLog("Options Window Init Done.\n");
}

void OptionsWindow::ProcessMenuBtn()
{
	if (ImGui::MenuItem("Options")) { this->SetVisible(!this->GetVisible()); }
}