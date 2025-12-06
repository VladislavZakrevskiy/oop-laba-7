#include "MovementSystem.h"
#include <chrono>
#include <condition_variable>

MovementSystem::MovementSystem(double map_width, double map_height)
    : running_(false), map_width_(map_width), map_height_(map_height) {}

MovementSystem::~MovementSystem() {
    stop();
}

void MovementSystem::addNPC(std::shared_ptr<NPC> npc) {
    npcs_.push_back(std::move(npc));
}

void MovementSystem::start() {
    running_.store(true);
}

void MovementSystem::stop() {
    running_.store(false);
    battle_queue_cv_.notify_all();
}

void MovementSystem::addBattleTask(const BattleTask& task) {
    std::lock_guard<std::mutex> lock(battle_queue_mutex_);
    battle_queue_.push(task);
    battle_queue_cv_.notify_one();
}

bool MovementSystem::getBattleTask(BattleTask& task) {
    std::unique_lock<std::mutex> lock(battle_queue_mutex_);
    
    battle_queue_cv_.wait(lock, [this] { 
        return !battle_queue_.empty() || !running_.load(); 
    });
    
    if (!running_.load() && battle_queue_.empty()) {
        return false;
    }
    
    if (!battle_queue_.empty()) {
        task = battle_queue_.front();
        battle_queue_.pop();
        return true;
    }
    
    return false;
}

void MovementSystem::movementLoop() {
    while (running_.load()) {
        for (auto& npc : npcs_) {
            if (npc->isAlive()) {
                npc->moveRandomly(map_width_, map_height_);
            }
        }
        
        checkForBattles();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void MovementSystem::checkForBattles() {
    for (size_t i = 0; i < npcs_.size(); ++i) {
        for (size_t j = i + 1; j < npcs_.size(); ++j) {
            auto& npc1 = npcs_[i];
            auto& npc2 = npcs_[j];
            
            if (!npc1->isAlive() || !npc2->isAlive()) {
                continue;
            }
            
            if (npc1->canKill(*npc2) && npc1->canReach(*npc2)) {
                addBattleTask(BattleTask(npc1, npc2));
            } else if (npc2->canKill(*npc1) && npc2->canReach(*npc1)) {
                addBattleTask(BattleTask(npc2, npc1));
            }
        }
    }
}
