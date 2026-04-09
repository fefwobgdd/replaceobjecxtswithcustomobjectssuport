#pragma once

#include "Includes.hpp"

#include <Geode/modify/EditorUI.hpp>

#include <Geode/ui/Button.hpp>

class $modify(ProEditorUI, EditorUI) {

    struct Fields {
        Button* button = nullptr;
        std::unordered_map<int, std::vector<int>> tabObjects;
    };

    void replace(const std::vector<int>&, int);
    void updateButton();

    $override
    void updateButtons();
    bool init(LevelEditorLayer*);

};