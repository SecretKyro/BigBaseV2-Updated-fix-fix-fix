#include "common.hpp"
#include "fiber_pool.hpp"
#include "gta/player.hpp"
#include "gta_util.hpp"
#include "gui.hpp"
#include "logger.hpp"
#include "memory/module.hpp"
#include "memory/pattern.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script.hpp"

#include <imgui.h>
#include <StackWalker.h>
#include <imguipp.h>
#include "notify/imgui_notify.h"
#include "notify/font_awesome.cpp"

namespace big
{

	ImFont* font1;
	ImFont* font2;
	ImFont* font3;
	ImFont* font4;
	ImFont* versionfont;
	ImFont* bigfontawesome;
	bool Darkmode = true;
	float theme[3] = { 1.0f, 1.0f, 1.0f };
	float background[4] = { 18 / 255.0f, 18 / 255.0f, 18 / 255.0f, 255 / 255.0f };
	int MenuTab = 0;
	bool ShowMenu = false;

	bool demo = false;

	void GetDesktopResolution(int& horizontal, int& vertical)
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		horizontal = desktop.right;
		vertical = desktop.bottom;
	}

	void gui::dx_init()
	{
		ImGuiIO& io = ::ImGui::GetIO();

		io.IniFilename = NULL;

		ImFontConfig font_cfg;
		font_cfg.FontDataOwnedByAtlas = false;

		io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 22);
		ImGui::MergeIconsWithLatestFont(16.f, false);
		io.Fonts->AddFontDefault();
		io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 35.0f, &font_cfg);
		bigfontawesome = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 30.0f, &font_cfg);
		font1 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 30);
		font2 = io.Fonts->AddFontFromFileTTF("C:\\font\\d\\futur.ttf", 40);
		font3 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 40);
		font4 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 22.5);
		ImGui::MergeIconsWithLatestFont(15.f, false);
		versionfont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 17);

		ImGuiStyle* style = &ImGui::GetStyle();

		ImGuiStyle& Style = ImGui::GetStyle();
		auto Color = Style.Colors;

		Style.WindowBorderSize = 0;

		Style.TabRounding = 10;
		Style.ScrollbarRounding = 0;
		Style.ChildRounding = 6;
		Style.WindowRounding = 7;

		Color[ImGuiCol_WindowBg] = Darkmode == true ? ImColor(background[0], background[1], background[2], background[3]) : ImColor(218, 218, 218, 255);
		Color[ImGuiCol_Text] = Darkmode == true ? ImColor(255, 255, 255, 255) : ImColor(18, 18, 18, 255);
		Color[ImGuiCol_FrameBg] = Darkmode == true ? ImColor(31, 31, 31, 255) : ImColor(231, 231, 231, 255);
		Color[ImGuiCol_FrameBgActive] = Darkmode == true ? ImColor(41, 41, 41, 255) : ImColor(189, 196, 223, 255);
		Color[ImGuiCol_FrameBgHovered] = Darkmode == true ? ImColor(41, 41, 41, 255) : ImColor(189, 196, 223, 255);
		Color[ImGuiCol_Separator] = Darkmode == true ? ImColor(53, 176, 67, 255) : ImColor(53, 176, 67, 255);

		Color[ImGuiCol_Button] = Darkmode == true ? ImColor(26, 26, 26, 255) : ImColor(26, 26, 26, 255);
		Color[ImGuiCol_ButtonActive] = Darkmode == true ? ImColor(38, 38, 38, 255) : ImColor(38, 38, 38, 255);
		Color[ImGuiCol_ButtonHovered] = Darkmode == true ? ImColor(38, 38, 38, 255) : ImColor(38, 38, 38, 255);

		Color[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
		Color[ImGuiCol_Separator] = Darkmode == true ? ImColor(36, 36, 36, 255) : ImColor(36, 36, 36, 255);

		Color[ImGuiCol_ResizeGrip] = Darkmode == true ? ImColor(30, 30, 30, 255) : ImColor(30, 30, 30, 255);
		Color[ImGuiCol_ResizeGripActive] = Darkmode == true ? ImColor(30, 30, 30, 255) : ImColor(189, 196, 223, 255);
		Color[ImGuiCol_ResizeGripHovered] = Darkmode == true ? ImColor(30, 30, 30, 255) : ImColor(189, 196, 223, 255);
		Color[ImGuiCol_ChildBg] = Darkmode == true ? ImColor(0, 0, 0, 0) : ImColor(0, 0, 0, 0);

		Color[ImGuiCol_SliderGrab] = ImColor(theme[0], theme[1], theme[2]);
		Color[ImGuiCol_SliderGrabActive] = ImColor(theme[0], theme[1], theme[2]);

		Color[ImGuiCol_ScrollbarBg] = ImColor(24, 24, 24, 255);
		Color[ImGuiCol_ScrollbarGrab] = ImColor(24, 24, 24, 255);
		Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);
		Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);

		Color[ImGuiCol_Header] = ImColor(39, 39, 39, 255);
		Color[ImGuiCol_HeaderActive] = ImColor(39, 39, 39, 255);
		Color[ImGuiCol_HeaderHovered] = ImColor(39, 39, 39, 255);
		Color[ImGuiCol_CheckMark] = ImColor(theme[0], theme[1], theme[2]);
	}

	void gui::dx_on_tick()
	{
		static int tab;
		ImGui::SetNextWindowSize({ 875.f, 535.f });
		ImGui::Begin("BigBaseV2", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
		{
			ImGui::SetScrollX(0);
			ImGui::BeginChild("##LeftSide", ImVec2(200, ImGuiPP::GetY()), ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
			{
				auto make_item = [](const char* name, int newTab)
				{
					ImGui::NewLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
					ImGui::PushStyleColor(ImGuiCol_Text, MenuTab == newTab ? Darkmode == true ? IM_COL32(48, 48, 48, 255) : IM_COL32(0, 0, 0, 255), IM_COL32(200, 200, 200, 255) : IM_COL32(120, 120, 120, 255));
					if (MenuTab == newTab)
					{
						ImGui::SetCursorPosX(0);
						ImGui::TextColored(ImColor(theme[0], theme[1], theme[2]), " |");
						ImGui::SameLine(0, 190 / 5.6);
					}
					ImGui::Text(name);
					ImGui::PopStyleColor();
					if (ImGui::IsItemClicked())
						MenuTab = newTab;
				};
				ImGui::Spacing();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
				ImGui::PushFont(font4);
				ImGui::Text("BigBaseV2");
				ImGui::PopFont();
				ImGui::SetScrollX(0);
				ImGui::SetScrollY(0);
				ImGui::NewLine();

				//Main Tab Function
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5);
				make_item(ICON_FA_USER" Self", 0);
				make_item(ICON_FA_CROSSHAIRS" Weapons", 1);
				make_item(ICON_FA_CAR" Vehicle", 2);
				make_item(ICON_FA_SERVER" Players", 3);
				make_item(ICON_FA_GLOBE" World", 4);
				make_item(ICON_FA_SITEMAP" Miscellaneous", 5);
				make_item(ICON_FA_COGS" Settings", 6);
				ImGui::NewLine();
				ImGuiPP::Line(1);
			}
			ImGui::EndChild();

			ImGuiPP::Linevertical();

			ImGui::BeginChild("##RightSide", ImVec2(ImGuiPP::GetX(), ImGuiPP::GetY()));
			{

				ImGui::Spacing();
				ImGui::Spacing();
				ImGuiPP::Line(2);

				static std::vector<ULONG> colors = {
					ImGuiCol_Button, ImGuiCol_ButtonActive, ImGuiCol_ButtonHovered ,
					ImGuiCol_Header, ImGuiCol_HeaderActive, ImGuiCol_HeaderHovered ,
					ImGuiCol_FrameBg , ImGuiCol_FrameBgActive, ImGuiCol_FrameBgHovered, ImGuiCol_WindowBg
				};

				switch (MenuTab)
				{
				case 0:

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(71, 86, 200, 255));
					for (int i = 0; i < 10; i++)
						ImGui::PushStyleColor(colors[i], IM_COL32(189, 196, 223, 255));
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
					ImGui::PushItemWidth(350);
					ImGui::PopStyleVar();
					ImGui::PopStyleColor(11);
					ImGuiPP::Line(3);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
					ImGui::Checkbox("Demo Toggle", &demo);
					ImGui::PopItemWidth();
					break;

				case 1:
				{
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(71, 86, 200, 255));
					for (int i = 0; i < 10; i++)
						ImGui::PushStyleColor(colors[i], IM_COL32(189, 196, 223, 255));
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
					ImGui::PushItemWidth(350);
					ImGui::PopStyleVar();
					ImGui::PopStyleColor(11);
					ImGuiPP::Line(3);
					//Call Features Here
					ImGui::PopItemWidth();
					break;
				}

				case 2:
				{
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(71, 86, 200, 255));
					for (int i = 0; i < 10; i++)
						ImGui::PushStyleColor(colors[i], IM_COL32(189, 196, 223, 255));
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
					ImGui::PushItemWidth(350);
					ImGui::PopStyleVar();
					ImGui::PopStyleColor(11);
					ImGuiPP::Line(3);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
					//Call Features Here
					ImGui::PopItemWidth();
					break;
				}

				case 3:
				{
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(71, 86, 200, 255));
					for (int i = 0; i < 10; i++)
						ImGui::PushStyleColor(colors[i], IM_COL32(189, 196, 223, 255));
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
					ImGui::PushItemWidth(350);
					ImGui::PopStyleVar();
					ImGui::PopStyleColor(11);
					ImGuiPP::Line(3);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
					//Call Features Here
					ImGui::PopItemWidth();
					break;
				}

				case 4:
				{
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(71, 86, 200, 255));
					for (int i = 0; i < 10; i++)
						ImGui::PushStyleColor(colors[i], IM_COL32(189, 196, 223, 255));
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
					ImGui::PushItemWidth(350);
					ImGui::PopStyleVar();
					ImGui::PopStyleColor(11);
					ImGuiPP::Line(3);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
					//Call Features Here
					ImGui::PopItemWidth();
					break;
				}


				case 5:
				{
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(71, 86, 200, 255));
					for (int i = 0; i < 10; i++)
						ImGui::PushStyleColor(colors[i], IM_COL32(189, 196, 223, 255));
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
					ImGui::PushItemWidth(350);
					ImGui::PopStyleVar();
					ImGui::PopStyleColor(11);
					ImGuiPP::Line(3);
					ImGui::PopItemWidth();
					//Call Features Here
					break;
				}


				case 6:
				{
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(71, 86, 200, 255));
					for (int i = 0; i < 10; i++)
						ImGui::PushStyleColor(colors[i], IM_COL32(189, 196, 223, 255));
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
					ImGui::PushItemWidth(350);
					ImGui::PopStyleVar();
					ImGui::PopStyleColor(11);
					ImGuiPP::Line(3);
					//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 180);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
					ImGui::ColorEdit3("Color Theme", theme);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
					ImGui::ColorEdit4("Background Color", background);
					ImGui::PopItemWidth();
					break;
				}
				}
				ImGui::EndChild();
			}
			ImGui::End();
		}
	}

	void gui::script_init()
	{
	}

	void gui::script_on_tick()
	{
		if (g_gui.m_opened)
		{
			CONTROLS::DISABLE_ALL_CONTROL_ACTIONS(0);
		}
	}

	void gui::script_func()
	{
		g_gui.script_init();
		while (true)
		{
			g_gui.script_on_tick();
			script::get_current()->yield();
		}
	}

	
}


