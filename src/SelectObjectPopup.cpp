#include "SelectObjectPopup.hpp"

#include <Geode/ui/Button.hpp>

SelectObjectPopup* SelectObjectPopup::create() {
    auto ret = new SelectObjectPopup();

    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool SelectObjectPopup::init() {
    Popup::init(300, 200);
    
    setTitle("Select Object");

    auto bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(44);
    bg->setContentSize({114, 147});
    bg->setPosition({78, 134});
    
    m_mainLayer->addChild(bg);

    bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(44);
    bg->setContentSize({114, 147});
    bg->setPosition({78, 134});
    
    m_mainLayer->addChild(bg);

    auto btn = Button::createWithSpriteFrameName("GJ_arrow_01_001.png", [](Button*) {

    });

    m_mainLayer->addChild(btn);

    auto spr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    spr->setFlipX(true);
    
    btn = Button::createWithNode(spr, [](Button*) {

    });

    m_mainLayer->addChild(btn);

    return true;
}