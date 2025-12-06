#include "NPCFactory.h"
#include "Orc.h"
#include "Knight.h"
#include "Bear.h"
#include <stdexcept>

std::shared_ptr<NPC> NPCFactory::createNPC(NPCType type, const std::string& name, double x, double y) {
    switch (type) {
        case NPCType::ORC:
            return std::make_shared<Orc>(name, x, y);
        case NPCType::KNIGHT:
            return std::make_shared<Knight>(name, x, y);
        case NPCType::BEAR:
            return std::make_shared<Bear>(name, x, y);
        default:
            throw std::invalid_argument("Unknown NPC type");
    }
}

std::shared_ptr<NPC> NPCFactory::createFromString(const std::string& type_str, const std::string& name, double x, double y) {
    NPCType type = NPC::stringToType(type_str);
    return createNPC(type, name, x, y);
}

std::shared_ptr<NPC> NPCFactory::loadFromStream(std::istream& is) {
    std::string type_str, name;
    double x, y;
    bool alive;
    
    is >> type_str >> name >> x >> y >> alive;
    
    auto npc = createFromString(type_str, name, x, y);
    if (!alive) {
        npc->kill();
    }
    
    return npc;
}
