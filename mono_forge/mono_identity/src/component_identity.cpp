#include "mono_identity/src/pch.h"
#include "mono_identity/include/component_identity.h"

#pragma comment(lib, "riaecs.lib")

mono_identity::ComponentIdentity::ComponentIdentity()
{
}

mono_identity::ComponentIdentity::~ComponentIdentity()
{
    name_.clear();
    tag_ = 0;
    layer_ = 0;
    activeSelf_ = true;
}

MONO_IDENTITY_API riaecs::ComponentRegistrar
<mono_identity::ComponentIdentity, mono_identity::ComponentIdentityMaxCount> mono_identity::ComponentIdentityID;