#pragma once
#include "mono_scene/include/dll_config.h"
#include "riaecs/riaecs.h"

namespace mono_scene
{
    class ISystemListEditCommand : public riaecs::ISystemLoopCommand
    {
    public:
        virtual ~ISystemListEditCommand() = default;
        virtual const std::vector<size_t> &GetNeedSystemIDs() const = 0;
        virtual std::unique_ptr<riaecs::ISystemLoopCommand> Clone() const = 0;
    };

} // namespace mono_scene