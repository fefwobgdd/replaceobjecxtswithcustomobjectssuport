#pragma once

#include "Includes.hpp"

class AddObjectPopup : public Popup {

private:

    Function<void(int)> m_onAdd;

    AddObjectPopup(Function<void(int)>);

    bool init(const std::vector<int>&, const std::vector<int>&);

public:

    static AddObjectPopup* create(const std::vector<int>&, const std::vector<int>&, Function<void(int)>);

};