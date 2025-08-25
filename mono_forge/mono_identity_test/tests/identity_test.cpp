#include "mono_identity_test/pch.h"

#include "mono_identity/include/component_identity.h"
#pragma comment(lib, "mono_identity.lib")
#pragma comment(lib, "riaecs.lib")

TEST(Identity, Component)
{
    mono_identity::ComponentIdentity identity;
    EXPECT_EQ(identity.name_, "");
    EXPECT_EQ(identity.tag_, 0);
    EXPECT_EQ(identity.layer_, 0);
    EXPECT_EQ(identity.activeSelf_, true);

    identity.name_ = "Test";
    identity.tag_ = 1;
    identity.layer_ = 2;
    identity.activeSelf_ = false;

    EXPECT_EQ(identity.name_, "Test");
    EXPECT_EQ(identity.tag_, 1);
    EXPECT_EQ(identity.layer_, 2);
    EXPECT_EQ(identity.activeSelf_, false);
}