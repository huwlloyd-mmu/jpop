#include "gtest/gtest.h"
#include "../../../src/puzzles/sudoku/valueset.h"

class ValueSetTest : public ::testing::Test {
protected:
	ValueSetTest() :
		empty_4(4, 0x0), full_4(4, 0xf), empty_9(9, 0x0),
		full_9(9, 0x1ff), empty_16(16, 0x0), full_16(16, 0xffff) {}
	~ValueSetTest() override {}
	void SetUp() override {}
	void TearDown() override {}

	ValueSet empty_4;
	ValueSet full_4;
	ValueSet empty_9;
	ValueSet full_9;
	ValueSet empty_16;
	ValueSet full_16;

	string AB9  = "123456789";
	string AB16 = "0123456789abcdef";
};

TEST_F(ValueSetTest, Constructor_4_0) {
	ValueSet v(4);
	EXPECT_EQ("-", v.toString(AB9));
}

TEST_F(ValueSetTest, Constructor_9_0) {
	ValueSet v(9);
	EXPECT_EQ("-", v.toString(AB9));
}

TEST_F(ValueSetTest, Constructor_16_0) {
	ValueSet v(16);
	EXPECT_EQ("-", v.toString(AB16));
}

TEST_F(ValueSetTest, Constructor_4_1) {
	ValueSet v(4, 0x2);
	EXPECT_EQ("2", v.toString(AB9));
}

TEST_F(ValueSetTest, Constructor_9_1) {
	ValueSet v(9, 0x80);
	EXPECT_EQ("8", v.toString(AB9));
}

TEST_F(ValueSetTest, Constructor_16_1) {
	ValueSet v(16, 0x4000);
	EXPECT_EQ("e", v.toString(AB16));
}

TEST_F(ValueSetTest, Constructor_4_3) {
	ValueSet v(4, 0xb);
	EXPECT_EQ("(124)", v.toString(AB9));
}

TEST_F(ValueSetTest, Constructor_9_3) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
}

TEST_F(ValueSetTest, Constructor_16_3) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
}

TEST_F(ValueSetTest, Clear_4) {
	ValueSet v(4, 0xb);
	EXPECT_EQ("(124)", v.toString(AB9));
	v.Clear();
	EXPECT_EQ("-", v.toString(AB9));
}

TEST_F(ValueSetTest, Clear_9) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	v.Clear();
	EXPECT_EQ("-", v.toString(AB9));
}

TEST_F(ValueSetTest, Clear_16) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	v.Clear();
	EXPECT_EQ("-", v.toString(AB16));
}

TEST_F(ValueSetTest, Add_4_1) {
	ValueSet v(4, 0xb);
	EXPECT_EQ("(124)", v.toString(AB9));
	v.Add(0x4); //3
	EXPECT_EQ("(1234)", v.toString(AB9));
}

TEST_F(ValueSetTest, Add_9_1) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	v.Add(0x8); //4
	EXPECT_EQ("(3458)", v.toString(AB9));
}

TEST_F(ValueSetTest, Add_16_1) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	v.Add(0x20); //5
	EXPECT_EQ("(157e)", v.toString(AB16));
}

TEST_F(ValueSetTest, Remove_4_1) {
	ValueSet v(4, 0xb);
	EXPECT_EQ("(124)", v.toString(AB9));
	v.Remove(0x2); //2
	EXPECT_EQ("(14)", v.toString(AB9));
}

TEST_F(ValueSetTest, Remove_9_1) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	v.Remove(0x10); //5
	EXPECT_EQ("(38)", v.toString(AB9));
}

TEST_F(ValueSetTest, Remove_16_1) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	v.Remove(0x2); //1
	EXPECT_EQ("(7e)", v.toString(AB16));
}

TEST_F(ValueSetTest, IdxAdd_4) {
	ValueSet v(4, 0xb);
	EXPECT_EQ("(124)", v.toString(AB9));
	v.IdxAdd(3);
	EXPECT_EQ("(1234)", v.toString(AB9));
}

TEST_F(ValueSetTest, IdxAdd_9) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	v.IdxAdd(4);
	EXPECT_EQ("(3458)", v.toString(AB9));
}

TEST_F(ValueSetTest, IdxAdd_16) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	v.IdxAdd(6); //5
	EXPECT_EQ("(157e)", v.toString(AB16));
}

