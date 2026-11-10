#include "EditorUI.hpp"
#include "ReplacePopup.hpp"

void ProEditorUI::replace(const std::vector<int>& toReplace, int replaceObj) {
    auto array = CCArray::create();

    if (
        m_selectedObject 
        && m_selectedObject->m_objectID != replaceObj
        && (std::ranges::contains(toReplace, m_selectedObject->m_objectID) || toReplace.empty())) {
        array->addObject(m_selectedObject);
    }

    if (m_selectedObjects) {
        for (auto object : CCArrayExt<GameObject*>(m_selectedObjects)) {
            if (
                object->m_objectID != replaceObj
                && (std::ranges::contains(toReplace, object->m_objectID) || toReplace.empty())
            ) {
                array->addObject(object);
            }
        }
    }

    if (array->count() <= 0) {
        if (Mod::get()->getSettingValue<bool>("show-notification")) {
            Notification::create("Replaced 0 objects.")->show();
        }

        return;
    }

    if (isCustomObject(replaceObj)) {
        auto positions = std::vector<CCPoint>{};
        auto seen = std::unordered_set<GameObject*>{};

        for (auto object : CCArrayExt<GameObject*>(array)) {
            if (seen.insert(object).second) {
                positions.push_back(object->getPosition());
            }
        }

        if (Mod::get()->getSettingValue<bool>("delete-replaced")) {
            this->deselectAll();
            this->selectObjects(array, false);
            this->onDeleteSelected(nullptr);
        }

        auto unlimited = m_editorLayer->m_level->m_unlimitedObjectsEnabled;
        m_editorLayer->m_keepEditorLayer = true;
        m_editorLayer->m_level->m_unlimitedObjectsEnabled = true;

        for (auto position : positions) {
            m_clickAtPosition = position;
            this->onCreateObject(replaceObj);
        }

        m_editorLayer->m_keepEditorLayer = false;
        m_editorLayer->m_level->m_unlimitedObjectsEnabled = unlimited;

        this->updateButtons();
        this->updateObjectInfoLabel();

        if (!Mod::get()->getSettingValue<bool>("select-placed")) {
            this->deselectAll();
        }

        if (Mod::get()->getSettingValue<bool>("show-notification")) {
            Notification::create(
                fmt::format("Replaced {} object{}", positions.size(), positions.size() > 1 ? "s" : ""),
                NotificationIcon::Success
            )->show();
        }

        return;
    }

    std::string str = this->copyObjects(array, true, true);

    auto vec = std::vector<std::string>{};

    for (auto subStr : string::split(str, ";")) {
        auto newVec = string::split(subStr, ",");

        if (static_cast<int>(newVec.size()) >= 1) {
            newVec[1] = numToString(replaceObj);
        }

        vec.push_back(string::join(newVec, ","));
    }

    str = string::join(vec, ";");

    if (Mod::get()->getSettingValue<bool>("delete-replaced")) {
        this->deselectAll();
        this->selectObjects(array, false);
        this->onDeleteSelected(nullptr);
    }

    auto og = m_editorLayer->m_level->m_unlimitedObjectsEnabled;

    m_editorLayer->m_keepEditorLayer = true;
    m_editorLayer->m_level->m_unlimitedObjectsEnabled = true;

    auto pasted = this->pasteObjects(str, false, false);

    m_editorLayer->m_keepEditorLayer = false;
    m_editorLayer->m_level->m_unlimitedObjectsEnabled = og;
    
    this->updateButtons();
    this->updateObjectInfoLabel();

    if (!Mod::get()->getSettingValue<bool>("select-placed")) {
        this->deselectAll();
    }

    auto replaced = pasted->count();

    if (Mod::get()->getSettingValue<bool>("show-notification")) {
        Notification::create(
            fmt::format("Replaced {} object{}", replaced, replaced > 1 ? "s" : ""),
            NotificationIcon::Success
        )->show();
    }
}

void ProEditorUI::updateButton() {
    if (auto button = m_fields->button) {
        auto enabled = m_selectedObject || (m_selectedObjects && m_selectedObjects->count() > 0);
        button->setOpacity(enabled ? 255 : 175);
        button->setEnabled(enabled);
    }
}

void ProEditorUI::updateButtons() {
    EditorUI::updateButtons();
    updateButton();
}

bool ProEditorUI::init(LevelEditorLayer* p0) {
    if (!EditorUI::init(p0)) {
        return false;
    }

    auto menu = this->getChildByID("editor-buttons-menu");

    if (!menu) {
        return true;
    }

    auto btn = Button::createWithSprite("replaceselected.png"_spr, [this](Button*) {
        ReplacePopup::create(this)->show();
    });
    btn->setContentSize({40, 40});
    btn->setID("button"_spr);

    menu->addChild(btn);
    menu->updateLayout();

    m_fields->button = btn;
    m_uiItems->addObject(btn);

    this->updateButton();

    auto i = 0;
    auto f = m_fields.self();

    for (auto bar : CCArrayExt<EditButtonBar*>(m_createButtonBars)) {
        for (auto item : CCArrayExt<CreateMenuItem*>(bar->m_buttonArray)) {
            if (item->m_objectID != 0 && !std::ranges::contains(f->tabObjects[i], item->m_objectID)) {
                f->tabObjects[i].push_back(item->m_objectID);
            }
        }

        i++;
    }

    for (auto key : CCArrayExt<CCString*>(GameManager::sharedState()->getOrderedCustomObjectKeys())) {
        f->tabObjects[13].push_back(key->intValue());
    }

    return true;
}
