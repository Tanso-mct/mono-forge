#pragma once
#include "mono_identity/include/dll_config.h"
#include "riaecs/riaecs.h"

#include <shared_mutex>
#include <string>

namespace mono_identity
{
    constexpr size_t ComponentIdentityMaxCount = 50000;
    class MONO_IDENTITY_API ComponentIdentity
    {
    public:
        ComponentIdentity();
        ~ComponentIdentity();

        std::shared_mutex mutex_;

        std::string name_;
        size_t tag_ = 0;
        size_t layer_ = 0;
        bool activeSelf_ = true;
    };
    extern MONO_IDENTITY_API riaecs::ComponentRegistrar<ComponentIdentity, ComponentIdentityMaxCount> ComponentIdentityID;

} // namespace mono_identity