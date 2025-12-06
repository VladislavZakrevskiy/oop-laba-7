#pragma once

#include "NPC.h"
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <queue>

struct BattleTask {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
    
    BattleTask(std::shared_ptr<NPC> a, std::shared_ptr<NPC> d) 
        : attacker(std::move(a)), defender(std::move(d)) {}
};

class MovementSystem {
private:
    std::vector<std::shared_ptr<NPC>> npcs_;
    std::atomic<bool> running_;
    std::queue<BattleTask> battle_queue_;
    std::mutex battle_queue_mutex_;
    std::condition_variable battle_queue_cv_;
    double map_width_;
    double map_height_;

public:
    MovementSystem(double map_width = 100.0, double map_height = 100.0);
    ~MovementSystem();
    
    void addNPC(std::shared_ptr<NPC> npc);
    void start();
    void stop();
    
    void addBattleTask(const BattleTask& task);
    bool getBattleTask(BattleTask& task);
    
    const std::vector<std::shared_ptr<NPC>>& getNPCs() const { return npcs_; }
    
    void movementLoop();
    
private:
    void checkForBattles();
};
