//
// Created by friska on 22/09/25.
//
#include "PCH/PCH.h"

#include "TMesh.h"

#include <gtest/gtest.h>

TEST(TMeshTest, FirstUT)
{
	gam::TMesh mesh;
	ASSERT_EQ(mesh.vertex_count(), 0);
}
