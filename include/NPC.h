#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <atomic>

class BattleSystem;

enum class NPCType {
    ORC,
    KNIGHT,
    BEAR
};

struct NPCStats {
    int move_distance;
    int kill_distance;
};

class NPC {
protected:
    std::string name_;
    std::atomic<double> x_, y_;
    NPCType type_;
    std::atomic<bool> alive_;
    NPCStats stats_;
    mutable std::shared_mutex mutex_;

public:
    NPC(const std::string& name, double x, double y, NPCType type, const NPCStats& stats);
    virtual ~NPC() = default;

    std::string getName() const;
    double getX() const { return x_.load(); }
    double getY() const { return y_.load(); }
    NPCType getType() const { return type_; }
    bool isAlive() const { return alive_.load(); }
    NPCStats getStats() const { return stats_; }

    void setPosition(double x, double y);
    void kill() { alive_.store(false); }

    void moveRandomly(double map_width, double map_height);
    
    double distanceTo(const NPC& other) const;

    virtual bool canKill(const NPC& other) const = 0;
    bool canReach(const NPC& other) const;

    virtual void serialize(std::ostream& os) const;
    virtual void deserialize(std::istream& is);

    friend std::ostream& operator<<(std::ostream& os, const NPC& npc);

    static std::string typeToString(NPCType type);
    static NPCType stringToType(const std::string& str);
    static NPCStats getStatsForType(NPCType type);
};
