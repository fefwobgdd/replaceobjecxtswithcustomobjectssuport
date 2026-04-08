#pragma once

#include "Includes.hpp"

class ObjectCell : public CCNode {

private:

    bool init() override;

public:

    static ObjectCell* create();

};