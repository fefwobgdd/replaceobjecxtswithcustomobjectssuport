#pragma once

#include "Includes.hpp"

#include <Geode/ui/Button.hpp>

class ProScrollLayer;

class SelectObjectPopup : public Popup {

private:

    Function<void(int)> m_onSelect;

    std::unordered_map<int, std::vector<int>> m_tabObjects;

    std::vector<CCMenuItemToggler*> m_tabToggles;

    CCNode* m_tabContainer = nullptr;
    CCNode* m_dotContainer = nullptr;

    int m_currentTab = 0;
    int m_currentPage = 0;

    SelectObjectPopup(const std::unordered_map<int, std::vector<int>>&, Function<void(int)>);

    bool init() override;

    void addTabToggle(ProScrollLayer*, ZStringView);
    void loadPage(int);
    void updateDots();

public:

    static SelectObjectPopup* create(const std::unordered_map<int, std::vector<int>>&, Function<void(int)>);

};