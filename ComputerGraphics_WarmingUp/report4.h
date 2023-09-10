#pragma once

#include <iostream>
#include <algorithm>

struct Vec3 {
	int x;
	int y;
	int z;
};

class Queue {
public:
	Queue() {
		q = new Vec3[maxQueueSize];
		sortQ = new Vec3[maxQueueSize];
	}

	~Queue() {
		if (q) {
			delete[] q;
		}
		q = nullptr;

		if (sortQ) {
			delete[] sortQ;
		}
		q = nullptr;
	}

private:
	const size_t maxQueueSize{ 10 };
	Vec3* q{ nullptr };
	Vec3* sortQ{ nullptr };
	Vec3* memPrevQ{ nullptr };
	size_t size{ };
	int curIndex{ -1 };

public:
	void insertData(const Vec3& data, size_t index) {
		q[index].x = data.x;
		q[index].y = data.y;
		q[index].z = data.z;
	}

	bool insert(const Vec3& data) {
		if (size == maxQueueSize) {
			return false;
		}

		++size;
		++curIndex;
		for (int i = curIndex; i > 0; --i) {
			q[i] = q[i - 1];
		}

		insertData(data, 0);
		return true;
	}

	bool pushBack(const Vec3& data) {
		if (size == maxQueueSize) {
			return false;
		}

		++size;
		++curIndex;

		insertData(data, curIndex);
		return true;
	}

	Vec3 popBack() {
		if (empty()) {
			return Vec3{ };
		}

		Vec3 rtVal = q[curIndex--];
		--size;
		return rtVal;
	}

	Vec3 popFront() {
		if (empty()) {
			return Vec3{ };
		}

		Vec3 rtVal = q[0];
		for (int i = 0; i < curIndex; ++i) {
			q[i] = q[i + 1];
		}
		--curIndex;
		--size;
		return rtVal;
	}

	void sortFront() {
		if (empty()) {
			return;
		}
		this->sort();
		printVec(sortQ[0], true);
	}

	void sortBack() {
		if (empty()) {
			return;
		}
		this->sort();
		printVec(sortQ[curIndex], true);
	}

	size_t getSize() const {
		return size;
	}

	bool empty() const {
		return size == 0 ? true : false;
	}

	void clear() {
		size = 0;
		curIndex = -1;
	}

	void sort(bool reverse = false) {
		if (empty()) {
			return;
		}

		memcpy(sortQ, q, sizeof(Vec3) * size);

		if (reverse) {
			std::sort(sortQ, sortQ + size, [](const Vec3& v1, const Vec3& v2)->bool {
				int dist1 = (v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z);
				int dist2 = (v2.x * v2.x) + (v2.y * v2.y) + (v2.z * v2.z);
				return dist1 > dist2;
				}
			);
		}
		else {
			std::sort(sortQ, sortQ + size, [](const Vec3& v1, const Vec3& v2)->bool {
				int dist1 = (v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z);
				int dist2 = (v2.x * v2.x) + (v2.y * v2.y) + (v2.z * v2.z);
				return dist1 < dist2;
				}
			);
		}
	}

	void printVec(const Vec3& data, bool newLine = false) const {
		std::cout << "(" << data.x << ", " << data.y << ", " << data.z << ")";
		if (newLine) {
			std::cout << "\n";
		}
	}

	void printSortQ() {
		for (int i = 0; i < size; ++i) {
			std::cout << "index " << i << ": ";
			printVec(sortQ[i], true);
		}
	}

	void print(bool reverse = false) const {
		for (int i = 0; i < size; ++i) {
			std::cout << "index " << i << ": ";
			printVec(q[i], true);
		}
	}

	void printBack() const {
		for (int i = curIndex; i >= 0; --i) {
			std::cout << "index " << i << ": ";
			printVec(q[i], true);
		}
	}
};

void solution4()
{
	Queue q{ };
	bool printSort{ false };
	while (true) {
		char input{ };
		std::cin >> input;

		Vec3 data{ };

		switch (input) {
		case '+':
			std::cin >> data.x >> data.y >> data.z;
			q.pushBack(data);
			q.print();
			break;

		case '-':
			q.popBack();
			q.print();
			break;

		case 'e':
			std::cin >> data.x >> data.y >> data.z;
			q.insert(data);
			q.print();
			break;

		case 'd':
			q.popFront();
			q.print();
			break;

		case 'l':
			q.print();
			std::cout << "Á¡ÀÇ °¹¼ö: " << q.getSize() << std::endl;
			break;

		case 'c':
			q.clear();
			break;

		case 'm':
			q.sortBack();
			break;

		case 'n':
			q.sortFront();
			break;

		case 'a':
			printSort = !printSort;
			if (printSort) {
				q.sort();
				q.printSortQ();
			}
			else {
				q.print();
			}
			break;

		case 's':
			printSort = !printSort;
			if (printSort) {
				q.sort(true);
				q.printSortQ();
			}
			else {
				q.print();
			}
			break;

		case 'q':
			return;
			break;
		}
	}
}