TEST_F(ValueSetTest, IdxRemove_4) {
	ValueSet v(4, 0xb);
	EXPECT_EQ("(124)", v.toString(AB9));
	v.IdxRemove(2);
	EXPECT_EQ("(14)", v.toString(AB9));
}

TEST_F(ValueSetTest, IdxRemove_9) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	v.IdxRemove(5);
	EXPECT_EQ("(38)", v.toString(AB9));
}

TEST_F(ValueSetTest, IdxRemove_16) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	v.IdxRemove(2); //1
	EXPECT_EQ("(7e)", v.toString(AB16));
}

TEST_F(ValueSetTest, Contains_4_1) {
	ValueSet v(4, 0xb);
	EXPECT_EQ("(124)", v.toString(AB9));
	EXPECT_TRUE(v.Contains(0x2)); //2
	EXPECT_FALSE(v.Contains(0x4)); //3
}

TEST_F(ValueSetTest, Contains_9_1) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	EXPECT_TRUE(v.Contains(0x10)); //5
	EXPECT_FALSE(v.Contains(0x8)); //4
}

TEST_F(ValueSetTest, Contains_16_1) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	EXPECT_TRUE(v.Contains(0x2)); //1
	EXPECT_FALSE(v.Contains(0x20)); //5
}

TEST_F(ValueSetTest, Contains_4_2) {
	ValueSet v(4, 0xb);
	EXPECT_EQ("(124)", v.toString(AB9));
	EXPECT_TRUE(v.Contains(0xa)); //2,4
	//EXPECT_FALSE(v.Contains(0xc)); //3,4
	ValueSet w(4, 0xa);
	EXPECT_EQ("(24)", w.toString(AB9));
	EXPECT_FALSE(w.Contains(0x5)); //1,3
}

TEST_F(ValueSetTest, Contains_9_2) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	EXPECT_TRUE(v.Contains(0x90)); //5,8
	//EXPECT_FALSE(v.Contains(0x18)); //4,5
	EXPECT_FALSE(v.Contains(0x48)); //4,7
}

TEST_F(ValueSetTest, Contains_16_2) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	EXPECT_TRUE(v.Contains(0x82)); //1,7
	//EXPECT_FALSE(v.Contains(0xa0)); //5,7
	EXPECT_FALSE(v.Contains(0x120)); //5,8
}

TEST_F(ValueSetTest, Contains_other_4_2) {
	ValueSet v(4, 0xb);
	EXPECT_EQ("(124)", v.toString(AB9));
	ValueSet w(4, 0xa);
	EXPECT_EQ("(24)", w.toString(AB9));
	EXPECT_TRUE(v.Contains(w));
	ValueSet x(4, 0xc);
	EXPECT_EQ("(34)", x.toString(AB9));
	//EXPECT_FALSE(v.Contains(x));
	ValueSet y(4, 0x5);
	EXPECT_EQ("(13)", y.toString(AB9));
	EXPECT_FALSE(w.Contains(y));
}

TEST_F(ValueSetTest, Contains_other_9_2) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	ValueSet w(9, 0x90);
	EXPECT_EQ("(58)", w.toString(AB9));
	EXPECT_TRUE(v.Contains(w));
	ValueSet x(9, 0x18);
	EXPECT_EQ("(45)", x.toString(AB9));
	//EXPECT_FALSE(v.Contains(x));
	ValueSet y(9, 0x48);
	EXPECT_EQ("(47)", y.toString(AB9));
	EXPECT_FALSE(v.Contains(y));
}

TEST_F(ValueSetTest, Contains_other_16_2) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	ValueSet w(16, 0x82);
	EXPECT_EQ("(17)", w.toString(AB16));
	EXPECT_TRUE(v.Contains(w));
	ValueSet x(16, 0xa0);
	EXPECT_EQ("(57)", x.toString(AB16));
	//EXPECT_FALSE(v.Contains(x));
	ValueSet y(16, 0x120);
	EXPECT_EQ("(58)", y.toString(AB16));
	EXPECT_FALSE(v.Contains(y));
}

