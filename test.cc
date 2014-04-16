#include "gtest/gtest.h"
#include "src/codepoint_iterator.h"

#include <string>

struct SampleText {
	std::string text;
	size_t length;
	std::u32string codepoints;
};

class CodepointIteratorTest : public ::testing::Test {
	protected:
		virtual void SetUp() {
			SampleText tmp;

			tmp.text       = u8"Hellø Uni¢od€!";
			tmp.codepoints =  U"Hellø Uni¢od€!";
			this->sample_.push_back(tmp);

			tmp.text       = u8"ᛖᚴ ᚷᛖᛏ ᛖᛏᛁ ᚧ ᚷᛚᛖᚱ ᛘᚾ ᚦᛖᛋᛋ ᚨᚧ ᚡᛖ ᚱᚧᚨ ᛋᚨᚱ";
			tmp.codepoints =  U"ᛖᚴ ᚷᛖᛏ ᛖᛏᛁ ᚧ ᚷᛚᛖᚱ ᛘᚾ ᚦᛖᛋᛋ ᚨᚧ ᚡᛖ ᚱᚧᚨ ᛋᚨᚱ";
			this->sample_.push_back(tmp);

			tmp.text       = u8"⠊⠀⠉⠁⠝⠀⠑⠁⠞⠀⠛⠇⠁⠎⠎⠀⠁⠝⠙⠀⠊⠞⠀⠙⠕⠑⠎⠝⠞⠀⠓⠥⠗⠞⠀⠍⠑";
			tmp.codepoints =  U"⠊⠀⠉⠁⠝⠀⠑⠁⠞⠀⠛⠇⠁⠎⠎⠀⠁⠝⠙⠀⠊⠞⠀⠙⠕⠑⠎⠝⠞⠀⠓⠥⠗⠞⠀⠍⠑";
			this->sample_.push_back(tmp);

			tmp.text       = u8"Ég get etið gler án þess að meiða mig";
			tmp.codepoints =  U"Ég get etið gler án þess að meiða mig";
			this->sample_.push_back(tmp);

			tmp.text       = u8"جام ييه بلورم بڭا ضررى طوقونمز";
			tmp.codepoints =  U"جام ييه بلورم بڭا ضررى طوقونمز";
			this->sample_.push_back(tmp);

			tmp.text       = u8"මට වීදුරු කෑමට හැකියි. එයින් මට කිසි හානියක් සිදු නොවේ";
			tmp.codepoints =  U"මට වීදුරු කෑමට හැකියි. එයින් මට කිසි හානියක් සිදු නොවේ";
			this->sample_.push_back(tmp);
		}

	std::vector<SampleText> sample_;
};

TEST_F(CodepointIteratorTest, ForwardIteration) {
	for ( auto&& tmp : this->sample_ ) {
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
	for ( auto&& tmp : this->sample_ ) {
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
	for ( auto&& tmp : this->sample_ ) {
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
