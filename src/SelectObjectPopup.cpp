#include "SelectObjectPopup.hpp"
#include "ProScrollLayer.hpp"

SelectObjectPopup::SelectObjectPopup(const std::unordered_map<int, std::vector<int>>& tabObjects, Function<void(int)> onSelect)
    : m_tabObjects(tabObjects), m_onSelect(std::move(onSelect)) {}

SelectObjectPopup* SelectObjectPopup::create(const std::unordered_map<int, std::vector<int>>& tabObjects, Function<void(int)> onSelect) {
    auto ret = new SelectObjectPopup(tabObjects, std::move(onSelect));

    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool SelectObjectPopup::init() {
    Popup::init(300, 200);
    
    this->setTitle("Select Object");

    auto bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(44);
    bg->setContentSize({56, 150});
    bg->setPosition({42, 90});
    
    m_mainLayer->addChild(bg);

    bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(44);
    bg->setContentSize({178, 150});
    bg->setPosition({184, 90});
    
    m_mainLayer->addChild(bg);

    auto btn = Button::createWithSpriteFrameName("GJ_arrow_01_001.png", [this](Button*) {
        this->loadPage(m_currentPage - 1);
    });
    btn->setPosition({83, 90});
    btn->setScale(0.6f);

    m_mainLayer->addChild(btn);

    auto spr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    spr->setFlipX(true);
    
    btn = Button::createWithNode(spr, [this](Button*) {
        this->loadPage(m_currentPage + 1);
    });
    btn->setPosition({285, 90});
    btn->setScale(0.6f);

    m_mainLayer->addChild(btn);

    auto scroll = ProScrollLayer::create({56, 135}, true, true);
    scroll->ignoreAnchorPointForPosition(false);
    scroll->setPosition({42, 90});
    scroll->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setGap(3.f)
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(114.f)
    );

    m_mainLayer->addChild(scroll);

    m_currentTab = Mod::get()->getSavedValue<int>("selected-tab", 0);

    this->addTabToggle(scroll, "square_01_001.png");
    this->addTabToggle(scroll, "blockOutline_01_001.png");
    this->addTabToggle(scroll, "triangle_a_02_001.png");
    this->addTabToggle(scroll, "spike_01_001.png");
    this->addTabToggle(scroll, "persp_outline_01_001.png");
    this->addTabToggle(scroll, "ring_01_001.png");
    this->addTabToggle(scroll, "GJBeast01_01_001.png");
    this->addTabToggle(scroll, "pixelb_03_01_001.png");
    this->addTabToggle(scroll, "pixelitem_001_001.png");
    this->addTabToggle(scroll, "particle_01_001.png");
    this->addTabToggle(scroll, "d_spikes_01_001.png");
    this->addTabToggle(scroll, "sawblade_02_001.png");
    this->addTabToggle(scroll, "edit_eTintCol01Btn_001.png");
    this->addTabToggle(scroll, "GJ_optionsBtn_001.png");

    scroll->m_contentLayer->updateLayout();
    scroll->scrollToTop();

    this->loadPage(Mod::get()->getSavedValue<int>(fmt::format("tab-{}-page", m_currentTab), 0));

    return true;
}