TEST_F(ValueSetTest, Count_4) {
	EXPECT_EQ(empty_4.Count(), 0);
	ValueSet v(4, 0xb);
	EXPECT_EQ("(124)", v.toString(AB9));
	EXPECT_EQ(v.Count(), 3);
	EXPECT_EQ(full_4.Count(), 4);
}

TEST_F(ValueSetTest, Count_9) {
	EXPECT_EQ(empty_4.Count(), 0);
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	EXPECT_EQ(v.Count(), 3);
	EXPECT_EQ(full_9.Count(), 9);
}

TEST_F(ValueSetTest, Count_16) {
	EXPECT_EQ(empty_16.Count(), 0);
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	EXPECT_EQ(v.Count(), 3);
	EXPECT_EQ(full_16.Count(), 16);
}

TEST_F(ValueSetTest, Fixed_4) {
	EXPECT_FALSE(empty_4.Fixed());
	ValueSet v(4, 0x2);
	EXPECT_EQ("2", v.toString(AB9));
	EXPECT_TRUE(v.Fixed());
	ValueSet w(4, 0xb);
	EXPECT_EQ("(124)", w.toString(AB9));
	EXPECT_FALSE(w.Fixed());
	EXPECT_FALSE(full_4.Fixed());
}

TEST_F(ValueSetTest, Fixed_9) {
	EXPECT_FALSE(empty_9.Fixed());
	ValueSet v(9, 0x4);
	EXPECT_EQ("3", v.toString(AB9));
	EXPECT_TRUE(v.Fixed());
	ValueSet w(9, 0x94);
	EXPECT_EQ("(358)", w.toString(AB9));
	EXPECT_FALSE(w.Fixed());
	EXPECT_FALSE(full_9.Fixed());
}

TEST_F(ValueSetTest, Fixed_16) {
	EXPECT_FALSE(empty_16.Fixed());
	ValueSet v(16, 0x80);
	EXPECT_EQ("7", v.toString(AB16));
	EXPECT_TRUE(v.Fixed());
	ValueSet w(16, 0x4082);
	EXPECT_EQ("(17e)", w.toString(AB16));
	EXPECT_FALSE(w.Fixed());
	EXPECT_FALSE(full_16.Fixed());
}

TEST_F(ValueSetTest, Empty_4) {
	EXPECT_TRUE(empty_4.Empty());
	ValueSet v(4, 0xb);
	EXPECT_EQ("(124)", v.toString(AB9));
	EXPECT_FALSE(v.Empty());
	v.Clear();
	EXPECT_EQ("-", v.toString(AB9));
	EXPECT_TRUE(v.Empty());
	EXPECT_FALSE(full_4.Empty());
}

TEST_F(ValueSetTest, Empty_9) {
	EXPECT_TRUE(empty_9.Empty());
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	EXPECT_FALSE(v.Empty());
	v.Clear();
	EXPECT_EQ("-", v.toString(AB9));
	EXPECT_TRUE(v.Empty());
	EXPECT_FALSE(full_9.Empty());
}

TEST_F(ValueSetTest, Empty_16) {
	EXPECT_TRUE(empty_16.Empty());
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	EXPECT_FALSE(v.Empty());
	v.Clear();
	EXPECT_EQ("-", v.toString(AB16));
	EXPECT_TRUE(v.Empty());
	EXPECT_FALSE(full_16.Empty());
}

TEST_F(ValueSetTest, Index_4) {
	//EXPECT_EQ(empty_4.Index(), 0);
	ValueSet v(4, 0x2);
	EXPECT_EQ("2", v.toString(AB9));
	EXPECT_EQ(v.Index(), 1);
	ValueSet w(4, 0x7);
	EXPECT_EQ("(123)", w.toString(AB9));
	EXPECT_EQ(w.Index(), 2);
	EXPECT_EQ(full_4.Index(), 3);
}

TEST_F(ValueSetTest, Index_9) {
	//EXPECT_EQ(empty_9.Index(), 0);
	ValueSet v(9, 0x4);
	EXPECT_EQ("3", v.toString(AB9));
	EXPECT_EQ(v.Index(), 2);
	ValueSet w(9, 0x94);
	EXPECT_EQ("(358)", w.toString(AB9));
	EXPECT_EQ(w.Index(), 7);
	EXPECT_EQ(full_9.Index(), 8);
}

