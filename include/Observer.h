#pragma once

#include <memory>
#include <fstream>
#include <iostream>
#include <mutex>

class NPC;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onKill(const NPC& killer, const NPC& victim) = 0;
};

class ConsoleObserver : public Observer {
private:
    static std::mutex console_mutex_;
    
public:
    void onKill(const NPC& killer, const NPC& victim) override;
};

class FileObserver : public Observer {
private:
    std::ofstream log_file_;
    std::mutex file_mutex_;
    
public:
    explicit FileObserver(const std::string& filename);
    ~FileObserver() override;
    
    void onKill(const NPC& killer, const NPC& victim) override;
};
