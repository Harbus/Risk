#include <thread>
#include "..\view\RiskGameView.h"
#include "Dice.h"
#include "RiskGame.h"
#include "World.h"

void RunThread() {
    RiskGame::Instance().ShowMenu();
}

int main() {
    Dice::Seed();
    std::thread thread(RunThread);
    RiskGameView::Instance().Run();
    thread.join();
}