void SelectObjectPopup::addTabToggle(ProScrollLayer* scroll, ZStringView frame) {
    auto sprite = CCSprite::createWithSpriteFrameName(frame.c_str());
    auto scaleX = 16.5f / sprite->getContentWidth();
    auto scaleY = 16.5f / sprite->getContentHeight();

    sprite->setScale(std::min(scaleX, scaleY));

    auto off = NineSlice::create("GJ_button_04.png");
    off->setContentSize(CCSize{48, 24} / 0.8f);
    off->setScale(0.8f);
    off->setColor({ 100, 100, 100 });
    off->addChild(sprite);

    sprite->setPosition(off->getContentSize() / 2.f + CCPoint{0, 0.5f});

    sprite = CCSprite::createWithSpriteFrameName(frame.c_str());
    sprite->setScale(std::min(scaleX, scaleY));
    sprite->setOpacity(210);

    auto on = NineSlice::create("GJ_button_04.png");
    on->setContentSize(CCSize{48, 24} / 0.8f);
    on->setScale(0.8f);
    on->addChild(sprite);

    sprite->setPosition(on->getContentSize() / 2.f + CCPoint{0, 0.5f});

    auto toggler = CCMenuItemExt::createToggler(
        off, on,
        [this](CCMenuItemToggler* toggler) {
            for (auto _toggler : m_tabToggles) {
                _toggler->toggle(_toggler == toggler);
                _toggler->setEnabled(_toggler != toggler);
            }

            m_currentTab = toggler->getTag();

            this->loadPage(Mod::get()->getSavedValue<int>(fmt::format("tab-{}-page", m_currentTab), 0));

            Mod::get()->setSavedValue("selected-tab", m_currentTab);
        }
    );
    toggler->setPosition({28, 11.5f});
    toggler->setTag(m_tabToggles.size());
    toggler->m_offButton->m_scaleMultiplier = 1.1f;
    toggler->m_onButton->m_scaleMultiplier = 1.1f;
    
    auto menu = CCMenu::create();
    menu->setContentSize({56, 24});
    menu->addChild(toggler);

    scroll->m_contentLayer->addChild(menu);

    if (static_cast<int>(m_tabToggles.size()) == m_currentTab) {
        toggler->toggle(true);
        toggler->setEnabled(false);
    }

    m_tabToggles.push_back(toggler);
}

void SelectObjectPopup::loadPage(int page) {
    auto objects = m_tabObjects.at(m_currentTab);
    auto maxPage = (static_cast<int>(objects.size()) - 1) / 30;
    
    if (page < 0) {
        page = maxPage;
    } else if (page > maxPage) {
        page = 0;
    }

    Mod::get()->setSavedValue(fmt::format("tab-{}-page", m_currentTab), page);
    
    if (m_tabContainer) {
        m_tabContainer->removeFromParent();
    }

    m_tabContainer = CCNode::create();
    m_tabContainer->setAnchorPoint({0.5f, 0.5f});
    m_tabContainer->setContentSize({167, 139.16666f});
    m_tabContainer->setAnchorPoint({0.f, 1.f});
    m_tabContainer->setPosition({100.5f, 159});
    m_tabContainer->setLayout(
        AxisLayout::create()
            ->setGrowCrossAxis(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::End)
            ->setAutoScale(false)
            ->setGap(3.7f)
            ->ignoreInvisibleChildren(false)
    );

    m_mainLayer->addChild(m_tabContainer); 

    for (int i = page * 30; i < page * 30 + 30; i++) {
        if (i == static_cast<int>(objects.size())) {
            break;
        }

        auto spr = CCSprite::create("GJ_button_04.png");

        auto sprite = createObjectSprite(objects[i], 23.25f);
        sprite->setPosition(spr->getContentSize() / 2.f + CCPoint{0, 0.5f});
        sprite->setCascadeOpacityEnabled(true);

        spr->addChild(sprite);

        auto btn = Button::createWithNode(spr, [this, id = objects[i]](Button*) {
            m_onSelect(id);
            onClose(nullptr);
        });
        btn->setScale(0.62f);
        btn->setPosition({222, 141});

        m_tabContainer->addChild(btn);
    }

    m_tabContainer->updateLayout();

    m_currentPage = page;

    this->updateDots();
}

void SelectObjectPopup::updateDots() {
    if (m_dotContainer) {
        m_dotContainer->removeFromParent();
    }

    m_dotContainer = CCNode::create();
    m_dotContainer->setContentSize({178, 5});
    m_dotContainer->setPosition({184, 10.5f});
    m_dotContainer->setAnchorPoint({0.5f, 0.5f});
    m_dotContainer->setLayout(
        SimpleAxisLayout::create(Axis::Row)
            ->setGap(5.f)
            ->ignoreInvisibleChildren(false)
    );

    m_mainLayer->addChild(m_dotContainer);

    auto maxPage = (static_cast<int>(m_tabObjects.at(m_currentTab).size()) - 1) / 30;

    for (auto child : m_dotContainer->getChildrenExt()) {
        child->removeFromParent();
    }

    for (int i = 0; i <= maxPage; i++) {
        auto dot = CCSprite::create("smallDot.png");
        dot->setScale(0.35f);

        if (i != m_currentPage) {
            dot->setColor({125, 125, 125});
        }

        m_dotContainer->addChild(dot);
    }

    m_dotContainer->updateLayout();
}
