#include "ReplacePopup.hpp"
#include "SelectObjectPopup.hpp"
#include "AddObjectPopup.hpp"
#include "ProScrollLayer.hpp"

#include <Geode/ui/GeodeUI.hpp>

#include <ranges>

ReplacePopup::ReplacePopup(ProEditorUI* editorUI)
    : m_editorUI(editorUI) {}

ReplacePopup* ReplacePopup::create(ProEditorUI* editorUI) {
    auto ret = new ReplacePopup(editorUI);

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

    m_scroll = ScrollLayer::create({114, 120}, true, true);
    m_scroll->ignoreAnchorPointForPosition(false);
    m_scroll->setPosition({78, 126});
    m_scroll->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setGap(0.f)
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(114.f)
            ->ignoreInvisibleChildren(false)
    );

    m_mainLayer->addChild(m_scroll);

    auto cellSize = CCSize{114, 30};

    m_anyCell = CCMenu::create();
    m_anyCell->setContentSize(cellSize);
    m_anyCell->setVisible(false);
    
    bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(33);
    bg->setContentSize(cellSize - CCSize{5, 5});
    bg->setPosition(cellSize / 2.f);

    m_anyCell->addChild(bg);

    auto toggler = CCMenuItemExt::createToggler(
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        [this](CCMenuItemToggler* toggler) {
            Mod::get()->setSavedValue("any-enabled", !toggler->isToggled());
            this->updateStuff();
        }
    );
    toggler->toggle(Mod::get()->getSavedValue<bool>("any-enabled"));
    toggler->setPosition({33, 15});
    toggler->setScale(0.55f);

    m_anyCell->addChild(toggler);

    lbl = CCLabelBMFont::create("Any", "bigFont.fnt");
    lbl->setScale(0.45f);
    lbl->setPosition({64, 16});

    m_anyCell->addChild(lbl);

    lbl = CCLabelBMFont::create("+ add", "bigFont.fnt");
    lbl->setScale(0.45f);
    lbl->setOpacity(90);
    lbl->getChildrenExt()[0]->setScale(1.2f);

    m_addBtn = Button::createWithNode(lbl, [this](Button*) {
        AddObjectPopup::create(m_totalObjects, m_objects, [this](int id) {
            m_objects.push_back(id);
            this->addObjectCell(id);

            auto obj = Mod::get()->getSavedValue<matjson::Value>("added-objects");

            if (!obj.isArray()) {
                obj = matjson::Value::array();
            }

            bool contains = false;

            for (const auto& value : obj) {
                if (value.asInt().unwrapOr(0) == id) {
                    contains = true;
                    break;
                }
            }
            
            if (!contains) {
                obj.push(id);
            }

            Mod::get()->setSavedValue("added-objects", obj);
        })->show();
    });
    m_addBtn->setContentWidth(114);
    
    lbl->setPositionX(57);

    m_scroll->m_contentLayer->addChild(m_addBtn);

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

    bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(33);
    bg->setContentSize({94, 113});
    bg->setPosition({222, 127});
    
    m_mainLayer->addChild(bg);

    auto btnSpr = ButtonSprite::create("Replace");
    btnSpr->setCascadeOpacityEnabled(true);

    m_replaceBtn = Button::createWithNode(btnSpr, [this](Button*) {
        m_editorUI->replace(
            Mod::get()->getSavedValue<bool>("any-enabled") ? std::vector<int>{} : m_objects, m_selectedObject
        );
        this->onClose(nullptr);
    });
    m_replaceBtn->setPosition({m_size.width / 2.f, 27});
    m_replaceBtn->setScale(0.8f);
    m_replaceBtn->setCascadeOpacityEnabled(true);

    m_mainLayer->addChild(m_replaceBtn);

    auto btn = Button::createWithSpriteFrameName("GJ_optionsBtn_001.png", [](Button*) {
        geode::openSettingsPopup(Mod::get(), false);
    });
    btn->setScale(0.725f);
    btn->setPosition(m_size - ccp(2, 2));

    m_mainLayer->addChild(btn);

    this->selectObject(Mod::get()->getSavedValue<int>("selected-object", 0));

    if (m_editorUI->m_selectedObject) {
        if (!m_objectCounts.contains(m_editorUI->m_selectedObject->m_objectID)) {
            m_totalObjects.push_back(m_editorUI->m_selectedObject->m_objectID);
        }
        
        m_objectCounts[m_editorUI->m_selectedObject->m_objectID]++;
    }

    if (m_editorUI->m_selectedObjects) {
        for (auto obj : CCArrayExt<GameObject*>(m_editorUI->m_selectedObjects)) {
            if (!m_objectCounts.contains(obj->m_objectID)) {
                m_totalObjects.push_back(obj->m_objectID);
            }
            
            m_objectCounts[obj->m_objectID]++;
        }
    }

    std::sort(m_totalObjects.begin(), m_totalObjects.end(), [this](int a, int b) {
        return m_objectCounts.at(a) > m_objectCounts.at(b);
    });


    if (m_totalObjects.size() == 1) {
        m_objects.push_back(m_totalObjects.front());
    } else {
        auto obj = Mod::get()->getSavedValue<matjson::Value>("added-objects");
        auto ids = std::unordered_set<int>{};

        for (const auto& value : obj) {
            if (auto id = value.asInt().unwrapOr(9)) {
                ids.insert(id);
            }
        }

        for (auto id : m_totalObjects) {
            if (ids.contains(id)) {
                m_objects.push_back(id);
            }
        }
    }

    for (auto id : m_objects | std::views::reverse) {
        this->addObjectCell(id);
    }

    this->updateStuff();

    m_scroll->scrollToTop();

    return true;
}

