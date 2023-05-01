//
//  CustomWidgets.cpp
//  ButOSX
//
//  Created by Can on 31.10.2020.
//  Copyright © 2020 Lyceion. All rights reserved.
//

#include "CustomWidgets.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "xorstr.h"

bool CustomWidgets::SubTab(const char* label, const ImVec2& size_arg, const bool selected)
{
    ImGuiButtonFlags flags = 0;
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    
    
    ImVec2 pos = window->DC.CursorPos;

    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;
    if (g.CurrentItemFlags & ImGuiItemFlags_ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
    if (selected){
        window->DrawList->AddRectFilled({ bb.Min.x,bb.Min.y }, { bb.Max.x,bb.Max.y }, ImColor(0.23f, 0.23f, 0.23f, style.Alpha));
        window->DrawList->AddRectFilled(ImVec2(bb.Min.x, bb.Max.y - 4), ImVec2(bb.Max.x, bb.Max.y), ImColor(0.99f, 0.43f, 0.f, style.Alpha));
    }
    else{
        window->DrawList->AddRectFilled({ bb.Min.x,bb.Min.y }, { bb.Max.x,bb.Max.y }, ImColor(0.11f, 0.11f, 0.11f, style.Alpha));
    }
    //window->DrawList->AddText(ImVec2(((pos.x + size.x) - label_size.x) / 2, ((pos.y + size.y) - label_size.y) / 2), ImColor(255, 255, 255), label);
    //window->DrawList->AddText(ImVec2(bb.Min.x + 5, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(label).y / 2), ImColor(255,255,255, 255), label);
    return pressed;
}

bool CustomWidgets::Switch(const char* label, bool* v)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float height = 20.00f;
    float width = height * 1.45f;
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    float radius = height * 0.50f;
    float radius2 = height * 0.70f;

    const float square_sz = ImGui::GetFrameHeight();
    ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + square_sz + (label_size.x > 5.0f ? style.ItemInnerSpacing.x + label_size.x : 5.0f), pos.y + label_size.y + style.FramePadding.y * 3.0f)); 
    ImGui::InvisibleButton(label, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *v = !*v;
    const ImRect check_bb(ImVec2(pos.x, pos.y), ImVec2(pos.x + square_sz, pos.y + square_sz));
    float t = *v ? 1.0f : 0.0f;
    ImGui::SameLine();
    ImGui::SameLine();
    ImGui::RenderText(ImVec2(18 + check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y - 5), label);
    float ANIM_SPEED = 0.25f;
    if (g.LastActiveId == g.CurrentWindow->GetID(label))
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = *v ? (t_anim) : (1.0f - t_anim);
    }

    ImU32 col_bg, top_bg2;
    if (ImGui::IsItemHovered()) {
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.17f, 0.21f, 0.24f, 1.00f), ImVec4(0.15f, 0.82f, 0.35f, 1.00f), t));
        top_bg2 = IM_COL32(103, 23, 244, 50);

    }
    else
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.17f, 0.21f, 0.24f, 1.00f), ImVec4(0.15f, 0.82f, 0.35f, 1.00f), t));
    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + 13.f), col_bg, height * 0.5f); // top arkası
    if (ImGui::IsItemClicked())
        draw_list->AddCircleFilled(ImVec2(p.x + radius - 2.f + t * (width + 5.f - radius * 2.0f), p.y + radius / 1.5), radius - 1.5f, ImColor(1.0f, 1.0f, 1.0f, style.Alpha), 30); //top
    else
        draw_list->AddCircleFilled(ImVec2(p.x + radius - 2.f + t * (width + 5.f - radius * 2.0f), p.y + radius / 1.5), radius - 1.5f, ImColor(1.0f, 1.0f, 1.0f, style.Alpha), 30); //top

    if (ImGui::IsItemHovered())
        draw_list->AddCircleFilled(ImVec2(p.x + radius2 - 6.f + t * (width + 13.f - radius2 * 2.0f), p.y + radius2 / 2), radius2 - 0.5f, IM_COL32(255, 255, 255, 50), 24); // saydam top

    if (window->SkipItems)
        return false;
    
    ImGui::ItemSize(total_bb, style.FramePadding.y);


    bool hovered, held;
    bool isChanged = false;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        isChanged = true;
        *v = !(*v);
        ImGui::MarkItemEdited(id);
    }

    return isChanged;
}

