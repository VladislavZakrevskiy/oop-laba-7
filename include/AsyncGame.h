#pragma once

#include "MovementSystem.h"
#include "BattleSystem.h"
#include "Observer.h"
#include "MapRenderer.h"
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>

class AsyncGame {
private:
    MovementSystem movement_system_;
    BattleSystem battle_system_;
    MapRenderer map_renderer_;
    std::shared_ptr<ConsoleObserver> console_observer_;
    std::shared_ptr<FileObserver> file_observer_;
    
    std::thread movement_thread_;
    std::thread battle_thread_;
    std::thread display_thread_;
    
    std::atomic<bool> running_;
    std::chrono::steady_clock::time_point start_time_;
    
    static constexpr double MAP_WIDTH = 100.0;
    static constexpr double MAP_HEIGHT = 100.0;
    static constexpr int GAME_DURATION_SECONDS = 30;

public:
    AsyncGame();
    ~AsyncGame();
    
    void createRandomNPCs(int count);
    void startGame();
    void stopGame();
    
    void printMap() const;
    void printSurvivors() const;
    void printDetailedMap() const;
    
    size_t getAliveCount() const;
    size_t getTotalCount() const;

private:
    void displayLoop();
    void movementThreadFunc();
    void battleThreadFunc();
};
