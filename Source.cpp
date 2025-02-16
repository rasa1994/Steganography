import Crypt;
import ImageCryptHandler;
import <bitset>;
import <filesystem>;
auto main() -> int
{
	std::string exit{};
	std::cout << "Path: " << std::endl;
	std::cin >> exit;
	std::string key = "22k22m";
	EncryptImage(exit, "Hello teszzt testzz", key);
	std::cout << "Encrypted" << std::endl;
	const auto parentPath = std::filesystem::path(exit).parent_path().string();
	DecryptImage(parentPath +"/Crypted.bmp", key, exit);
	std::cout << "Message: " << exit << std::endl;
}