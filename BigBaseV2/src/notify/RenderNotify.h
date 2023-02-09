#pragma once
#include "../includes.h"
#include "./fa_solid_900.h"
#include "./font_awesome_5.h"
#include "./imgui_notify.h"
#include "./tahoma.h"

namespace RedlineNotify
{
	void InitNotify(float color1, float color2, float color3, float color4)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 7.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(color1, color2, color3, color4));
		ImGui::RenderNotifications();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);
	}
}