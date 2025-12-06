#include "AsyncGame.h"
#include "MapRenderer.h"
#include "NPCFactory.h"
#include <iostream>
#include <thread>
#include <chrono>

void demonstrateRenderer() {
    std::cout << "=== Демонстрация MapRenderer ===" << std::endl;
    
    AsyncGame game;
    
    std::cout << "Создаем 20 NPC для демонстрации..." << std::endl;
    game.createRandomNPCs(20);
    
    std::cout << "\nНачальное расположение NPC:" << std::endl;
    game.printDetailedMap();
    
    std::cout << "\nНажмите Enter чтобы запустить игру...";
    std::cin.get();
    
    game.startGame();
    
    for (int i = 0; i < 15; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        if (i % 3 == 0) {
            game.printDetailedMap();
        } else {
            game.printMap();
        }
        
        if (game.getAliveCount() <= 1) {
            std::cout << "\nОстался только один выживший! Игра окончена." << std::endl;
            break;
        }
    }
    
    game.stopGame();
    
    std::cout << "\nФинальное состояние:" << std::endl;
    game.printSurvivors();
}

void demonstrateStaticRenderer() {
    std::cout << "\n=== Демонстрация статического рендерера ===" << std::endl;
    
    MapRenderer renderer(100, 100, 30, 15);
    
    std::vector<std::shared_ptr<NPC>> npcs;
    
    npcs.push_back(NPCFactory::createNPC(NPCType::ORC, "Grosh", 10, 10));
    npcs.push_back(NPCFactory::createNPC(NPCType::KNIGHT, "Arthur", 50, 50));
    npcs.push_back(NPCFactory::createNPC(NPCType::BEAR, "Baloo", 90, 90));
    npcs.push_back(NPCFactory::createNPC(NPCType::ORC, "Ugluk", 20, 80));
    npcs.push_back(NPCFactory::createNPC(NPCType::KNIGHT, "Lancelot", 80, 20));
    npcs.push_back(NPCFactory::createNPC(NPCType::BEAR, "Yogi", 50, 25));
    
    std::cout << "\nСтатическая карта с 6 NPC:" << std::endl;
    renderer.renderMapWithInfo(npcs, 0);
    renderer.renderNPCList(npcs);
    
    std::cout << "\nПосле движения:" << std::endl;
    for (auto& npc : npcs) {
        npc->moveRandomly(100, 100);
    }
    
    renderer.renderMapWithInfo(npcs, 5);
}

int main() {
    try {
        std::cout << "=== BALAGUR FATE 3: ENHANCED MAP RENDERER ===" << std::endl;
        std::cout << "Выберите демонстрацию:" << std::endl;
        std::cout << "1. Статический рендерер" << std::endl;
        std::cout << "2. Динамическая игра с рендерером" << std::endl;
        std::cout << "Введите номер (1 или 2): ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1) {
            demonstrateStaticRenderer();
        } else if (choice == 2) {
            demonstrateRenderer();
        } else {
            std::cout << "Неверный выбор. Запускаю статический рендерер..." << std::endl;
            demonstrateStaticRenderer();
        }
        
        std::cout << "\nДемонстрация завершена!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
