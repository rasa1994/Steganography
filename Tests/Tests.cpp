#include <gtest/gtest.h>

import Crypt;


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

TEST(CryptTest, RemoveRandomValues)
{
	constexpr auto sentence = "123456789";
	constexpr auto randCharacter = 3;
	const auto removed = RemoveRandomValues(sentence, randCharacter);
	EXPECT_EQ(removed, "147");
}

TEST(CryptTest, InsertRandomValues)
{
	const std::string sentence = "Hello World!";
	constexpr auto randCharacter = 3;
	const auto inserted = InsertRandomValues(sentence, randCharacter);
	EXPECT_EQ(inserted.size(), sentence.size() * (randCharacter + 1));
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


int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}