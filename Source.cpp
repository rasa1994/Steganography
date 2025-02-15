import Crypt;

auto main() -> int
{
	std::string TestSequence = "hello, im john malkovich from siberia, i have 32 children";
	std::cout << TestSequence.size() << std::endl;
	std::string TestKey = "2K22L";
	CryptValues cryptValues(TestKey);
	const auto withRandomValues = InsertRandomValues(TestSequence, cryptValues.m_randValues);
	const auto randomShifted = ShiftString(withRandomValues, cryptValues.m_shiftValues);
	const auto cyphered = CreateVectorOfBits(randomShifted);
	auto result = CreateHashedString(cyphered);

	auto hashedString = result;
	
	Matrix matrix(100, 100);
	matrix.InsertCryptedString(result, 4);
	result = matrix.GetCryptedMessageFromMatrix(4);

	const auto decrypted = CreateVectorOfBitsFromHashedValue(result);
	const auto decypher = DecypherCipheredVector(decrypted);
	const auto removeShift = RemoveShiftString(decypher, cryptValues.m_shiftValues);
	const auto removedValues = RemoveRandomValues(removeShift, cryptValues.m_randValues);

	std::cout << removedValues;
}