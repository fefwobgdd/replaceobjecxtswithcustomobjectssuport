#include "ReplacePopup.hpp"
#include "SelectObjectPopup.hpp"
#include "ObjectCell.hpp"

#include <Geode/ui/Button.hpp>

ReplacePopup* ReplacePopup::create() {
    auto ret = new ReplacePopup();

    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ReplacePopup::init() {
    Popup::init(300, 259);

    this->setTitle("Replace Selected");

    auto bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(44);
    bg->setContentSize({114, 147});
    bg->setPosition({78, 134});
    
    m_mainLayer->addChild(bg);

    auto lbl = CCLabelBMFont::create("Replace", "goldFont.fnt");
    lbl->setScale(0.545f);
    lbl->setPosition({78, 198});
    
    m_mainLayer->addChild(lbl);

    auto scroll = ScrollLayer::create({114, 120}, true, true);
    scroll->ignoreAnchorPointForPosition(false);
    scroll->setPosition({78, 126});
    scroll->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setGap(0.f)
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(114.f)
    );

    handleTouchPriority(scroll, true);
    queueInMainThread([scroll = Ref(scroll)] {
        if (auto handler = CCTouchDispatcher::get()->findHandler(scroll.data())) {
            CCTouchDispatcher::get()->setPriority(handler->getPriority() - 1, handler->getDelegate());
        }
    });

    m_mainLayer->addChild(scroll);

    for (int i = 1; i < 7; i++) {
        auto sprite = EditorUI::get()->spriteFromObjectString(fmt::format("1,{}", i), true, false, 1, nullptr, nullptr, nullptr);

        if (sprite->getContentSize() == CCSize{0, 0}){ 
            continue;
        }

        sprite->setScale(0.525f);
        sprite->setPosition({10, 15});
        sprite->setAnchorPoint({0.f, 0.5f});

        float scaleX = 15.75f / sprite->getContentWidth();
        float scaleY = 15.75f / sprite->getContentHeight();

        sprite->setScale(std::min(scaleX, scaleY));

        auto size = CCSize{114, 30};
        auto cell = CCNode::create();
        cell->setContentSize(size);

        cell->addChild(sprite, 1);

        auto bg = NineSlice::create("square02b_001.png");
        bg->setColor({0, 0, 0});
        bg->setOpacity(33);
        bg->setContentSize(size - CCSize{5, 5});
        bg->setPosition(size / 2.f);

        cell->addChild(bg);

        auto lbl = CCLabelBMFont::create(numToString(i).c_str(), "bigFont.fnt");
        lbl->setAnchorPoint({0.f, 0.5f});
        lbl->setPosition({10 + sprite->getScaledContentWidth() + 5, 15.5f});
        lbl->limitLabelWidth(88.5f - lbl->getPositionX(), 0.45f, 0.f);

        cell->addChild(lbl);

        lbl = CCLabelBMFont::create("-", "bigFont.fnt");
        lbl->setScale(0.6f);
        lbl->setOpacity(90);

        auto btn = Button::createWithNode(lbl, [](Button*) {

        });
        btn->setContentSize({19.5f, 19.5f});
        btn->setPosition({99, 16.5f});

        lbl->setPosition(btn->getContentSize() / 2.f);

        cell->addChild(btn);

        scroll->m_contentLayer->addChild(cell);
    }

    lbl = CCLabelBMFont::create("+ add", "bigFont.fnt");
    lbl->setScale(0.45f);
    lbl->setOpacity(90);
    lbl->getChildrenExt()[0]->setScale(1.2f);

    auto btn = Button::createWithNode(lbl, [](Button*) {

    });

    scroll->m_contentLayer->addChild(btn);

    scroll->m_contentLayer->updateLayout();
    scroll->scrollToTop();

    auto line = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    line->setRotation(90);
    line->setScaleX(0.4f);
    line->setOpacity(54);
    line->setPosition(m_size / 2.f + CCPoint{0, 4.5f});

    m_mainLayer->addChild(line);

    bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(44);
    bg->setContentSize({114, 147});
    bg->setPosition({222, 134});
    
    m_mainLayer->addChild(bg);

    lbl = CCLabelBMFont::create("With", "goldFont.fnt");
    lbl->setScale(0.545f);
    lbl->setPosition({222, 198});

    m_mainLayer->addChild(lbl);

    btn = Button::createWithNode(ButtonSprite::create("Replace"), [](Button*) {

    });
    btn->setPosition({m_size.width / 2.f, 27});
    btn->setScale(0.8f);

    m_mainLayer->addChild(btn);

    bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(33);
    bg->setContentSize({94, 113});
    bg->setPosition({222, 127});
    
    m_mainLayer->addChild(bg);

    auto spr = CCSprite::create("GJ_button_04.png");

    auto sprite = EditorUI::get()->spriteFromObjectString(fmt::format("1,{}", 1), true, false, 1, nullptr, nullptr, nullptr);
    sprite->setPosition(spr->getContentSize() / 2.f + CCPoint{0, 0.5f});
    sprite->setCascadeOpacityEnabled(true);
    sprite->setOpacity(149);

    float scaleX = 23.25f / sprite->getContentWidth();
    float scaleY = 23.25f / sprite->getContentHeight();

    sprite->setScale(std::min(scaleX, scaleY));

    spr->addChild(sprite);

    btn = Button::createWithNode(spr, [](Button*) {
        SelectObjectPopup::create()->show();
    });
    btn->setScale(0.625f);
    btn->setPosition({222, 141});

    m_mainLayer->addChild(btn);

    lbl = CCLabelBMFont::create("Not Selected", "bigFont.fnt");
    lbl->setPosition({222, 112});
    lbl->setOpacity(118);
    lbl->limitLabelWidth(75.5f, 0.5f, 0.f);

    m_mainLayer->addChild(lbl);

    return true;
}