#pragma once

using namespace geode::prelude;

static CCSprite* createObjectSprite(int id, float size) {
    auto sprite = EditorUI::get()->spriteFromObjectString(fmt::format("1,{}", id), true, false, 1, nullptr, nullptr, nullptr);
    auto scaleX = size / sprite->getContentWidth();
    auto scaleY = size / sprite->getContentHeight();

    sprite->setScale(std::min(scaleX, scaleY));

    if (id == 759 || id == 959) {
        sprite->setScale(sprite->getScale() * 0.4f);
    }

    return sprite;
}