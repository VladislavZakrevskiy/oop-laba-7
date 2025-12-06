#include "BattleSystem.h"
#include <random>
#include <algorithm>

BattleSystem::BattleSystem() : running_(false), movement_system_(nullptr) {}

BattleSystem::~BattleSystem() {
    stop();
}

void BattleSystem::setMovementSystem(MovementSystem* system) {
    movement_system_ = system;
}

void BattleSystem::addObserver(std::shared_ptr<Observer> observer) {
    std::lock_guard<std::mutex> lock(observers_mutex_);
    observers_.push_back(observer);
}

void BattleSystem::removeObserver(std::shared_ptr<Observer> observer) {
    std::lock_guard<std::mutex> lock(observers_mutex_);
    observers_.erase(
        std::remove(observers_.begin(), observers_.end(), observer),
        observers_.end()
    );
}

void BattleSystem::start() {
    running_.store(true);
}

void BattleSystem::stop() {
    running_.store(false);
}

void BattleSystem::battleLoop() {
    while (running_.load()) {
        if (!movement_system_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        
        BattleTask task(nullptr, nullptr);
        if (movement_system_->getBattleTask(task)) {
            processBattle(task);
        }
    }
}

void BattleSystem::processBattle(const BattleTask& task) {
    if (!task.attacker->isAlive() || !task.defender->isAlive()) {
        return;
    }
    
    if (!task.attacker->canKill(*task.defender) || !task.attacker->canReach(*task.defender)) {
        return;
    }
    
    bool attacker_wins = rollDice();
    
    if (attacker_wins) {
        task.defender->kill();
        notifyObservers(*task.attacker, *task.defender);
    }
}

void BattleSystem::notifyObservers(const NPC& killer, const NPC& victim) {
    std::lock_guard<std::mutex> lock(observers_mutex_);
    for (auto& observer : observers_) {
        if (observer) {
            observer->onKill(killer, victim);
        }
    }
}

bool BattleSystem::rollDice() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 6);
    
    int attack = dis(gen);
    int defense = dis(gen);
    
    return attack > defense;
}