TEST_F(ValueSetTest, Index_16) {
	//EXPECT_EQ(empty_16.Index(), 0);
	ValueSet v(16, 0x80);
	EXPECT_EQ("7", v.toString(AB16));
	EXPECT_EQ(v.Index(), 7);
	ValueSet w(16, 0x4082);
	EXPECT_EQ("(17e)", w.toString(AB16));
	EXPECT_EQ(w.Index(), 14);
	EXPECT_EQ(full_16.Index(), 15);
}

TEST_F(ValueSetTest, Highest_4) {
	EXPECT_EQ(empty_4.Highest(), 0);
	ValueSet v(4, 0x2);
	EXPECT_EQ("2", v.toString(AB9));
	EXPECT_EQ(v.Highest(), 2);
	ValueSet w(4, 0x7);
	EXPECT_EQ("(123)", w.toString(AB9));
	EXPECT_EQ(w.Highest(), 3);
	EXPECT_EQ(full_4.Highest(), 4);
}

TEST_F(ValueSetTest, Highest_9) {
	EXPECT_EQ(empty_9.Highest(), 0);
	ValueSet v(9, 0x4);
	EXPECT_EQ("3", v.toString(AB9));
	EXPECT_EQ(v.Highest(), 3);
	ValueSet w(9, 0x94);
	EXPECT_EQ("(358)", w.toString(AB9));
	EXPECT_EQ(w.Highest(), 8);
	EXPECT_EQ(full_9.Highest(), 9);
}

TEST_F(ValueSetTest, Highest_16) {
	EXPECT_EQ(empty_16.Highest(), 0);
	ValueSet v(16, 0x80);
	EXPECT_EQ("7", v.toString(AB16));
	EXPECT_EQ(v.Highest(), 8);
	ValueSet w(16, 0x4082);
	EXPECT_EQ("(17e)", w.toString(AB16));
	EXPECT_EQ(w.Highest(), 15);
	EXPECT_EQ(full_16.Highest(), 16);
}

TEST_F(ValueSetTest, Lowest_4) {
	EXPECT_EQ(empty_4.Lowest(), 0);
	ValueSet v(4, 0x2);
	EXPECT_EQ("2", v.toString(AB9));
	EXPECT_EQ(v.Lowest(), 2);
	ValueSet w(4, 0x7);
	EXPECT_EQ("(123)", w.toString(AB9));
	EXPECT_EQ(w.Lowest(), 1);
	EXPECT_EQ(full_4.Lowest(), 1);
}

TEST_F(ValueSetTest, Lowest_9) {
	EXPECT_EQ(empty_9.Lowest(), 0);
	ValueSet v(9, 0x4);
	EXPECT_EQ("3", v.toString(AB9));
	EXPECT_EQ(v.Lowest(), 3);
	ValueSet w(9, 0x94);
	EXPECT_EQ("(358)", w.toString(AB9));
	EXPECT_EQ(w.Lowest(), 3);
	EXPECT_EQ(full_9.Lowest(), 1);
}

TEST_F(ValueSetTest, Lowest_16) {
	EXPECT_EQ(empty_16.Lowest(), 0);
	ValueSet v(16, 0x80);
	EXPECT_EQ("7", v.toString(AB16));
	EXPECT_EQ(v.Lowest(), 8);
	ValueSet w(16, 0x4082);
	EXPECT_EQ("(17e)", w.toString(AB16));
	EXPECT_EQ(w.Lowest(), 2);
	EXPECT_EQ(full_16.Lowest(), 1);
}

TEST_F(ValueSetTest, Union_4) {
	ValueSet v(4, 0x7);
	EXPECT_EQ("(123)", v.toString(AB9));
	ValueSet ue = v.Union(empty_4);
	EXPECT_EQ(ue, v);
	ValueSet uf = v.Union(full_4);
	EXPECT_EQ(uf, full_4);
	ValueSet w(4, 0x3);
	EXPECT_EQ("(12)", w.toString(AB9));
	ValueSet vw = v.Union(w);
	EXPECT_EQ(vw, v);
	ValueSet x(4, 0x8);
	EXPECT_EQ("4", x.toString(AB9));
	ValueSet vx = v.Union(x);
	EXPECT_EQ(vx, full_4);
}

