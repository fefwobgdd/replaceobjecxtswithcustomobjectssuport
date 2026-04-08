#include "ObjectCell.hpp"

ObjectCell* ObjectCell::create() {
    auto ret = new ObjectCell();

    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ObjectCell::init() {
    this->setContentSize({114, 30});

    auto bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(33);
    bg->setContentSize(getContentSize() - CCSize{5, 5});
    bg->setPosition(getContentSize() / 2.f);

    this->addChild(bg);

    return true;
}