#pragma once

#include "Includes.hpp"

class ReplacePopup : public Popup {

private:

    bool init() override;

public:

    static ReplacePopup* create();

};