TEST_F(ValueSetTest, Union_9) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	ValueSet ue = v.Union(empty_9);
	EXPECT_EQ(ue, v);
	ValueSet uf = v.Union(full_9);
	EXPECT_EQ(uf, full_9);
	ValueSet w(9, 0x84);
	EXPECT_EQ("(38)", w.toString(AB9));
	ValueSet vw = v.Union(w);
	EXPECT_EQ(vw, v);
	ValueSet x(9, 0x2);
	EXPECT_EQ("2", x.toString(AB9));
	ValueSet vx = v.Union(x);
	ValueSet vx2(9, 0x96);
	EXPECT_EQ("(2358)", vx2.toString(AB9));
	EXPECT_EQ(vx, vx2);
}

TEST_F(ValueSetTest, Union_16) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	ValueSet ue = v.Union(empty_16);
	EXPECT_EQ(ue, v);
	ValueSet uf = v.Union(full_16);
	EXPECT_EQ(uf, full_16);
	ValueSet w(16, 0x4002);
	EXPECT_EQ("(1e)", w.toString(AB16));
	ValueSet vw = v.Union(w);
	EXPECT_EQ(vw, v);
	ValueSet x(16, 0x200);
	EXPECT_EQ("9", x.toString(AB16));
	ValueSet vx = v.Union(x);
	ValueSet vx2(16, 0x4282);
	EXPECT_EQ("(179e)", vx2.toString(AB16));
	EXPECT_EQ(vx, vx2);
}

TEST_F(ValueSetTest, Intersection_4) {
	ValueSet v(4, 0x7);
	EXPECT_EQ("(123)", v.toString(AB9));
	ValueSet ue = v.Intersection(empty_4);
	EXPECT_EQ(ue, empty_4);
	ValueSet uf = v.Intersection(full_4);
	EXPECT_EQ(uf, v);
	ValueSet w(4, 0x3);
	EXPECT_EQ("(12)", w.toString(AB9));
	ValueSet vw = v.Intersection(w);
	EXPECT_EQ(vw, w);
	ValueSet x(4, 0x8);
	EXPECT_EQ("4", x.toString(AB9));
	ValueSet vx = v.Intersection(x);
	EXPECT_EQ(vx, empty_4);
}

TEST_F(ValueSetTest, Intersection_9) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	ValueSet ue = v.Intersection(empty_9);
	EXPECT_EQ(ue, empty_9);
	ValueSet uf = v.Intersection(full_9);
	EXPECT_EQ(uf, v);
	ValueSet w(9, 0x84);
	EXPECT_EQ("(38)", w.toString(AB9));
	ValueSet vw = v.Intersection(w);
	EXPECT_EQ(vw, w);
	ValueSet x(9, 0x1ff & ~0x94);
	EXPECT_EQ("(124679)", x.toString(AB9));
	ValueSet vx = v.Intersection(x);
	EXPECT_EQ(vx, empty_9);
}

TEST_F(ValueSetTest, Intersection_16) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	ValueSet ue = v.Intersection(empty_16);
	EXPECT_EQ(ue, empty_16);
	ValueSet uf = v.Intersection(full_16);
	EXPECT_EQ(uf, v);
	ValueSet w(16, 0x4002);
	EXPECT_EQ("(1e)", w.toString(AB16));
	ValueSet vw = v.Intersection(w);
	EXPECT_EQ(vw, w);
	ValueSet x(16, 0xffff & ~0x4082);
	EXPECT_EQ("(02345689abcdf)", x.toString(AB16));
	ValueSet vx = v.Intersection(x);
	EXPECT_EQ(vx, empty_16);
}

TEST_F(ValueSetTest, Difference_4) {
	ValueSet v(4, 0x7);
	EXPECT_EQ("(123)", v.toString(AB9));
	ValueSet ue = v.Difference(empty_4);
	EXPECT_EQ(ue, v);
	ValueSet uf = v.Difference(full_4);
	EXPECT_EQ(uf, empty_4);
	ValueSet w(4, 0x3);
	EXPECT_EQ("(12)", w.toString(AB9));
	ValueSet vw  = v.Difference(w);
	ValueSet vw2(4, 0x4);
	EXPECT_EQ("3", vw2.toString(AB9));
	EXPECT_EQ(vw, vw2);
	ValueSet x(4, 0x8);
	EXPECT_EQ("4", x.toString(AB9));
	ValueSet vx = v.Difference(x);
	EXPECT_EQ(vx, v);
}

