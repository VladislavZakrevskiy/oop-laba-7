#pragma once

#include "NPC.h"
#include <vector>
#include <memory>
#include <iostream>
#include <string>

class MapRenderer {
private:
    double map_width_;
    double map_height_;
    int grid_width_;
    int grid_height_;
    
public:
    MapRenderer(double map_width = 100.0, double map_height = 100.0, 
                int grid_width = 20, int grid_height = 10);
    
    void renderMap(const std::vector<std::shared_ptr<NPC>>& npcs, 
                   std::ostream& os = std::cout) const;
    
    void renderMapWithInfo(const std::vector<std::shared_ptr<NPC>>& npcs,
                          int elapsed_seconds, std::ostream& os = std::cout) const;
    
    void renderNPCList(const std::vector<std::shared_ptr<NPC>>& npcs,
                      std::ostream& os = std::cout) const;
    
    void clearScreen() const;
    
private:
    char getNPCSymbol(NPCType type) const;
    std::string getNPCColor(NPCType type) const;
    void printBorder(int width, std::ostream& os) const;
    void printLegend(std::ostream& os) const;
};
