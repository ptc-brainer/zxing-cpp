/*
* Copyright 2021 gitlost
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "BitArray.h"
#include "BitArrayUtility.h"
#include "oned/rss/ODRSSExpandedBinaryDecoder.h"

#include "gtest/gtest.h"

using namespace ZXing;

static std::string parse(std::string bitStr)
{
	return OneD::DataBar::DecodeExpandedBits(Utility::ParseBitArray(bitStr, '1'));
}

TEST(ODRSSExpandedBinaryDecoderTest, FNC1NumericLatch)
{
	std::string result;

	// Correctly encoded (Alphanumeric FNC1 "01111" implying numeric latch)
	result = parse("0000000100110010101000010000001111011011000111110100001000000100");
	EXPECT_EQ(result, "(10)12A(422)123");

	// Incorrectly encoded (Alphanumeric FNC1 "01111" followed by numeric latch "000")
	result = parse("0000000100110010101000010000001111000011011000111110100001000000100");
	EXPECT_EQ(result, "(10)12A(422)123");

	// Correctly encoded (ISO646 FNC1 "01111" implying numeric latch)
	result = parse("0001000100110010101000000100111011010111101101100011111010000100000010000100");
	EXPECT_EQ(result, "(10)12((422)123");

	// Incorrectly encoded (ISO646 FNC1 "01111" followed by numeric latch "000")
	result = parse("0001000100110010101000000100111011010111100001101100011111010000100000010000100");
	EXPECT_EQ(result, "(10)12((422)123");
}

TEST(ODRSSExpandedBinaryDecoderTest, DecodeAI01392x)
{
	std::string result;

	// Parse AIs following AI01392x
	result = parse("00110000000000000100111010101000110111110111101000001110100111100001001");
	EXPECT_EQ(result, "(01)90012345678908(3920)1(20)01");

	result = parse("0011000000000000010011101010100011011111011110101010111101000100111100000010000010");
	EXPECT_EQ(result, "(01)90012345678908(3922)7955(20)01");

	result = parse("0100100100000000010011101010100011011111011110100110010010011100101010101101100010110111011101011001"
					"01010101101111100000010000011101");
	EXPECT_EQ(result, "(01)90012345678908(3929)12345678901234(20)01");
}

TEST(ODRSSExpandedBinaryDecoderTest, DecodeAI01393x)
{
	std::string result;

	// Parse AIs following AI01393x
	result = parse("0011010000000000010011101010100011011111011110100000000011000011101001111000010010010011000010000010"
					"000100110");
	EXPECT_EQ(result, "(01)90012345678908(3930)0121(20)01(10)AB1");

	result = parse("0011010000000000010011101010100011011111011110101000000010000010101010110111110000001000001010000000"
					"010110000010000100110");
	EXPECT_EQ(result, "(01)90012345678908(3932)0081234(20)01(10)AB1");

	result = parse("0011010000000000010011101010100011011111011110101111111001010010101010110110001011011101110101100101"
					"0101011011001001001111000010010010011000010000010000100110");
	EXPECT_EQ(result, "(01)90012345678908(3933)997123456789012345(20)01(10)AB1");
}
