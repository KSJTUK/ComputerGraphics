#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <random>
//
std::random_device rd;
std::default_random_engine dre{ rd() };

#define SLEEPTIME 3000

int getRandSol5(int min, int max)
{
	std::uniform_int_distribution<> uid{ min, max };
	return uid(dre);
}


class CardGame {
public:
	CardGame() { }
	~CardGame() { }

public:
	class Pt {
	public:
		int x{ };
		int y{ };

		Pt() = default;
		~Pt() { };

		Pt(int x, int y) : x{ x }, y{ y } { };

		Pt& operator=(const Pt& other) {
			x = other.x;
			y = other.y;
			return *this;
		}
	};

private:
	size_t cardSize{ 8 };
	size_t boardSize{ 4 };
	std::vector<unsigned short> colorVec{ 16 };
	std::unordered_map<char, unsigned short> cardColor{ };
	std::vector<char> cardSet{ };
	std::vector<std::vector<bool>> fliped{ };
	std::vector<std::vector<char>> board{ };

	Pt selectedPt{ };
	bool flip2Card{ false };
	int flipCnt{ };

public:
	bool init() {
		for (int i = 0; i < cardSize; ++i) {
			for (int j = 0; j < 2; ++j) {
				cardSet.push_back('A' + i);
			}
			cardColor.insert(std::make_pair(static_cast<char>('A' + i), i + 1));
		}

		int loopSize = getRandSol5(1, 10);
		for (int i = 0; i < loopSize; ++i) {
			std::random_shuffle(cardSet.begin(), cardSet.end());
		}
		//
		board.resize(boardSize);
		for (int y = 0; y < boardSize; ++y) {
			board[y].resize(boardSize);
			for (int x = 0; x < boardSize; ++x) {
				board[y][x] = cardSet[(y * boardSize) + x];
			}
		}

		fliped.resize(boardSize);
		for (int y = 0; y < boardSize; ++y) {
			fliped[y].resize(boardSize);
			for (int x = 0; x < boardSize; ++x) {
				fliped[y][x] = false;
			}
		}

		return true;
	}

	bool sameCard(const Pt& p1, const Pt& p2) {
		if (board[p1.y][p1.x] == board[p2.y][p2.x]) {
			return true;
		}

		return false;
	}

	void flipCard(const Pt& p) {
		fliped[p.y][p.x] = !fliped[p.y][p.x];
	}

	void print() const {
		std::cout << cardColor.size() << std::endl;
		std::cout << "   a  b  c  d" << std::endl;
		for (int y = 0; y < boardSize; ++y) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			std::cout << y + 1 << "  ";
			for (int x = 0; x < boardSize; ++x) {
				if (fliped[y][x]) {
					auto iter = cardColor.find(board[y][x]);
					unsigned short color = iter->second;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
					std::cout << board[y][x] << "  ";
				}
				else {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					std::cout << "*  ";
				}
			}
			std::cout << std::endl;
		}
	}

	void update(const Pt& pt) {
		flipCard(pt);
		++flipCnt;

		if (flipCnt == 2) {
			if (!sameCard(selectedPt, pt)) {
				system("cls");
				print();
				
				flipCard(selectedPt);
				flipCard(pt);
				flipCnt = 0;

				selectedPt = Pt{ };
				Sleep(SLEEPTIME);

				return;
			}
			else {
				selectedPt = Pt{ };
				flipCnt = 0;
			}
		}

		selectedPt = pt;
	}

	bool GameEndCheck() {
		for (int y = 0; y < boardSize; ++y) {
			for (int x = 0; x < boardSize; ++x) {
				if (!fliped[y][x]) {
					return false;
				}
			}
		}

		return true;
	}

	void input() {
		char chBuf[2]{ };
		Pt select{ };

		while (true) {
			system("cls");
			print();
			std::cout << "input card " << flipCnt + 1 << ": ";

			for (int i = 0; i < 2; ++i) {
				chBuf[i] = getchar();
			}

			rewind(stdin);

			if (!correctInput(chBuf[0], chBuf[1])) {
				continue;
			}

			select.x = chBuf[0] - 'a';
			select.y = chBuf[1] - '1';

			break;
		}
		
		update(select);
	}

	void clear() {

	}

	bool correctInput(char c1, char c2) {
		int diff1{ c1 - 'a'};
		if (diff1 >= 0 and diff1 < boardSize) {
			int diff2{ c2 - '1' };
			if (diff2 >= 0 and diff2 < boardSize) {
				return true;
			}
		}

		return false;
	}

public:
	void loop() {
		bool flip2Card{ false };
		while (true) {
			input();
			
			print();

			if (GameEndCheck()) {
				std::cout << "game clear!" << std::endl;
				std::cout << "retry: any key" << std::endl;
				std::cout << "quit: q" << std::endl;

				//char ch;

			}
		}
	}
};

void solution5()
{
	CardGame game{ };
	game.init();

	game.loop();
}