TEST_F(ValueSetTest, Difference_9) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	ValueSet ue = v.Difference(empty_9);
	EXPECT_EQ(ue, v);
	ValueSet uf = v.Difference(full_9);
	EXPECT_EQ(uf, empty_9);
	ValueSet w(9, 0x84);
	EXPECT_EQ("(38)", w.toString(AB9));
	ValueSet vw = v.Difference(w);
	ValueSet vw2(9, 0x10);
	EXPECT_EQ("5", vw2.toString(AB9));
	EXPECT_EQ(vw, vw2);
	ValueSet x(9, 0x1ff & ~0x94);
	EXPECT_EQ("(124679)", x.toString(AB9));
	ValueSet vx = v.Difference(x);
	EXPECT_EQ(vx, v);
}

TEST_F(ValueSetTest, Difference_16) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	ValueSet ue = v.Difference(empty_16);
	EXPECT_EQ(ue, v);
	ValueSet uf = v.Difference(full_16);
	EXPECT_EQ(uf, empty_16);
	ValueSet w(16, 0x4002);
	EXPECT_EQ("(1e)", w.toString(AB16));
	ValueSet vw = v.Difference(w);
	ValueSet vw2(16, 0x80);
	EXPECT_EQ("7", vw2.toString(AB16));
	EXPECT_EQ(vw, vw2);
	ValueSet x(16, 0xffff & ~0x4082);
	EXPECT_EQ("(02345689abcdf)", x.toString(AB16));
	ValueSet vx = v.Difference(x);
	EXPECT_EQ(vx, v);
}

TEST_F(ValueSetTest, Complement_4) {
	EXPECT_EQ(empty_4, full_4.Complement());
	EXPECT_EQ(full_4, empty_4.Complement());
	ValueSet v(4, 0x7);
	EXPECT_EQ("(123)", v.toString(AB9));
	ValueSet w(4, 0xf & ~0x7);
	EXPECT_EQ("4", w.toString(AB9));
	EXPECT_EQ(w, v.Complement());
}

TEST_F(ValueSetTest, Complement_9) {
	EXPECT_EQ(empty_9, full_9.Complement());
	EXPECT_EQ(full_9, empty_9.Complement());
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	ValueSet w(9, 0x1ff & ~0x94);
	EXPECT_EQ("(124679)", w.toString(AB9));
	EXPECT_EQ(w, v.Complement());
}

TEST_F(ValueSetTest, Complement_16) {
	EXPECT_EQ(empty_16, full_16.Complement());
	EXPECT_EQ(full_16, empty_16.Complement());
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	ValueSet w(16, 0xffff & ~0x4082);
	EXPECT_EQ("(02345689abcdf)", w.toString(AB16));
	EXPECT_EQ(w, v.Complement());
}

// Testing of operator+, operator^, operator-, operator~ seems unnecessary, as they simply call
// Union, Intersection, Difference and Complement

TEST_F(ValueSetTest, op_union_assign_4) {
	ValueSet v(4, 0x7);
	EXPECT_EQ("(123)", v.toString(AB9));
	v += empty_4;
	ValueSet v2(4, 0x7);
	EXPECT_EQ("(123)", v2.toString(AB9));
	EXPECT_EQ(v, v2);
	ValueSet w(4, 0x7);
	EXPECT_EQ("(123)", w.toString(AB9));
	w += full_4;
	EXPECT_EQ(w, full_4);
	ValueSet x(4, 0x3);
	EXPECT_EQ("(12)", x.toString(AB9));
	v += x;
	EXPECT_EQ(v, v2);
	ValueSet y(4, 0x8);
	EXPECT_EQ("4", y.toString(AB9));
	v += y;
	EXPECT_EQ(v, full_4);
}

