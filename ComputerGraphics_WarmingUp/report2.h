#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#define PATH "data.txt"

class FileRead {
public:
	FileRead() = delete;

	FileRead(std::string& fPath) : path{ fPath } { }

	~FileRead() { };

private:
	std::string path{ };

	std::vector<int> vecDigit{ };
	std::vector<std::string> vecFileBuff{ };
	std::vector<std::string> vecFileWordBuff{ };
	std::vector<std::string> vecWord{ };
	std::vector<std::string> vecCapsWord{ };

	std::vector<std::string> modifyStrs{ };

public:
	bool isDigit(const std::string& str) const {
		for (auto e : str) {
			if (!isdigit(e)) {
				return false;
			}
		}
		return true;
	}

	bool isUpper(const std::string& str) const {
		for (auto& e : str) {
			if (isupper(e)) {
				return true;
			}
		}
		return false;
	}

	bool read() {
		std::ifstream fs{ path, std::ios::in };
		std::string  strBuf{ };
		if (!fs.is_open()) {
			return false;
		}

		while (!fs.eof()) {
			std::getline(fs, strBuf);
			vecFileBuff.push_back(strBuf);
			std::istringstream iss{ strBuf };
			while (iss >> strBuf) {
				// 숫자인지 검사
				if (isDigit(strBuf)) {
					int numBuf = atoi(strBuf.c_str());
					vecDigit.push_back(numBuf);
				}
				else {
					// 숫자가 아니면 대문자가 포함되었는지 검사
					if (isUpper(strBuf)) {
						vecCapsWord.push_back(strBuf);
					}
					vecWord.push_back(strBuf);
				}
			}
		}

		for (const auto& e : vecFileBuff) {
			modifyStrs.push_back(e);
		}

		fs.close();
		return true;
	}

	size_t getNumWord() {
		return vecWord.size();
	}

	size_t getNumDigit() {
		return vecDigit.size();
	}

	size_t getNumCaps() {
		return vecCapsWord.size();
	}

	void reverseLine() {
		for (auto& e : modifyStrs) {
			std::reverse(e.begin(), e.end());
		}
	}

	void printModify() {
		for (const auto& e : modifyStrs) {
			std::cout << e << std::endl;
		}
	}

	void replaceAll(const char tgCh, const char repCh) {
		for (auto& e : modifyStrs) {
			std::string::size_type pos = 0;
			while ((pos = e.find(tgCh)) != std::string::npos) {
				e.replace(pos, 1, 1, repCh);
			}
		}
	}

	void inputCh(const char input, const int space, const int cnt) {
		size_t inputIdx = space;
		size_t incIdx = static_cast<size_t>(space) + static_cast<size_t>(cnt);
		for (auto& e : modifyStrs) {
			size_t length = e.length();
			if (length < space) {
				continue;
			}
			bool inputEnd = length % space == 0 ? true : false;
			size_t inputCnt = length / space;
			if (inputEnd) {
				inputCnt--;
			}

			for (int i = 0; i < inputCnt; ++i) {
				if (inputIdx > e.length()) {
					break;
				}
				e.insert(inputIdx, cnt, input);
				inputIdx += incIdx;
			}

			if (inputEnd) {
				for (int i = 0; i < cnt; ++i) {
					e.push_back(input);
				}
			}
			inputIdx = space;
		}
	}

	void eraseCh(const char c) {
		for (auto& e : modifyStrs) {
			e.erase(std::remove(e.begin(), e.end(), c), e.end());
		}
	}

	void equalWhere() {
		for (const auto& e : modifyStrs) {
			if (e.empty()) {
				break;
			}

			std::string result{ };
			size_t length{ e.length() };
			size_t idxBegin{ };
			size_t idxEnd{ length - 1 };
			for (; idxBegin < idxEnd; ++idxBegin, --idxEnd) {
				if (e[idxBegin] == e[idxEnd]) {
					result += e[idxBegin];
				}
				else {
					break;
				}
			}

			if (result.empty()) {
				result = "0";
			}

			std::cout << result << std::endl;
		}
	}

	void reverseWord() {
		for (auto& e : modifyStrs) {
			if (e.empty()) {
				break;
			}

			bool noBlank{ true };
			std::string::size_type posEnd{ };
			std::string::size_type posBegin{ };
			std::string::iterator begin = e.begin();
			std::string::iterator end = e.end();
			std::string::iterator iter = e.begin();
			std::string::iterator iterEnd = e.begin();
			while ((posEnd = e.find(' ', posEnd + 1)) != std::string::npos) {
				iter = begin + posBegin;
				iterEnd = begin + posEnd;
				if (posBegin != 0 and (iter + 1) != iterEnd) {
					++iter;
				}


				std::reverse(iter, iterEnd);
				posBegin = posEnd;
				noBlank = false;
			}

			if (!noBlank) {
				std::reverse(++iterEnd, end);
			}
			else {
				std::reverse(begin, end);
			}
		}
	}
};;

void solution2()
{
	std::string path = PATH;
	FileRead fr{ path };

	fr.read();

	std::cout << "word count: " << fr.getNumWord() << "\n";
	std::cout << "number count: " << fr.getNumDigit() << "\n";
	std::cout << "Capital word: " << fr.getNumCaps() << "\n";
	
	fr.reverseLine();
	fr.printModify();
	fr.reverseLine();
	fr.printModify();
	fr.replaceAll('i', 'K');
	fr.printModify();
	fr.replaceAll('K', 'i');
	fr.printModify();
	fr.inputCh('@', 3, 2);
	fr.printModify();
	fr.eraseCh('@');
	fr.printModify();
	fr.equalWhere();

	fr.reverseWord();
	fr.printModify();
}