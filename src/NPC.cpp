#include "NPC.h"
#include <cmath>
#include <random>
#include <shared_mutex>

NPC::NPC(const std::string& name, double x, double y, NPCType type, const NPCStats& stats)
    : name_(name), x_(x), y_(y), type_(type), alive_(true), stats_(stats) {
    if (x < 0 || x > 100 || y < 0 || y > 100) {
        throw std::invalid_argument("Coordinates must be in range [0, 100]");
    }
}

std::string NPC::getName() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return name_;
}

void NPC::setPosition(double x, double y) {
    if (x < 0 || x > 100 || y < 0 || y > 100) {
        return;
    }
    x_.store(x);
    y_.store(y);
}

void NPC::moveRandomly(double map_width, double map_height) {
    if (!alive_.load()) {
        return;
    }
    
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    std::uniform_real_distribution<> angle_dis(0, 2 * M_PI);
    
    double angle = angle_dis(gen);
    double dx = stats_.move_distance * std::cos(angle);
    double dy = stats_.move_distance * std::sin(angle);
    
    double new_x = std::max(0.0, std::min(map_width, x_.load() + dx));
    double new_y = std::max(0.0, std::min(map_height, y_.load() + dy));
    
    setPosition(new_x, new_y);
}

double NPC::distanceTo(const NPC& other) const {
    double dx = x_.load() - other.x_.load();
    double dy = y_.load() - other.y_.load();
    return std::sqrt(dx * dx + dy * dy);
}

bool NPC::canReach(const NPC& other) const {
    return distanceTo(other) <= stats_.kill_distance;
}

void NPC::serialize(std::ostream& os) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    os << typeToString(type_) << " " << name_ << " " << x_.load() << " " << y_.load() << " " << alive_.load();
}

void NPC::deserialize(std::istream& is) {
    std::lock_guard<std::shared_mutex> lock(mutex_);
    std::string type_str;
    double x, y;
    bool alive;
    is >> type_str >> name_ >> x >> y >> alive;
    type_ = stringToType(type_str);
    x_.store(x);
    y_.store(y);
    alive_.store(alive);
    stats_ = getStatsForType(type_);
}

std::ostream& operator<<(std::ostream& os, const NPC& npc) {
    os << NPC::typeToString(npc.type_) << " \"" << npc.getName() 
       << "\" at (" << npc.x_.load() << ", " << npc.y_.load() << ")";
    if (!npc.alive_.load()) {
        os << " [DEAD]";
    }
    return os;
}

std::string NPC::typeToString(NPCType type) {
    switch (type) {
        case NPCType::ORC: return "Orc";
        case NPCType::KNIGHT: return "Knight";
        case NPCType::BEAR: return "Bear";
        default: return "Unknown";
    }
}

NPCType NPC::stringToType(const std::string& str) {
    if (str == "Orc") return NPCType::ORC;
    if (str == "Knight") return NPCType::KNIGHT;
    if (str == "Bear") return NPCType::BEAR;
    throw std::invalid_argument("Unknown NPC type: " + str);
}

NPCStats NPC::getStatsForType(NPCType type) {
    switch (type) {
        case NPCType::ORC: return {20, 10};
        case NPCType::KNIGHT: return {30, 10};
        case NPCType::BEAR: return {5, 10};
        default: return {10, 10};
    }
}
