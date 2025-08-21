#pragma once
#include "riaecs/riaecs.h"

#include <future>
#include <mutex>

namespace mono_forge
{
    class GameLoop
    {
    private:
        bool isRunning_ = true;
        std::future<void> future_;
        mutable std::shared_mutex mutex_;

    public:
        GameLoop() = default;
        ~GameLoop();

        void Run(riaecs::ISystemLoop &systemLoop,riaecs::IECSWorld &ecsWorld, riaecs::IAssetContainer &assetCont);
        bool IsRunning() const;
    };

} // namespace mono_forge