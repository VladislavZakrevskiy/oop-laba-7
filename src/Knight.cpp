#include "Knight.h"

Knight::Knight(const std::string& name, double x, double y)
    : NPC(name, x, y, NPCType::KNIGHT, getStatsForType(NPCType::KNIGHT)) {}

bool Knight::canKill(const NPC& other) const {
    return other.getType() == NPCType::ORC;
}