void ReplacePopup::selectObject(int id) {
    Mod::get()->setSavedValue("selected-object", id);

    auto lbl = CCLabelBMFont::create(id <= 0 ? "Not Selected" : fmt::format("ID: {}", id).c_str(), "bigFont.fnt");
    lbl->setPosition({222, 112});
    lbl->limitLabelWidth(75.5f, 0.5f, 0.f);

    if (id <= 0) {
        lbl->setOpacity(118);
    }

    m_mainLayer->addChild(lbl);

    auto spr = CCSprite::create("GJ_button_04.png");

    auto sprite = createObjectSprite(id <= 0 ? 1 : id, 23.25f);
    sprite->setPosition(spr->getContentSize() / 2.f + CCPoint{0, 0.5f});

    if (id <= 0) {
        sprite->setCascadeOpacityEnabled(true);
        sprite->setOpacity(149);
    }

    spr->addChild(sprite);

    m_selectedObjBtn = Button::createWithNode(spr, [this, lbl](Button*) {
        SelectObjectPopup::create(
            m_editorUI->m_fields->tabObjects,
            [this, lbl](int id) {
                lbl->removeFromParent();
                m_selectedObjBtn->removeFromParent();
                this->selectObject(id);
            }
        )->show();
    });
    m_selectedObjBtn->setScale(0.625f);
    m_selectedObjBtn->setPosition({222, 141});

    m_mainLayer->addChild(m_selectedObjBtn);

    m_selectedObject = id;

    this->updateStuff();
}

void ReplacePopup::addObjectCell(int id) {
    auto cellSize = CCSize{114, 30};

    auto sprite = createObjectSprite(id, 15.75f);
    sprite->setPosition({10, 15});
    sprite->setAnchorPoint({0.f, 0.5f});

    auto cell = CCNode::create();
    cell->setContentSize(cellSize);

    cell->addChild(sprite, 1);

    auto bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(33);
    bg->setContentSize(cellSize - CCSize{5, 5});
    bg->setPosition(cellSize / 2.f);

    cell->addChild(bg);

    auto lbl = CCLabelBMFont::create(fmt::format("x{}", m_objectCounts.at(id)).c_str(), "bigFont.fnt");
    lbl->setAnchorPoint({0.f, 0.5f});
    lbl->setPosition({10 + sprite->getScaledContentWidth() + 5, 15.5f});
    lbl->limitLabelWidth(88.5f - lbl->getPositionX(), 0.45f, 0.f);

    cell->addChild(lbl);

    if (m_totalObjects.size() > 1) {
        lbl = CCLabelBMFont::create("-", "bigFont.fnt");
        lbl->setScale(0.6f);
        lbl->setOpacity(90);

        auto btn = Button::createWithNode(lbl, [this, id, cell](Button*) {
            m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), id), m_objects.end());
            cell->removeFromParent();
            this->updateStuff();

            auto obj = Mod::get()->getSavedValue<matjson::Value>("added-objects");
            auto newObj = matjson::Value::array();

            for (const auto& value : obj) {
                if (value.asInt().unwrapOr(0) != id) {
                    newObj.push(value);
                }
            }

            Mod::get()->setSavedValue("added-objects", newObj);
        });
        btn->setContentSize({19.5f, 19.5f});
        btn->setPosition({99, 16.5f});

        lbl->setPosition(btn->getContentSize() / 2.f);

        cell->addChild(btn);
    }

    cell->setTag(m_objectCounts.at(id));

    auto didAdd = false;

    for (auto child : m_scroll->m_contentLayer->getChildrenExt()) {
        if (child != m_anyCell && child != m_addBtn && child->getTag() <= cell->getTag()) {
            m_scroll->m_contentLayer->insertBefore(cell, child);
            didAdd = true;
            break;
        }
    }

    if (!didAdd) {
        m_scroll->m_contentLayer->insertBefore(cell, m_addBtn);
    }

    this->updateStuff();
}

void ReplacePopup::updateStuff() {
    auto enabled = m_selectedObject > 0 && (!m_objects.empty() || Mod::get()->getSavedValue<bool>("any-enabled"));

    m_replaceBtn->setOpacity(enabled ? 255 : 100);
    m_replaceBtn->setEnabled(enabled);

    auto visible = m_objects.empty() && m_totalObjects.size() > 1;

    m_anyCell->setVisible(visible);

    if (!visible) {
        m_anyCell->removeFromParent();
    } else if (!m_scroll->m_contentLayer->getChildren()->containsObject(m_anyCell)) {
        m_scroll->m_contentLayer->insertBefore(m_anyCell, m_addBtn);
    }

    enabled = m_totalObjects.size() > 1
        && m_totalObjects.size() != m_objects.size()
        && (!Mod::get()->getSavedValue<bool>("any-enabled") || !m_anyCell->isVisible());

    m_addBtn->setEnabled(enabled);
    m_addBtn->setOpacity(enabled ? 255 : 130);
    m_addBtn->setColor(enabled ? ccColor3B{255, 255, 255} : ccColor3B{60, 60, 60});

    auto posMult = m_scroll->m_contentLayer->getPositionY() / m_scroll->m_contentLayer->getContentHeight();

    m_scroll->m_contentLayer->updateLayout();
    m_scroll->m_contentLayer->setPositionY(m_scroll->m_contentLayer->getContentHeight() * posMult);

    m_addBtn->setOpacity(static_cast<int>(m_totalObjects.size()) > 1 ? m_addBtn->getOpacity() : 0);
}