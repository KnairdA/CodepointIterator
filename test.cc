#include "gtest/gtest.h"
#include "src/codepoint_iterator.h"

#include <string>

struct SampleText {
	std::string text;
	size_t length;
	std::vector<char32_t> codepoints;
};

class CodepointIteratorTest : public ::testing::Test {
	protected:
		virtual void SetUp() {
			SampleText tmp;

			tmp.text       = u8"Hellø Uni¢od€!";
			tmp.codepoints = { 72, 101, 108, 108, 248, 32, 85, 110, 105, 162, 111, 100, 8364, 33 };
			this->sample_.push_back(tmp);

			tmp.text       = u8"ᛖᚴ ᚷᛖᛏ ᛖᛏᛁ ᚧ ᚷᛚᛖᚱ ᛘᚾ ᚦᛖᛋᛋ ᚨᚧ ᚡᛖ ᚱᚧᚨ ᛋᚨᚱ";
			tmp.codepoints = { 5846, 5812, 32, 5815, 5846, 5839, 32, 5846, 5839, 5825, 32, 5799, 32, 5815, 5850, 5846, 5809, 32, 5848, 5822, 32, 5798, 5846, 5835, 5835, 32, 5800, 5799, 32, 5793, 5846, 32, 5809, 5799, 5800, 32, 5835, 5800, 5809 };
			this->sample_.push_back(tmp);

			tmp.text       = u8"⠊⠀⠉⠁⠝⠀⠑⠁⠞⠀⠛⠇⠁⠎⠎⠀⠁⠝⠙⠀⠊⠞⠀⠙⠕⠑⠎⠝⠞⠀⠓⠥⠗⠞⠀⠍⠑";
			tmp.codepoints = { 10250, 10240, 10249, 10241, 10269, 10240, 10257, 10241, 10270, 10240, 10267, 10247, 10241, 10254, 10254, 10240, 10241, 10269, 10265, 10240, 10250, 10270, 10240, 10265, 10261, 10257, 10254, 10269, 10270, 10240, 10259, 10277, 10263, 10270, 10240, 10253, 10257 };
			this->sample_.push_back(tmp);

			tmp.text       = u8"Ég get etið gler án þess að meiða mig";
			tmp.codepoints = { 201, 103, 32, 103, 101, 116, 32, 101, 116, 105, 240, 32, 103, 108, 101, 114, 32, 225, 110, 32, 254, 101, 115, 115, 32, 97, 240, 32, 109, 101, 105, 240, 97, 32, 109, 105, 103 };
			this->sample_.push_back(tmp);

			tmp.text       = u8"جام ييه بلورم بڭا ضررى طوقونمز";
			tmp.codepoints = { 1580, 1575, 1605, 32, 1610, 1610, 1607, 32, 1576, 1604, 1608, 1585, 1605, 32, 1576, 1709, 1575, 32, 1590, 1585, 1585, 1609, 32, 1591, 1608, 1602, 1608, 1606, 1605, 1586 };
			this->sample_.push_back(tmp);

			tmp.text       = u8"මට වීදුරු කෑමට හැකියි. එයින් මට කිසි හානියක් සිදු නොවේ";
			tmp.codepoints = { 3512, 3495, 32, 3520, 3539, 3503, 3540, 3515, 3540, 32, 3482, 3537, 3512, 3495, 32, 3524, 3536, 3482, 3538, 3514, 3538, 46, 32, 3473, 3514, 3538, 3505, 3530, 32, 3512, 3495, 32, 3482, 3538, 3523, 3538, 32, 3524, 3535, 3505, 3538, 3514, 3482, 3530, 32, 3523, 3538, 3503, 3540, 32, 3505, 3548, 3520, 3546 };
			this->sample_.push_back(tmp);
		}

	std::vector<SampleText> sample_;
};

TEST_F(CodepointIteratorTest, ForwardIteration) {
	for ( auto tmp : this->sample_ ) {
		size_t length = 0;

		for ( UTF8::CodepointIterator iter(tmp.text.cbegin());
		      iter != tmp.text.cend();
		      ++iter ) {
			length++;
		}

		EXPECT_EQ(tmp.codepoints.size(), length);
	}
}

TEST_F(CodepointIteratorTest, ReverseIteration) {
	for ( auto tmp : this->sample_ ) {
		size_t length = 0;

		for ( UTF8::CodepointIterator iter(tmp.text.cend());
		      iter != tmp.text.cbegin();
		      --iter ) {
			length++;
		}

		EXPECT_EQ(tmp.codepoints.size(), length);
	}
}

TEST_F(CodepointIteratorTest, Dereferencing) {
	for ( auto tmp : this->sample_ ) {
		size_t index = 0;

		for ( UTF8::CodepointIterator iter(tmp.text.cbegin());
			  iter != tmp.text.cend();
			  ++iter ) {
			EXPECT_EQ(tmp.codepoints[index], *iter);

			++index;
		}
	}
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
