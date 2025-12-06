#include "Bear.h"

Bear::Bear(const std::string& name, double x, double y)
    : NPC(name, x, y, NPCType::BEAR, getStatsForType(NPCType::BEAR)) {}

bool Bear::canKill(const NPC& other) const {
    return other.getType() == NPCType::KNIGHT;
}
