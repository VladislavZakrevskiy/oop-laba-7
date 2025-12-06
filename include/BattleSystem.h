#pragma once

#include "MovementSystem.h"
#include "Observer.h"
#include <vector>
#include <memory>
#include <atomic>
#include <thread>

class BattleSystem {
private:
    std::atomic<bool> running_;
    std::vector<std::shared_ptr<Observer>> observers_;
    std::mutex observers_mutex_;
    MovementSystem* movement_system_;

public:
    BattleSystem();
    ~BattleSystem();
    
    void setMovementSystem(MovementSystem* system);
    void addObserver(std::shared_ptr<Observer> observer);
    void removeObserver(std::shared_ptr<Observer> observer);
    
    void start();
    void stop();
    
    void battleLoop();
    
private:
    void processBattle(const BattleTask& task);
    void notifyObservers(const NPC& killer, const NPC& victim);
    bool rollDice();
};
