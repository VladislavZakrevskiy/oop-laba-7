#include <gtest/gtest.h>
#include "AsyncGame.h"
#include "NPCFactory.h"
#include "MovementSystem.h"
#include "BattleSystem.h"
#include <thread>
#include <chrono>

class AsyncDungeonTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<AsyncGame>();
    }
    
    void TearDown() override {
        if (game) {
            game->stopGame();
        }
    }
    
    std::unique_ptr<AsyncGame> game;
};

TEST_F(AsyncDungeonTest, NPCCreationThreadSafe) {
    auto orc = NPCFactory::createNPC(NPCType::ORC, "TestOrc", 50, 50);
    auto knight = NPCFactory::createNPC(NPCType::KNIGHT, "TestKnight", 60, 60);
    
    EXPECT_TRUE(orc->isAlive());
    EXPECT_TRUE(knight->isAlive());
    EXPECT_EQ(orc->getType(), NPCType::ORC);
    EXPECT_EQ(knight->getType(), NPCType::KNIGHT);
}

TEST_F(AsyncDungeonTest, MovementSystem) {
    MovementSystem movement_system(100, 100);
    
    auto npc = NPCFactory::createNPC(NPCType::ORC, "MovingOrc", 50, 50);
    movement_system.addNPC(npc);
    
    npc->moveRandomly(100, 100);
    
    EXPECT_GE(npc->getX(), 0);
    EXPECT_LE(npc->getX(), 100);
    EXPECT_GE(npc->getY(), 0);
    EXPECT_LE(npc->getY(), 100);
}

TEST_F(AsyncDungeonTest, BattleRules) {
    auto orc = NPCFactory::createNPC(NPCType::ORC, "TestOrc", 0, 0);
    auto knight = NPCFactory::createNPC(NPCType::KNIGHT, "TestKnight", 5, 5);
    auto bear = NPCFactory::createNPC(NPCType::BEAR, "TestBear", 10, 10);
    
    EXPECT_TRUE(orc->canKill(*bear));
    EXPECT_FALSE(orc->canKill(*knight));
    
    EXPECT_TRUE(knight->canKill(*orc));
    EXPECT_FALSE(knight->canKill(*bear));
    
    EXPECT_TRUE(bear->canKill(*knight));
    EXPECT_FALSE(bear->canKill(*orc));
}

TEST_F(AsyncDungeonTest, ThreadSafeOperations) {
    auto npc = NPCFactory::createNPC(NPCType::ORC, "ThreadSafeOrc", 50, 50);
    
    std::atomic<bool> test_running(true);
    std::thread mover([&npc, &test_running]() {
        while (test_running.load()) {
            npc->moveRandomly(100, 100);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    
    std::thread reader([&npc, &test_running]() {
        while (test_running.load()) {
            volatile double x = npc->getX();
            volatile double y = npc->getY();
            volatile bool alive = npc->isAlive();
            (void)x; (void)y; (void)alive;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    test_running.store(false);
    
    mover.join();
    reader.join();
    
    EXPECT_TRUE(npc->isAlive());
}

TEST_F(AsyncDungeonTest, GameCreation) {
    game->createRandomNPCs(10);
    EXPECT_EQ(game->getTotalCount(), 10u);
    EXPECT_EQ(game->getAliveCount(), 10u);
}

TEST_F(AsyncDungeonTest, ShortGameRun) {
    game->createRandomNPCs(5);
    
    size_t initial_count = game->getAliveCount();
    EXPECT_EQ(initial_count, 5u);
    
    game->startGame();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    game->stopGame();
    
    EXPECT_LE(game->getAliveCount(), initial_count);
}
