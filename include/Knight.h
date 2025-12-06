#pragma once

#include "NPC.h"

class Knight : public NPC {
public:
    Knight(const std::string& name, double x, double y);
    
    bool canKill(const NPC& other) const override;
};
