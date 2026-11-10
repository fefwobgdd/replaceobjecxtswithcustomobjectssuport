#pragma once

using namespace geode::prelude;

static bool isCustomObject(int id) {
    return id < 0;
}

static std::string customObjectString(int id) {
    return GameManager::sharedState()->stringForCustomObject(id);
}

static int previewObjectID(int id) {
    if (!isCustomObject(id)) {
        return id;
    }

    auto firstObject = string::split(customObjectString(id), ";");
    if (firstObject.empty()) {
        return 1;
    }

    auto values = string::split(firstObject.front(), ",");
    return values.size() > 1 ? std::stoi(values[1]) : 1;
}

static CCSprite* createObjectSprite(int id, float size) {
    auto sprite = EditorUI::get()->spriteFromObjectString(
        fmt::format("1,{}", previewObjectID(id)), true, false, 1, nullptr, nullptr, nullptr
    );
    auto scaleX = size / sprite->getContentWidth();
    auto scaleY = size / sprite->getContentHeight();

    sprite->setScale(std::min(scaleX, scaleY));

    if (id == 759 || id == 959) {
        sprite->setScale(sprite->getScale() * 0.4f);
    }

    return sprite;
}
