#include "AsyncGame.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    try {
        std::cout << "=== Асинхронная RPG игра Balagur Fate 3 ===" << std::endl;
        std::cout << "Многопоточная версия с движением и боями" << std::endl;
        
        AsyncGame game;
        
        std::cout << "Создаем 50 случайных NPC..." << std::endl;
        game.createRandomNPCs(50);
        
        game.startGame();
        
        std::this_thread::sleep_for(std::chrono::seconds(31));
        
        game.stopGame();
        
        std::cout << "\nПроверьте файл async_battle_log.txt для подробных логов битв." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
