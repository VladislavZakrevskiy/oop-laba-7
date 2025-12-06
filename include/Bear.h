#pragma once

#include "NPC.h"

class Bear : public NPC {
public:
    Bear(const std::string& name, double x, double y);
    
    bool canKill(const NPC& other) const override;
};
