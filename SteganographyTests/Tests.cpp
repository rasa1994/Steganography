#include <bitset>
#include <gtest/gtest.h>

import Crypt;
import ImageCryptHandler;

TEST(CryptTest, GetCryptRandValues)
{
	constexpr auto CryptValuesTest = "22k10l";
	CryptValues crypt_values(CryptValuesTest);
	EXPECT_EQ(crypt_values.m_randValues, 22);
}


TEST(CryptTest, GetCryptShiftValues)
{
	constexpr auto CryptValuesTest = "22k10l";
	CryptValues crypt_values(CryptValuesTest);
	EXPECT_EQ(crypt_values.m_shiftValues, 10);
}

TEST(CryptTest, GetRandomNumber)
{
	constexpr auto min = 0;
	constexpr auto max = 10;
	const auto random_number = GetRandomNumber(min, max);
	EXPECT_GE(random_number, min);
	EXPECT_LE(random_number, max);
}

TEST(CryptTest, CreateVectorOfBits)
{
	constexpr auto message = "Hello World";
	const auto bits = CreateVectorOfBits(message);
	EXPECT_EQ(bits.size(), 11);
}

TEST(CryptTest, RemoveShiftString)
{
	constexpr auto sentence = "12345678";
	constexpr auto shiftPlace = 3;
	const auto shifted = RemoveShiftString(sentence, shiftPlace);
	EXPECT_EQ(shifted, "45678123");
}

TEST(CryptTest, ShiftRemoveShift)
{
	constexpr auto sentence = "123awawdawdwdawdawdadawdawddawdaw45678123zzmcmmcmcmc!!!HHHHH@#awdzzxxczxczcxzcsssaddzxc";
	constexpr auto shiftPlace = 1555;
	const auto shifted = ShiftString(sentence, shiftPlace);
	auto removed = RemoveShiftString(shifted, shiftPlace);
	EXPECT_EQ(removed, sentence);
}

TEST(CryptTest, RemoveRandomValues)
{
	constexpr auto sentence = "123456789";
	constexpr auto randCharacter = 3;
	const auto removed = RemoveRandomValues(sentence, randCharacter);
	EXPECT_EQ(removed, "159");
}

TEST(CryptTest, InsertRandomValues)
{
	const std::string sentence = "Hello World!";
	constexpr auto randCharacter = 3;
	const auto inserted = InsertRandomValues(sentence, randCharacter);
	EXPECT_EQ(inserted.size(), sentence.size() * (randCharacter + 1));
}

TEST(CryptTest, InsertAndRemoveRandomValues)
{
	const std::string sentence = "hello world!";
	constexpr auto randCharacter = 3;
	const auto inserted = InsertRandomValues(sentence, randCharacter);
	const auto removed = RemoveRandomValues(inserted, randCharacter);
	EXPECT_EQ(sentence, removed);
}

TEST(CryptTest, ShiftString)
{
	constexpr auto sentence = "Hello World!";
	constexpr auto shiftPlace = 3;
	const auto shifted = ShiftString(sentence, shiftPlace);
	EXPECT_EQ(shifted, "ld!Hello Wor");
}


TEST(CryptTest, CreateHashedString)
{
	const std::string message = "Helloawdawddzzzz Worawawdazzzzzzawdawdwz222123123aasdvbnld!";
	const auto bits = CreateVectorOfBits(message);
	const auto hashed = CreateHashedString(bits);
	EXPECT_EQ(hashed.size(), std::ceil(static_cast<float>(bits.size()) * MaxBits / 8));
}

TEST(CryptTest, CreateVectorOfBitsFromHashedValue)
{
	const std::string message = "He222112313132zzlo Worlaazxcvvbnfcccd!";
	const auto bits = CreateVectorOfBits(message);
	const auto hashed = CreateHashedString(bits);
	const auto decypheredBits = CreateVectorOfBitsFromHashedValue(hashed);
	const bool isEqual = std::equal(bits.begin(), bits.end(), decypheredBits.begin());
	EXPECT_TRUE(isEqual);
}

TEST(CryptTest, DecypherCipheredVector)
{
	const std::string message = "hello world!";
	const auto bits = CreateVectorOfBits(message);
	const auto hashed = CreateHashedString(bits);
	const auto decypheredBits = CreateVectorOfBitsFromHashedValue(hashed);
	const auto decyphered = DecypherCipheredVector(decypheredBits);
	EXPECT_EQ(decyphered, message);
}


