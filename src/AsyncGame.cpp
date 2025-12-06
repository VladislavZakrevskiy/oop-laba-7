#include "AsyncGame.h"
#include "NPCFactory.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <iomanip>

AsyncGame::AsyncGame() 
    : movement_system_(MAP_WIDTH, MAP_HEIGHT), 
      map_renderer_(MAP_WIDTH, MAP_HEIGHT, 25, 12),
      running_(false) {
    
    console_observer_ = std::make_shared<ConsoleObserver>();
    file_observer_ = std::make_shared<FileObserver>("async_battle_log.txt");
    
    battle_system_.setMovementSystem(&movement_system_);
    battle_system_.addObserver(console_observer_);
    battle_system_.addObserver(file_observer_);
}

AsyncGame::~AsyncGame() {
    stopGame();
}

void AsyncGame::createRandomNPCs(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> pos_dis(0.0, MAP_WIDTH);
    std::uniform_int_distribution<> type_dis(0, 2);
    
    std::vector<std::string> orc_names = {"Grosh", "Ugluk", "Azog", "Bolg", "Grishnakh"};
    std::vector<std::string> knight_names = {"Galahad", "Lancelot", "Percival", "Gareth", "Tristan"};
    std::vector<std::string> bear_names = {"Baloo", "Yogi", "Paddington", "Winnie", "Smokey"};
    
    for (int i = 0; i < count; ++i) {
        NPCType type = static_cast<NPCType>(type_dis(gen));
        double x = pos_dis(gen);
        double y = pos_dis(gen);
        
        std::string name;
        switch (type) {
            case NPCType::ORC:
                name = orc_names[i % orc_names.size()] + std::to_string(i);
                break;
            case NPCType::KNIGHT:
                name = knight_names[i % knight_names.size()] + std::to_string(i);
                break;
            case NPCType::BEAR:
                name = bear_names[i % bear_names.size()] + std::to_string(i);
                break;
        }
        
        auto npc = NPCFactory::createNPC(type, name, x, y);
        movement_system_.addNPC(npc);
    }
}

void AsyncGame::startGame() {
    running_.store(true);
    start_time_ = std::chrono::steady_clock::now();
    
    movement_system_.start();
    battle_system_.start();
    
    movement_thread_ = std::thread(&AsyncGame::movementThreadFunc, this);
    battle_thread_ = std::thread(&AsyncGame::battleThreadFunc, this);
    display_thread_ = std::thread(&AsyncGame::displayLoop, this);
    
    std::cout << "Игра началась! Продолжительность: " << GAME_DURATION_SECONDS << " секунд" << std::endl;
    std::cout << "Карта: " << MAP_WIDTH << "x" << MAP_HEIGHT << std::endl;
    std::cout << "Начальное количество NPC: " << getTotalCount() << std::endl;
}

void AsyncGame::stopGame() {
    if (running_.load()) {
        running_.store(false);
        
        movement_system_.stop();
        battle_system_.stop();
        
        if (movement_thread_.joinable()) movement_thread_.join();
        if (battle_thread_.joinable()) battle_thread_.join();
        if (display_thread_.joinable()) display_thread_.join();
        
        std::cout << "\n=== ИГРА ОКОНЧЕНА ===" << std::endl;
        printSurvivors();
    }
}

void AsyncGame::printMap() const {
    static std::mutex print_mutex;
    std::lock_guard<std::mutex> lock(print_mutex);
    
    auto elapsed = std::chrono::steady_clock::now() - start_time_;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    
    map_renderer_.renderMapWithInfo(movement_system_.getNPCs(), seconds);
}

void AsyncGame::printDetailedMap() const {
    static std::mutex print_mutex;
    std::lock_guard<std::mutex> lock(print_mutex);
    
    auto elapsed = std::chrono::steady_clock::now() - start_time_;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    
    map_renderer_.renderMapWithInfo(movement_system_.getNPCs(), seconds);
    map_renderer_.renderNPCList(movement_system_.getNPCs());
}

void AsyncGame::printSurvivors() const {
    std::cout << "=== ВЫЖИВШИЕ ===" << std::endl;
    for (const auto& npc : movement_system_.getNPCs()) {
        if (npc->isAlive()) {
            std::cout << *npc << std::endl;
        }
    }
    std::cout << "Всего выживших: " << getAliveCount() << "/" << getTotalCount() << std::endl;
}

size_t AsyncGame::getAliveCount() const {
    return std::count_if(movement_system_.getNPCs().begin(), movement_system_.getNPCs().end(),
        [](const std::shared_ptr<NPC>& npc) { return npc->isAlive(); });
}

size_t AsyncGame::getTotalCount() const {
    return movement_system_.getNPCs().size();
}

void AsyncGame::displayLoop() {
    while (running_.load()) {
        auto elapsed = std::chrono::steady_clock::now() - start_time_;
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
        
        if (seconds >= GAME_DURATION_SECONDS) {
            running_.store(false);
            break;
        }
        
        printMap();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void AsyncGame::movementThreadFunc() {
    movement_system_.movementLoop();
}

void AsyncGame::battleThreadFunc() {
    battle_system_.battleLoop();
}