TEST_F(ValueSetTest, op_union_assign_9) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	v += empty_9;
	ValueSet v2(9, 0x94);
	EXPECT_EQ("(358)", v2.toString(AB9));
	EXPECT_EQ(v, v2);
	ValueSet w(9, 0x94);
	EXPECT_EQ("(358)", w.toString(AB9));
	w += full_9;
	EXPECT_EQ(w, full_9);
	ValueSet x(9, 0x84);
	EXPECT_EQ("(38)", x.toString(AB9));
	v += x;
	EXPECT_EQ(v, v2);
	ValueSet y(9, 0x2);
	EXPECT_EQ("2", y.toString(AB9));
	v += y;
	ValueSet y2(9, 0x96);
	EXPECT_EQ("(2358)", y2.toString(AB9));
	EXPECT_EQ(v, y2);
}

TEST_F(ValueSetTest, op_union_assign_16) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	v += empty_16;
	ValueSet v2(16, 0x4082);
	EXPECT_EQ("(17e)", v2.toString(AB16));
	EXPECT_EQ(v, v2);
	ValueSet w(16, 0x4082);
	EXPECT_EQ("(17e)", w.toString(AB16));
	w += full_16;
	EXPECT_EQ(w, full_16);
	ValueSet x(16, 0x4002);
	EXPECT_EQ("(1e)", x.toString(AB16));
	v += x;
	EXPECT_EQ(v, v2);
	ValueSet y(16, 0x200);
	EXPECT_EQ("9", y.toString(AB16));
	v += y;
	ValueSet y2(16, 0x4282);
	EXPECT_EQ("(179e)", y2.toString(AB16));
	EXPECT_EQ(v, y2);
}

TEST_F(ValueSetTest, op_intersection_assign_4) {
	ValueSet v(4, 0x7);
	EXPECT_EQ("(123)", v.toString(AB9));
	v ^= empty_4;
	EXPECT_EQ(v, empty_4);
	ValueSet v2(4, 0x7);
	EXPECT_EQ("(123)", v2.toString(AB9));
	v2 ^= full_4;
	ValueSet w(4, 0x7);
	EXPECT_EQ("(123)", w.toString(AB9));
	EXPECT_EQ(v2, w);
	ValueSet v3(4, 0x7);
	EXPECT_EQ("(123)", v3.toString(AB9));
	ValueSet x(4, 0x3);
	EXPECT_EQ("(12)", x.toString(AB9));
	v3 ^= x;
	EXPECT_EQ(v3, x);
	ValueSet v4(4, 0x7);
	EXPECT_EQ("(123)", v4.toString(AB9));
	ValueSet y(4, 0x8);
	EXPECT_EQ("4", y.toString(AB9));
	v4 ^= y;
	EXPECT_EQ(v4, empty_4);
}

TEST_F(ValueSetTest, op_intersection_assign_9) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	v ^= empty_9;
	EXPECT_EQ(v, empty_9);
	ValueSet v2(9, 0x94);
	EXPECT_EQ("(358)", v2.toString(AB9));
	v2 ^= full_9;
	ValueSet w(9, 0x94);
	EXPECT_EQ("(358)", w.toString(AB9));
	EXPECT_EQ(v2, w);
	ValueSet v3(9, 0x94);
	EXPECT_EQ("(358)", v3.toString(AB9));
	ValueSet x(9, 0x84);
	EXPECT_EQ("(38)", x.toString(AB9));
	v3 ^= x;
	EXPECT_EQ(v3, x);
	ValueSet v4(9, 0x94);
	EXPECT_EQ("(358)", v4.toString(AB9));
	ValueSet y(9, 0x2);
	EXPECT_EQ("2", y.toString(AB9));
	v4 ^= y;
	EXPECT_EQ(v4, empty_9);
}

TEST_F(ValueSetTest, op_intersection_assign_16) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	v ^= empty_16;
	EXPECT_EQ(v, empty_16);
	ValueSet v2(16, 0x4082);
	EXPECT_EQ("(17e)", v2.toString(AB16));
	v2 ^= full_16;
	ValueSet w(16, 0x4082);
	EXPECT_EQ("(17e)", w.toString(AB16));
	EXPECT_EQ(v2, w);
	ValueSet v3(16, 0x4082);
	EXPECT_EQ("(17e)", v3.toString(AB16));
	ValueSet x(16, 0x4002);
	EXPECT_EQ("(1e)", x.toString(AB16));
	v3 ^= x;
	EXPECT_EQ(v3, x);
	ValueSet v4(16, 0x4082);
	EXPECT_EQ("(17e)", v4.toString(AB16));
	ValueSet y(16, 0x200);
	EXPECT_EQ("9", y.toString(AB16));
	v4 ^= y;
	EXPECT_EQ(v4, empty_16);
}

