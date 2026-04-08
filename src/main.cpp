#include "Includes.hpp"
#include "ReplacePopup.hpp"

#include <Geode/modify/EditorUI.hpp>

#include <Geode/ui/Button.hpp>

class $modify(EditorUI) {

    bool init(LevelEditorLayer* p0) {
        if (!EditorUI::init(p0)) {
            return false;
        }

        auto menu = getChildByID("editor-buttons-menu");

        if (!menu) {
            return true;
        }

        auto btn = Button::createWithSprite("replaceselected.png"_spr, [](Button*) {
            ReplacePopup::create()->show();
        });
        btn->setContentSize({40, 40});

        menu->addChild(btn);
        menu->updateLayout();

        return true;
    }

};