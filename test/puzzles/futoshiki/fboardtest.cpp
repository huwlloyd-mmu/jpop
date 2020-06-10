#include "gtest/gtest.h"
#include "../../../src/puzzles/futoshiki/fboard.h"

class FBoardTest : public ::testing::Test {
protected:
	FBoardTest() {}
	~FBoardTest() override {}
	void SetUp() override {}
	void TearDown() override {}

	string FB4 = "5;............1............;8;1>2;6>5;5>10;11>6;14>13;15>10;18>19;16>21;";
	string FB7 = "7;6.......5.2...5.........................4.....5..;16;5>6;2>9;11>4;12>13;18>17;16>23;17>24;25>24;28>21;32>25;33>26;30>31;31>38;39>40;42>43;43>44;";
	string FB9 = "9;........5...95....2..................................7....3.9........4...........;37;3>4;7>8;10>1;2>11;17>16;18>9;22>13;15>24;22>21;24>23;20>29;23>32;24>33;27>28;33>34;36>27;44>35;38>37;42>41;46>37;47>46;48>47;49>48;49>50;50>51;53>52;55>46;62>53;54>63;56>65;57>66;67>68;74>65;80>71;75>74;78>77;79>78;";
	string AS4 = " 1   3 > 2   4   5  \n                  \n 3 < 4   5   1   2  \n v   ^            \n 2   5   1   3 < 4  \n ^                \n 4   2   3   5 > 1  \n     v            \n 5   1   4   2   3  ";
	string AS7 = " 6   .   .   .   .   . > .  \n         v       ^        \n .   5   .   2   .   . > .  \n                          \n 5   .   .   . < .   .   .  \n         v   v            \n .   .   .   . < .   .   .  \n ^               ^   ^    \n .   .   . > .   .   .   .  \n             v            \n .   .   .   .   . > 4   .  \n                          \n . > . > .   .   5   .   .  ";
	string AS9 = " .   .   .   . > .   .   .   . > 5  \n     ^   v                        \n 1   .   .   9   5   .   .   . < .  \n ^               ^       v        \n 2   .   .   . < .   . < .   .   .  \n         v           v   v        \n . > .   .   .   .   .   . > .   .  \n ^                               ^\n .   . < .   .   .   . < .   .   .  \n     ^                            \n .   . < . < . < . > . > .   . < 7  \n     ^                           ^\n .   .   .   .   3   .   9   .   8  \n v       v   v                    \n .   .   .   .   . > .   4   .   .  \n         ^                       ^\n .   .   . < .   .   . < . < .   .  ";
};

TEST_F(FBoardTest, Constructor_PuzzleString_4) {
	FBoard board(FB4, false);
	EXPECT_EQ(board.AsString(), AS4);
	cout << AS4 << endl;
	cout << AS7 << endl;
	cout << AS9 << endl;
}

TEST_F(FBoardTest, Constructor_PuzzleString_7) {
	FBoard board(FB7, false);
	EXPECT_EQ(board.AsString(), AS7);
}

TEST_F(FBoardTest, Constructor_PuzzleString_9) {
	FBoard board(FB9, false);
	EXPECT_EQ(board.AsString(), AS9);
}

TEST_F(FBoardTest, Constructor_File_4) {
	FBoard board("instances/futoshiki/janko191_5.txt", true);
	EXPECT_EQ(board.AsString(), AS4);
}

TEST_F(FBoardTest, Constructor_File_7) {
	FBoard board("instances/futoshiki/janko195_7.txt", true);
	EXPECT_EQ(board.AsString(), AS7);
}

TEST_F(FBoardTest, Constructor_File_9) {
	FBoard board("instances/futoshiki/janko200_9.txt", true);
	EXPECT_EQ(board.AsString(), AS9);
}
