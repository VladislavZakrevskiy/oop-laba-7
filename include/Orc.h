#pragma once

#include "NPC.h"

class Orc : public NPC {
public:
    Orc(const std::string& name, double x, double y);
    
    bool canKill(const NPC& other) const override;
};
