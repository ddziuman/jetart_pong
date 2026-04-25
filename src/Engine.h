#ifndef ENGINE_H
#define ENGINE_H
#include "WindowManager.h"
#include "ActorManager.h"
#include "Timer.h"
#include "PlayerPaddle.h"

class Engine {
private:
    WindowManager m_wm {};
    ActorManager m_am {};
    Timer m_timer {};
    PlayerPaddle m_player {};
    PlayerPaddle m_bot {};
    
public:
    void init();
    bool run();
};

#endif