TEST(CryptTest, GetCryptedMessageFromMatrix)
{
	Matrix matrix(100, 100);
	matrix.InsertCryptedString("Hello World959595959595119ajiajjisidjaijdjiwadjijiwAIWWIDIAWDIAIWD9292992929211131322333345!", 4);
	const auto result = matrix.GetCryptedMessageFromMatrix(4);
	EXPECT_EQ(result, "Hello World959595959595119ajiajjisidjaijdjiwadjijiwAIWWIDIAWDIAIWD9292992929211131322333345!");
}

TEST(CryptTest, GetCryptedMessageFromMatrixWithShift)
{
	const auto CompareLower = [](const std::string& str1, const std::string& str2)
		{
		  return std::ranges::equal(str1, str2, [](char a, char b)
		  {
			return std::tolower(a) == std::tolower(b);
		  });
		};
	const std::string beginingTestString = "This is first test string!";
	std::string hashString = "191K22m";
	CryptValues cryptValues(std::move(hashString));
	const auto bits = InsertRandomValues(beginingTestString, cryptValues.m_randValues);
	const auto randomShifted = ShiftString(bits, cryptValues.m_shiftValues);
	const auto cyphered = CreateVectorOfBits(randomShifted);
	auto result = CreateHashedString(cyphered);

	Matrix matrix(100, 100);
	matrix.InsertCryptedString(result, 4);
	result = matrix.GetCryptedMessageFromMatrix(4);
	const auto decrypted = CreateVectorOfBitsFromHashedValue(result);
	EXPECT_EQ(cyphered, decrypted);
	const auto decypher = DecypherCipheredVector(decrypted);
	EXPECT_TRUE(CompareLower(randomShifted, decypher));
	const auto removeShift = RemoveShiftString(decypher, cryptValues.m_shiftValues);
	EXPECT_TRUE(CompareLower(bits, removeShift));
	const auto removedValues = RemoveRandomValues(removeShift, cryptValues.m_randValues);
	EXPECT_TRUE(CompareLower(beginingTestString, removedValues));
}

TEST(ImageCryptHandlerTest, EmplaceValueToPixel)
{
	RGBA pixel{ 0, 0, 0, 0 };
	constexpr auto value = 2125;
	EmplaceValueToPixel(pixel, value);
	const auto extracted = ExtractValueFromPixel(pixel);
	EXPECT_EQ(value, extracted);
}


TEST(ImageCryptHandlerTest, EmplaceTwoCharsToPixel)
{
	RGBA pixel{ 0, 0, 0, 0 };
	constexpr auto a = 'a';
	constexpr auto b = 'b';
	EmplaceTwoCharsToPixel(pixel, a, b);
	std::bitset<BIT> bitsetR(pixel.R);
	std::bitset<BIT> bitsetG(pixel.G);
	std::bitset<BIT> bitsetB(pixel.B);
	std::bitset<BIT> bitsetA(pixel.A);
	std::bitset<BIT> bitsetCharA(a);
	std::bitset<BIT> bitsetCharB(b);
	for (auto bit{ 0ul }; bit < BIT / 2; ++bit)
	{
		EXPECT_EQ(bitsetR[bit + BIT / 2], bitsetCharA[bit]);
		EXPECT_EQ(bitsetG[bit + BIT / 2], bitsetCharA[bit + BIT / 2]);
		EXPECT_EQ(bitsetB[bit + BIT / 2], bitsetCharB[bit]);
		EXPECT_EQ(bitsetA[bit + BIT / 2], bitsetCharB[bit + BIT / 2]);
	}
}

TEST(ImageCryptHandlerTest, ExtractedTwoCharsFromPixel)
{
	RGBA pixel{ 0, 0, 0, 0 };
	constexpr auto a = 'a';
	constexpr auto b = 'b';
	EmplaceTwoCharsToPixel(pixel, a, b);
	const auto extractedA = ExtractTwoCharsFromPixel(pixel);
	EXPECT_EQ(a, extractedA.first);
	EXPECT_EQ(b, extractedA.second);
}


int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}