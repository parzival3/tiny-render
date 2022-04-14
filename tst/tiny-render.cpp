#include "tiny-render.hpp"

#include "common.hpp"
#include "tests_include.hpp"

TEST(TinyRender, barycentric_test) {
    tr::Vec3f A(454, 341, 0.488050014f);
    tr::Vec3f B(453, 347, 0.49871999f);
    tr::Vec3f C(459, 346, 0.489565015f);
    tr::Vec3f P(453, 341, 0);

    auto result = tr::barycentric({A, B, C}, P);
    EXPECT_FLOAT_EQ((*result)[0], 1.02857149);
    EXPECT_FLOAT_EQ((*result)[1], 0.142857149);
    EXPECT_FLOAT_EQ((*result)[2], -0.171428576);
}

TEST(TinyRender, crossproduct_test) {
    tr::Vec3f A(5, -1, 1);
    tr::Vec3f B(5, 6, 0);
    tr::Vec3f prod = A ^ B;
    tr::Vec3f res(-6, 5, 35);
    EXPECT_FLOAT_EQ(prod[0], res[0]);
    EXPECT_FLOAT_EQ(prod[1], res[1]);
    EXPECT_FLOAT_EQ(prod[2], res[2]);
}
