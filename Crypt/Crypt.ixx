export module Crypt;

export import <iostream>;
import <unordered_map>;
import <algorithm>;
import <bitset>;
import <ranges>;
import <random>;
import <cassert>;
import <vector>;
import <optional>;
import <array>;


export
{

	using uchar = unsigned char;
	const std::string key = "32R17K";

	struct Matrix
	{
	private:
		using container = std::vector<std::vector<std::array<size_t, 3>>>;
		container matrix;
		using it = typename container::iterator;
		using cit = typename container::const_iterator;

	public:
		Matrix(size_t i, size_t j)
		{
			matrix.resize(i);
			for (auto& v : matrix)
			{
				v.resize(j);
				for (auto& s : v)
					s.fill(0);
			}
		}

		it begin() { return matrix.begin(); }
		it end() { return matrix.end(); }
		cit begin() const { return matrix.begin(); }
		cit end() const { return matrix.end(); }

		std::vector<std::array<size_t, 3>>& operator[](size_t index)
		{
			return matrix[index];
		}

		[[maybe_unused]] void PrintMatrix() const noexcept
		{
			for (const auto& i : matrix)
			{
				for (const auto& j : i)
				{
					std::cout << "[";
					for (size_t arr = 0; arr < j.size(); arr++)
					{
						std::cout << j[arr];
						if (arr != j.size() - 1)
							std::cout << ", ";
					}

					std::cout << "]";
				}
				std::cout << std::endl;
			}
		}

		std::string GetCryptedMessageFromMatrix(size_t cryptBytes)
		{
			std::string returnValue;
			std::bitset<8> Char;
			size_t nextByte = 0;
			std::bitset<2> twoBytesUse;
			std::bitset<4> fourBytesUse;
			size_t messageLength = 0;
			for (size_t i = 0; i < matrix.size(); i++)
			{
				for (size_t j = 0; j < matrix[i].size(); j++)
				{
					if (i == 0 && j == 0)
					{
						std::bitset<24> length(0);
						for (size_t k = 0; k < 3; k++)
						{
							std::bitset<8> factorDivided(matrix[i][j][k]);
							for (size_t l = 0; l < 8; l++)
							{
								length[l + k * 8] = factorDivided[l];
							}
						}
						messageLength = length.to_ulong();
						std::cout << messageLength;
						continue;
					}

					if (messageLength == returnValue.size())
						return returnValue;

					for (size_t k = 0; k < 3; k++)
					{
						if (nextByte >= 8)
						{
							nextByte = 0;
							returnValue.push_back(static_cast<char>(Char.to_ulong()));
						}

						std::bitset<8> value(matrix[i][j][k]);

						for (size_t l = 0; l < static_cast<size_t>(8 - cryptBytes); l++)
						{
							if (cryptBytes == 2)
							{
								twoBytesUse[l] = value[l];
							}
							else
							{
								fourBytesUse[l] = value[l];
							}
						}

						for (size_t l = 0; l < static_cast<size_t>(8 - cryptBytes); l++)
						{
							// Change this part
							if (cryptBytes == 2)
							{
								Char[nextByte++] = twoBytesUse[l];
							}
							else
							{
								Char[nextByte++] = fourBytesUse[l];
							}
						}
					}
				}
			}

			return returnValue;
		}

		void InsertCryptedString(const std::string& cryptedMessage, size_t cryptBytes)
		{
			std::bitset<24> messageLength(cryptedMessage.size());
			std::optional<std::bitset<8>> Char;
			size_t nextChar = 0;
			size_t nextByte = 0;
			std::bitset<2> twoBytesUse;
			std::bitset<4> fourBytesUse;
			for (size_t i = 0; i < matrix.size(); i++)
			{
				for (size_t j = 0; j < matrix[i].size(); j++)
				{
					if (i == 0 && j == 0)
					{
						for (size_t k = 0; k < 3; k++)
						{
							std::bitset<8> factorDivided(0);
							for (size_t l = 0; l < 8; l++)
							{
								factorDivided[l] = messageLength[l + k * 8];
							}
							matrix[i][j][k] = static_cast<size_t>(factorDivided.to_ulong());
						}
						continue;
					}
					for (size_t k = 0; k < 3; k++)
					{
						if (!Char.has_value())
						{
							if (nextChar == cryptedMessage.size() - 1)
								return;

							Char = std::bitset<8>(cryptedMessage[nextChar++]);
						}

						for (size_t l = static_cast<size_t>(8 - cryptBytes); l < 8; l++)
						{
							// Change this part
							if (cryptBytes == 2)
							{
								twoBytesUse[l - (8 - cryptBytes)] = Char.value()[nextByte++];
							}
							else
							{
								fourBytesUse[l - (8 - cryptBytes)] = Char.value()[nextByte++];
							}
						}

						matrix[i][j][k] = static_cast<size_t>((cryptBytes == 2) ? twoBytesUse.to_ulong() : fourBytesUse.to_ullong());
						if (nextByte >= 8)
						{
							nextByte = 0;
							Char.reset();
						}
					}
				}
			}
		}
	};

	struct CryptValues
	{
		size_t m_randValues;
		size_t m_shiftValues;
		std::string m_key;
		CryptValues(const std::string& _key) : m_randValues(0), m_shiftValues(0), m_key(_key)
		{
			ExtractRandValues();
			ExtractShiftValues();
		}

	private:
		void ExtractRandValues() noexcept
		{
			if (!m_key.empty())
			{
				std::string val;
				size_t it = 0;
				while (isdigit(m_key[it]))
				{
					val.push_back(m_key[it++]);
				}

				m_randValues = atol(m_key.c_str());
				m_key = m_key.substr(val.size() + 1, m_key.size());
			}
		};
		void ExtractShiftValues() noexcept
		{
			if (!m_key.empty())
			{
				std::string val;
				size_t it = 0;
				while (isdigit(m_key[it]))
				{
					val.push_back(m_key[it++]);
				}
				m_shiftValues = atol(m_key.c_str());
				m_key = m_key.substr(val.size() + 1, m_key.size());
			}
		};
	};

	std::random_device device{};
	std::mt19937 engine(device());

	size_t GetRandomNumber(size_t min, size_t max)
	{
		std::uniform_int_distribution<size_t> dist(min, max - 1);
		return dist(engine);
	}

	const std::unordered_map<uchar, size_t> codes
	{
		{'q', 0},
		{'w', 1},
		{'e', 2},
		{'r', 3},
		{'t', 4},
		{'y', 5},
		{'u', 6},
		{'i', 7},
		{'o', 8},
		{'p', 9},
		{'a', 10},
		{'s', 11},
		{'d', 12},
		{'f', 13},
		{'g', 14},
		{'h', 15},
		{'j', 16},
		{'k', 17},
		{'l', 18},
		{'z', 19},
		{'x', 20},
		{'c', 21},
		{'v', 22},
		{'b', 23},
		{'n', 24},
		{'m', 25},
		{',', 26},
		{'.', 27},
		{'_', 28},
		{'?', 29},
		{'!', 30},
		{' ', 31},
		{'+', 32},
		{'-', 33},
		{'*', 34},
		{'/', 35},
		{'0', 36},
		{'1', 37},
		{'2', 38},
		{'3', 39},
		{'4', 40},
		{'5', 41},
		{'6', 42},
		{'7', 43},
		{'8', 44},
		{'9', 45}
	};

	constexpr size_t MaxBits = 6;


	std::vector<std::bitset<MaxBits>> CreateVectorOfBits(const std::string& message)
	{
		std::vector<std::bitset<MaxBits>> cyphered;
		std::ranges::transform(message, std::back_inserter(cyphered), [](const auto c)
			{
				if (const auto toLower = std::tolower(c); codes.contains(toLower))
					return codes.at(toLower);

				return size_t{};
			});

		return cyphered;
	}

	std::string CreateHashedString(const std::vector<std::bitset<MaxBits>>& cypheredString)
	{
		std::string returnValue;
		std::vector<bool> fullCypheredValues;
		fullCypheredValues.reserve(cypheredString.size() * MaxBits + 8);
		for (const auto& cString : cypheredString)
		{
			for (auto lbegin = 0; lbegin < MaxBits; lbegin++)
			{
				fullCypheredValues.push_back(cString[lbegin]);
			}
		}

		while (fullCypheredValues.size() % 8 != 0)
		{
			fullCypheredValues.push_back(false);
		}

		for (size_t Char = 0; Char < fullCypheredValues.size(); Char += 8)
		{
			std::bitset<8> word(0);
			for (size_t pos = 0; pos < 8; pos++)
			{
				word[pos] = fullCypheredValues[Char + pos];
			}
			returnValue.push_back(static_cast<char>(word.to_ulong()));
		}

		return returnValue;
	}

	std::vector<std::bitset<MaxBits>> CreateVectorOfBitsFromHashedValue(const std::string& cyphered)
	{
		std::vector<std::bitset<MaxBits>> returnValue;
		std::vector<bool> cypheredToArray;
		cypheredToArray.reserve(cyphered.size() * 8);
		for (const auto c : cyphered)
		{
			std::bitset<8> charToBit(c);
			for (size_t lbegin = 0; lbegin < 8; lbegin++)
			{
				cypheredToArray.push_back(charToBit[lbegin]);
			}
		}

		while (cypheredToArray.size() % 6 != 0)
		{
			cypheredToArray.pop_back();
		}

		for (size_t word = 0; word < cypheredToArray.size(); word += MaxBits)
		{
			std::bitset<MaxBits> oneChar(0);
			for (size_t lBegin = 0; lBegin < MaxBits; lBegin++)
			{
				oneChar[lBegin] = cypheredToArray[word + lBegin];
			}
			returnValue.push_back(oneChar);
		}
		return returnValue;
	}

	std::string DecypherCipheredVector(const std::vector<std::bitset<MaxBits>>& decyphered)
	{
		std::string returnValue;

		for (const auto& Char : decyphered)
		{
			const auto val = std::ranges::find_if(codes, [Char](const auto& codeVal) {return codeVal.second == Char; });
			returnValue.push_back(val->first);
		}

		return returnValue;
	}

	std::string InsertRandomValues(const std::string& sentence, size_t randCharacter)
	{
		std::string returnValue;
		for (const auto Char : sentence)
		{
			returnValue.push_back(Char);
			for (auto _ : std::views::iota(0ul, randCharacter))
			{
				auto it = codes.begin();
				std::advance(it, GetRandomNumber(0, codes.size()));
				returnValue.push_back(it->first);
			}
		}

		return returnValue;
	}

	std::string RemoveRandomValues(const std::string& sentence, size_t randCharacter)
	{
		std::string returnValue{};
		for (size_t Char = 0; Char < sentence.size(); Char += randCharacter)
		{
			returnValue.push_back(sentence[Char]);
		}
		return returnValue;
	}


	std::string ShiftString(const std::string& sentence, size_t shiftPlace)
	{
		std::string returnValue;
		returnValue.resize(sentence.size());

		for (size_t Char = 0; Char < sentence.size(); Char++)
		{
			returnValue[(Char + shiftPlace) % sentence.size()] = sentence[Char];
		}

		return returnValue;
	}

	std::string RemoveShiftString(const std::string& sentence, size_t shiftPlace)
	{
		shiftPlace = shiftPlace % sentence.size();
		shiftPlace = sentence.size() - shiftPlace;

		return ShiftString(sentence, shiftPlace);
	}
}