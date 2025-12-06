#include "Observer.h"
#include "NPC.h"
#include <iostream>
#include <iomanip>
#include <chrono>

std::mutex ConsoleObserver::console_mutex_;

void ConsoleObserver::onKill(const NPC& killer, const NPC& victim) {
    std::lock_guard<std::mutex> lock(console_mutex_);
    std::cout << "[BATTLE] " << killer << " killed " << victim << std::endl;
}

FileObserver::FileObserver(const std::string& filename) : log_file_(filename, std::ios::app) {
    if (!log_file_.is_open()) {
        throw std::runtime_error("Cannot open log file: " + filename);
    }
}

FileObserver::~FileObserver() {
    std::lock_guard<std::mutex> lock(file_mutex_);
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

void FileObserver::onKill(const NPC& killer, const NPC& victim) {
    std::lock_guard<std::mutex> lock(file_mutex_);
    
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    log_file_ << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] "
              << killer << " killed " << victim << std::endl;
    log_file_.flush();
}