bool CustomWidgets::SwitchTouchbar(const char* label, TBWMenuItem* _btn)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float height = 20.00f;
    float width = height * 1.45f;
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    float radius = height * 0.50f;
    float radius2 = height * 0.70f;

    const float square_sz = ImGui::GetFrameHeight();
    ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + square_sz + (label_size.x > 5.0f ? style.ItemInnerSpacing.x + label_size.x : 5.0f), pos.y + label_size.y + style.FramePadding.y * 3.0f));
    ImGui::InvisibleButton(label, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        _btn->state = !_btn->state;
    const ImRect check_bb(ImVec2(pos.x, pos.y), ImVec2(pos.x + square_sz, pos.y + square_sz));
    float t = _btn->state ? 1.0f : 0.0f;
    ImGui::SameLine();
    ImGui::SameLine();
    ImGui::RenderText(ImVec2(18 + check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y - 5), label);
    float ANIM_SPEED = 0.25f;
    if (g.LastActiveId == g.CurrentWindow->GetID(label))
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = _btn->state ? (t_anim) : (1.0f - t_anim);
    }

    ImU32 col_bg, top_bg2;
    if (ImGui::IsItemHovered()) {
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.17f, 0.21f, 0.24f, 1.00f), ImVec4(0.15f, 0.82f, 0.35f, 1.00f), t));
        top_bg2 = IM_COL32(103, 23, 244, 50);

    }
    else
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.17f, 0.21f, 0.24f, 1.00f), ImVec4(0.15f, 0.82f, 0.35f, 1.00f), t));
    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + 13.f), col_bg, height * 0.5f); // top arkası
    if (ImGui::IsItemClicked())
        draw_list->AddCircleFilled(ImVec2(p.x + radius - 2.f + t * (width + 5.f - radius * 2.0f), p.y + radius / 1.5), radius - 1.5f, ImColor(1.0f, 1.0f, 1.0f, style.Alpha), 30); //top
    else
        draw_list->AddCircleFilled(ImVec2(p.x + radius - 2.f + t * (width + 5.f - radius * 2.0f), p.y + radius / 1.5), radius - 1.5f, ImColor(1.0f, 1.0f, 1.0f, style.Alpha), 30); //top

    if (ImGui::IsItemHovered())
        draw_list->AddCircleFilled(ImVec2(p.x + radius2 - 6.f + t * (width + 13.f - radius2 * 2.0f), p.y + radius2 / 2), radius2 - 0.5f, IM_COL32(255, 255, 255, 50), 24); // saydam top

    if (window->SkipItems)
        return false;
    
    ImGui::ItemSize(total_bb, style.FramePadding.y);


    bool hovered, held;
    bool isChanged = false;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        isChanged = true;
        _btn->state = !(_btn->state);
        ImGui::MarkItemEdited(id);
    }
    UpdateButton(_btn);
    return isChanged;
}

bool CustomWidgets::ControlBox(bool* HideShowBool){
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
    
    //Close Button
    style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.36f, 0.33f, style.Alpha);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.56f, 0.53f, style.Alpha);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.26f, 0.23f, style.Alpha);
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 0.00f);
    ImGui::SetCursorPos(ImVec2(10, 9));
    if (ImGui::Button(xorstr("X"), ImVec2(10, 10)))
        exit(69); //Nice.
    
    //Minimize Button
    style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.76f, 0.20f, style.Alpha);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.96f, 0.40f, style.Alpha);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.66f, 0.10f, style.Alpha);
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 0.00f);
    ImGui::SetCursorPos(ImVec2(25, 9));
    if (ImGui::Button(xorstr("-"), ImVec2(10, 10)))
        *HideShowBool = false;
    ImGui::PopStyleVar();
    return true;
}

bool CustomWidgets::ColorPicker4(const char* label, TBWMenuItem* color, int flags)
{
    float col4[4] = { (float)color->color.redComponent, (float)color->color.greenComponent, (float)color->color.blueComponent, (float)color->color.alphaComponent };
    if (!ImGui::ColorPicker4(label, col4, flags))
        return false;
    color->color = [NSColor colorWithCalibratedRed:(CGFloat)(col4[0]) green:(CGFloat)(col4[1]) blue:(CGFloat)(col4[2]) alpha:(CGFloat)col4[3]];
    [color->colReferance setColor:color->color];
    return true;
}
