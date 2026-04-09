#include "AddObjectPopup.hpp"
#include "ProScrollLayer.hpp"

#include <Geode/ui/Button.hpp>

AddObjectPopup::AddObjectPopup(Function<void(int)> onAdd)
    : m_onAdd(std::move(onAdd)) {}

AddObjectPopup* AddObjectPopup::create(const std::vector<int>& objects, const std::vector<int>& addedObjects, Function<void(int)> onAdd) {
    auto ret = new AddObjectPopup(std::move(onAdd));

    if (ret->init(objects, addedObjects)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool AddObjectPopup::init(const std::vector<int>& objects, const std::vector<int>& addedObjects) {
    Popup::init(215, 238);

    this->setTitle("Add Object");

    auto scroll = ProScrollLayer::create({178, 182}, true, true);
    scroll->m_contentLayer->setLayout(
        AxisLayout::create()
            ->setGrowCrossAxis(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::End)
            ->setAutoScale(false)
            ->setGap(3.85f)
    );

    auto border = Border::create(scroll, {0, 0, 0, 44}, {178, 182}, {0, 0});
    border->ignoreAnchorPointForPosition(false);
    border->setPosition({m_size.width / 2.f, 108});
    border->setPaddingLeft(5.f);
    border->setPaddingTop(5.f);

    m_mainLayer->addChild(border);
    
    auto scrollbar = Scrollbar::create(scroll);
    scrollbar->setPosition({204, 108});

    m_mainLayer->addChild(scrollbar);

    for (auto id : objects) {
        auto spr = CCSprite::create("GJ_button_04.png");

        auto sprite = createObjectSprite(id, 23.25f);
        sprite->setPosition(spr->getContentSize() / 2.f + CCPoint{0, 0.5f});
        sprite->setCascadeOpacityEnabled(true);

        spr->addChild(sprite);

        auto btn = Button::createWithNode(spr, [this, id](Button*) {
            onClose(nullptr);
            m_onAdd(id);
        });
        btn->setScale(0.62f);
        btn->setPosition({222, 141});
        btn->setScaleMultiplier(1.1f);

        if (std::ranges::contains(addedObjects, id)) {
            spr->setColor({50, 50, 50});
            spr->setOpacity(100);
            sprite->setOpacity(100);
            btn->setEnabled(false);
        }

        scroll->m_contentLayer->addChild(btn);
    }

    scroll->m_contentLayer->updateLayout();
    scroll->scrollToTop();

    return true;
}