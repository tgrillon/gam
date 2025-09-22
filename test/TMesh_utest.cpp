//
// Created by friska on 22/09/25.
//
#include <gtest/gtest.h>
#include "TMesh.h"

TEST(TMeshTest, FirstUT)
{
	gam::TMesh mesh;
	ASSERT_EQ(mesh.vertex_count(), 0);
}