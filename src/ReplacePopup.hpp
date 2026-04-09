#pragma once

#include "Includes.hpp"
#include "EditorUI.hpp"

#include <Geode/ui/Button.hpp>

class ProScrollLayer;

class ReplacePopup : public Popup {

private:

    std::vector<int> m_totalObjects;
    std::vector<int> m_objects;
    std::unordered_map<int, int> m_objectCounts;

    ProEditorUI* m_editorUI = nullptr;
    
    ScrollLayer* m_scroll = nullptr;

    Button* m_selectedObjBtn = nullptr;
    Button* m_replaceBtn = nullptr;

    Ref<Button> m_addBtn = nullptr;
    Ref<CCMenu> m_anyCell = nullptr;

    int m_selectedObject = 0;

    ReplacePopup(ProEditorUI*);

    bool init() override;

    void selectObject(int);
    void addObjectCell(int);

    void updateReplaceBtn();
    void updateStuff();

public:

    static ReplacePopup* create(ProEditorUI*);

};