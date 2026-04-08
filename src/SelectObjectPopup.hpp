#pragma once

#include "Includes.hpp"

class SelectObjectPopup : public Popup {

private:

    bool init() override;

public:

    static SelectObjectPopup* create();

};