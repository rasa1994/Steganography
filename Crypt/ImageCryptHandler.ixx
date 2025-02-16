export module ImageCryptHandler;
import Crypt;
export import ImageLoader;
import <iostream>;
import <bitset>;
import <optional>;

void WriteIntoImage(Image& image, const std::string& message);
constexpr auto ImageDimension = 4;

export
{
	void EmplaceValueToPixel(RGBA& pixel, size_t value);
	size_t ExtractValueFromPixel(const RGBA& pixel);
	void EmplaceTwoCharsToPixel(RGBA& pixel, char a, const std::optional<char>& b);
	std::pair<char, char> ExtractTwoCharsFromPixel(const RGBA& pixel);

	unsigned char& GetElement(RGBA& pixel, size_t index)
	{
		switch (index)
		{
		case 0:
			return pixel.R;
		case 1:
			return pixel.G;
		case 2:
			return pixel.B;
		case 3:
			return pixel.A;
		}

		throw std::out_of_range("Index out of range");
	}

	unsigned char GetElement(const RGBA& pixel, size_t index)
	{
		switch (index)
		{
		case 0:
			return pixel.R;
		case 1:
			return pixel.G;
		case 2:
			return pixel.B;
		case 3:
			return pixel.A;
		}
		throw std::out_of_range("Index out of range");
	}


	void EncryptImage(const std::string& path, const std::string& message, const std::string& key)
	{
		ErrorType error = ErrorType::NO_ERROR;
		Image img = LoadImageFile(path, error);
		if (error != ErrorType::NO_ERROR)
		{
			std::cout << GetErrorString(error) << std::endl;
			return;
		}

		if (message.size() > img.m_width * img.m_height * ImageDimension)
		{
			std::cout << "Message is too long for the image" << std::endl;
			return;
		}

		WriteIntoImage(img, message);
	}

	void WriteIntoImage(Image& image, const std::string& message)
	{
		for (auto row{ 0ul }; row < image.m_height; ++row)
		{
			for (auto col{ 0ul }; col < image.m_width; ++col)
			{
				if (row == 0 && col == 0)
				{
					std::bitset<BIT* ImageDimension> length(message.size());
				}
			}
		}
	}

	void EmplaceValueToPixel(RGBA& pixel, size_t value)
	{
		std::bitset<BIT* ImageDimension> bitsetValue(value);
		for (auto dimension{ 0ul }; dimension < ImageDimension; ++dimension)
		{
			std::bitset<BIT> innerPixelValue{ 0 };
			for (auto bit{ 0ul }; bit < BIT; ++bit)
			{
				innerPixelValue[bit] = bitsetValue[dimension * BIT + bit];
			}
			GetElement(pixel, dimension) = static_cast<unsigned char>(innerPixelValue.to_ulong());
		}
	}

	size_t ExtractValueFromPixel(const RGBA& pixel)
	{
		std::bitset<ImageDimension* BIT> value;
		for (auto dimension{ 0ul }; dimension < ImageDimension; ++dimension)
		{
			std::bitset<BIT> bitsetValue(GetElement(pixel, dimension));
			for (auto bit{ 0ul }; bit < BIT; ++bit)
			{
				value[dimension * BIT + bit] = bitsetValue[bit];
			}
		}
		return value.to_ulong();
	}


	void EmplaceTwoCharsToPixel(RGBA& pixel, char a, const std::optional<char>& b)
	{
		std::bitset<BIT> bitsetR(pixel.R);
		std::bitset<BIT> bitsetG(pixel.G);
		std::bitset<BIT> bitsetCharA(a);
		for (auto bit{0ul}; bit < BIT / 2; ++bit)
		{
			bitsetR[bit + BIT / 2] = bitsetCharA[bit];
			bitsetG[bit + BIT / 2] = bitsetCharA[bit + BIT / 2];
		}

		pixel.R = static_cast<unsigned char>(bitsetR.to_ulong());
		pixel.G = static_cast<unsigned char>(bitsetG.to_ulong());

		if (b.has_value())
		{
			std::bitset<BIT> bitsetB(pixel.B);
			std::bitset<BIT> bitsetA(pixel.A);
			std::bitset<BIT> bitsetCharB(b.value());
			for (auto bit{ 0ul }; bit < BIT / 2; ++bit)
			{
				bitsetB[bit + BIT / 2] = bitsetCharB[bit];
				bitsetA[bit + BIT / 2] = bitsetCharB[bit + BIT / 2];
			}
			pixel.B = static_cast<unsigned char>(bitsetB.to_ulong());
			pixel.A = static_cast<unsigned char>(bitsetA.to_ulong());
		}
	}

	std::pair<char, char> ExtractTwoCharsFromPixel(const RGBA& pixel)
	{
		std::bitset<BIT> bitsetR(pixel.R);
		std::bitset<BIT> bitsetG(pixel.G);
		std::bitset<BIT> bitsetB(pixel.B);
		std::bitset<BIT> bitsetA(pixel.A);
		std::bitset<BIT> bitsetCharA;
		std::bitset<BIT> bitsetCharB;
		for (auto bit{ 0ul }; bit < BIT / 2; ++bit)
		{
			bitsetCharA[bit] = bitsetR[bit + BIT / 2];
			bitsetCharA[bit + BIT / 2] = bitsetG[bit + BIT / 2];
			bitsetCharB[bit] = bitsetB[bit + BIT / 2];
			bitsetCharB[bit + BIT / 2] = bitsetA[bit + BIT / 2];
		}

		return { static_cast<char>(bitsetCharA.to_ulong()), static_cast<char>(bitsetCharB.to_ulong()) };
	}
}


