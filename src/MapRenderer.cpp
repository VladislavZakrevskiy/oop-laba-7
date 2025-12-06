#include "MapRenderer.h"
#include <iomanip>
#include <algorithm>
#include <map>

// ANSI цветовые коды
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

MapRenderer::MapRenderer(double map_width, double map_height, int grid_width, int grid_height)
    : map_width_(map_width), map_height_(map_height), 
      grid_width_(grid_width), grid_height_(grid_height) {}

void MapRenderer::renderMap(const std::vector<std::shared_ptr<NPC>>& npcs, std::ostream& os) const {
    std::vector<std::vector<std::vector<std::shared_ptr<NPC>>>> grid(
        grid_height_, std::vector<std::vector<std::shared_ptr<NPC>>>(grid_width_)
    );
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            int grid_x = static_cast<int>(npc->getX() / map_width_ * grid_width_);
            int grid_y = static_cast<int>(npc->getY() / map_height_ * grid_height_);
            
            grid_x = std::max(0, std::min(grid_width_ - 1, grid_x));
            grid_y = std::max(0, std::min(grid_height_ - 1, grid_y));
            
            grid[grid_y][grid_x].push_back(npc);
        }
    }
    
    printBorder(grid_width_, os);
    
    for (int y = 0; y < grid_height_; ++y) {
        os << "|";
        for (int x = 0; x < grid_width_; ++x) {
            const auto& cell = grid[y][x];
            
            if (cell.empty()) {
                os << " ";
            } else if (cell.size() == 1) {
                char symbol = getNPCSymbol(cell[0]->getType());
                std::string color = getNPCColor(cell[0]->getType());
                os << color << symbol << RESET;
            } else {
                // Несколько NPC в одной клетке
                os << BOLD << WHITE << "*" << RESET;
            }
        }
        os << "|" << std::endl;
    }
    
    printBorder(grid_width_, os);
}

void MapRenderer::renderMapWithInfo(const std::vector<std::shared_ptr<NPC>>& npcs,
                                   int elapsed_seconds, std::ostream& os) const {
    clearScreen();
    
    os << BOLD << CYAN << "=== BALAGUR FATE 3: ASYNC BATTLE ===" << RESET << std::endl;
    os << "Время: " << elapsed_seconds << "s | ";
    
    std::map<NPCType, int> counts;
    int total_alive = 0;
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            counts[npc->getType()]++;
            total_alive++;
        }
    }
    
    os << "Живых: " << total_alive << "/" << npcs.size() << " | ";
    os << "Орки: " << counts[NPCType::ORC] << " | ";
    os << "Рыцари: " << counts[NPCType::KNIGHT] << " | ";
    os << "Медведи: " << counts[NPCType::BEAR] << std::endl;
    
    os << "Карта " << map_width_ << "x" << map_height_ << " (сетка " 
       << grid_width_ << "x" << grid_height_ << "):" << std::endl;
    
    renderMap(npcs, os);
    printLegend(os);
}

void MapRenderer::renderNPCList(const std::vector<std::shared_ptr<NPC>>& npcs, std::ostream& os) const {
    os << "\n" << BOLD << "=== СПИСОК NPC ===" << RESET << std::endl;
    
    int count = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            std::string color = getNPCColor(npc->getType());
            char symbol = getNPCSymbol(npc->getType());
            
            os << color << symbol << RESET << " " 
               << std::setw(15) << std::left << npc->getName()
               << " (" << std::fixed << std::setprecision(1) 
               << npc->getX() << ", " << npc->getY() << ")" << std::endl;
            
            count++;
            if (count >= 10) {
                os << "... и еще " << (npcs.size() - count) << " NPC" << std::endl;
                break;
            }
        }
    }
}

void MapRenderer::clearScreen() const {
    std::cout << "\033[2J\033[H";
}

char MapRenderer::getNPCSymbol(NPCType type) const {
    switch (type) {
        case NPCType::ORC: return 'O';
        case NPCType::KNIGHT: return 'K';
        case NPCType::BEAR: return 'B';
        default: return '?';
    }
}

std::string MapRenderer::getNPCColor(NPCType type) const {
    switch (type) {
        case NPCType::ORC: return RED;
        case NPCType::KNIGHT: return BLUE;
        case NPCType::BEAR: return GREEN;
        default: return WHITE;
    }
}

void MapRenderer::printBorder(int width, std::ostream& os) const {
    os << "+";
    for (int i = 0; i < width; ++i) {
        os << "-";
    }
    os << "+" << std::endl;
}

void MapRenderer::printLegend(std::ostream& os) const {
    os << "\nЛегенда: ";
    os << RED << "O" << RESET << "=Орк ";
    os << BLUE << "K" << RESET << "=Рыцарь ";
    os << GREEN << "B" << RESET << "=Медведь ";
    os << BOLD << WHITE << "*" << RESET << "=Несколько";
    os << " .=Пусто" << std::endl;
    
    os << "Правила боя: Орки→Медведи, Рыцари→Орки, Медведи→Рыцари" << std::endl;
}
