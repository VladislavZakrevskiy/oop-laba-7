#include "Orc.h"

Orc::Orc(const std::string& name, double x, double y)
    : NPC(name, x, y, NPCType::ORC, getStatsForType(NPCType::ORC)) {}

bool Orc::canKill(const NPC& other) const {
    return other.getType() == NPCType::BEAR;
}
