#pragma once

#include "NPC.h"
#include <memory>
#include <string>

class NPCFactory {
public:
    static std::shared_ptr<NPC> createNPC(NPCType type, const std::string& name, double x, double y);
    static std::shared_ptr<NPC> createFromString(const std::string& type_str, const std::string& name, double x, double y);
    static std::shared_ptr<NPC> loadFromStream(std::istream& is);
};