TEST_F(ValueSetTest, op_difference_assign_4) {
	ValueSet v(4, 0x7);
	EXPECT_EQ("(123)", v.toString(AB9));
	v -= empty_4;
	ValueSet w(4, 0x7);
	EXPECT_EQ("(123)", w.toString(AB9));
	EXPECT_EQ(v, w);
	ValueSet v2(4, 0x7);
	EXPECT_EQ("(123)", v2.toString(AB9));
	v2 -= full_4;
	EXPECT_EQ(v2, empty_4);
	ValueSet v3(4, 0x7);
	EXPECT_EQ("(123)", v3.toString(AB9));
	ValueSet x(4, 0x3);
	EXPECT_EQ("(12)", x.toString(AB9));
	v3 -= x;
	ValueSet y(4, 0x4);
	EXPECT_EQ("3", y.toString(AB9));
	EXPECT_EQ(v3, y);
	ValueSet v4(4, 0x7);
	EXPECT_EQ("(123)", v4.toString(AB9));
	ValueSet z(4, 0x8);
	EXPECT_EQ("4", z.toString(AB9));
	v4 -= z;
	ValueSet a(4, 0x7);
	EXPECT_EQ("(123)", a.toString(AB9));
	EXPECT_EQ(v4, a);
}

TEST_F(ValueSetTest, op_difference_assign_9) {
	ValueSet v(9, 0x94);
	EXPECT_EQ("(358)", v.toString(AB9));
	v -= empty_9;
	ValueSet w(9, 0x94);
	EXPECT_EQ("(358)", w.toString(AB9));
	EXPECT_EQ(v, w);
	ValueSet v2(9, 0x94);
	EXPECT_EQ("(358)", v2.toString(AB9));
	v2 -= full_9;
	EXPECT_EQ(v2, empty_9);
	ValueSet v3(9, 0x94);
	EXPECT_EQ("(358)", v3.toString(AB9));
	ValueSet x(9, 0x84);
	EXPECT_EQ("(38)", x.toString(AB9));
	v3 -= x;
	ValueSet y(9, 0x10);
	EXPECT_EQ("5", y.toString(AB9));
	EXPECT_EQ(v3, y);
	ValueSet v4(9, 0x94);
	EXPECT_EQ("(358)", v4.toString(AB9));
	ValueSet z(9, 0x1ff & ~0x94);
	EXPECT_EQ("(124679)", z.toString(AB9));
	v4 -= z;
	ValueSet a(9, 0x94);
	EXPECT_EQ("(358)", a.toString(AB9));
	EXPECT_EQ(v4, a);
}

TEST_F(ValueSetTest, op_difference_assign_16) {
	ValueSet v(16, 0x4082);
	EXPECT_EQ("(17e)", v.toString(AB16));
	v -= empty_16;
	ValueSet w(16, 0x4082);
	EXPECT_EQ("(17e)", w.toString(AB16));
	EXPECT_EQ(v, w);
	ValueSet v2(16, 0x4082);
	EXPECT_EQ("(17e)", v2.toString(AB16));
	v2 -= full_16;
	EXPECT_EQ(v2, empty_16);
	ValueSet v3(16, 0x4082);
	EXPECT_EQ("(17e)", v3.toString(AB16));
	ValueSet x(16, 0x4002);
	EXPECT_EQ("(1e)", x.toString(AB16));
	v3 -= x;
	ValueSet y(16, 0x80);
	EXPECT_EQ("7", y.toString(AB16));
	EXPECT_EQ(v3, y);
	ValueSet v4(16, 0x4082);
	EXPECT_EQ("(17e)", v4.toString(AB16));
	ValueSet z(16, 0xffff & ~0x4082);
	EXPECT_EQ("(02345689abcdf)", z.toString(AB16));
	v4 -= z;
	ValueSet a(16, 0x4082);
	EXPECT_EQ("(17e)", a.toString(AB16));
	EXPECT_EQ(v4, a);
}

// Testing of operator<<= and operator== do not seem worthwhile,
// as these are a direct mapping of <<= and == on the bitmap(s)

