#include <iostream>
#include <random>
#include <valarray>
#include <vector>
#include <string> 

std::random_device rd;
std::default_random_engine dre{ rd() };

int getRand(int min, int max)
{
	std::uniform_int_distribution<> uid{ min, max };

	return uid(dre);
}

class Matrix {
private:
	int matSize{ };
	std::vector<std::vector<int>> matrix{ };
	
public:
	Matrix() : matSize{ 0 }, matrix{ } { }

	Matrix(int matSize) : matSize{ matSize } {
		matrix.resize(matSize);
		for (int i = 0; i < matSize; ++i) {
			matrix[i].resize(matSize);
			for (int j = 0; j < matSize; ++j) {
				matrix[i][j] = getRand(1, 10);
			}
		}
	}

	Matrix(int matSize, int init) : matSize{ matSize } {
		matrix.resize(matSize);
		for (int i = 0; i < matSize; ++i) {
			matrix[i].resize(matSize);
			for (int j = 0; j < matSize; ++j) {
				matrix[i][j] = init;
			}
		}
	}

	~Matrix() {
	}

	Matrix(const Matrix& other) : matSize{ other.matSize } {
		matrix.resize(matSize);
		for (int i = 0; i < matSize; ++i) {
			matrix[i].resize(matSize);
			for (int j = 0; j < matSize; ++j) {
				matrix[i][j] = other.matrix[i][j];
			}
		}
	}


public:
	void print() const {
		for (int i = 0; i < matSize; ++i) {
			for (int j = 0; j < matSize; ++j) {
				std::cout << matrix[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}

	void printOper(const Matrix& other, const std::string& oper)
	{
		for (int i = 0; i < matSize; ++i) {
			for (int j = 0; j < matSize; ++j) {
				std::cout << matrix[i][j] << " ";
			}

			if (!oper.empty() and ((matSize % 2 + 1) == i)) {
				std::cout << oper << " ";
			}

			for (int j = 0; j < matSize; ++j) {
				std::cout << other.matrix[i][j] << " ";
			}

			std::cout << std::endl;
		}
	}

	void operator+(const Matrix& other) {
		for (int i = 0; i < matSize; ++i) {
			for (int j = 0; j < matSize; ++j) {
				matrix[i][j] += other.matrix[i][j];
			}
		}
	}

	void operator-(const Matrix& other) {
		for (int i = 0; i < matSize; ++i) {
			for (int j = 0; j < matSize; ++j) {
				matrix[i][j] -= other.matrix[i][j];
			}
		}
	}

	void operator*=(const Matrix& other) {
		for (int i = 0; i < matSize; ++i) {
			for (int j = 0; j < matSize; ++j) {
				for (int k = 0; k < matSize; ++k) {
					matrix[i][k] *= other.matrix[k][j];
				}
			}
		}
	}

	void Trans() {
		std::vector<std::vector<int>> newMat{ matrix };
		for (int i = 0; i < matSize; ++i) {
			for (int j = 0; j < matSize; ++j) {
				newMat[i][j] = matrix[j][i];
			}
		}
		matrix = newMat;
	}

	void operator*=(int s) {
		for (int i = 0; i < matSize; ++i) {
			for (int j = 0; j < matSize; ++j) {
				matrix[i][j] *= s;
			}
		}
	}

	Matrix& operator=(const Matrix& other) {
		matSize = other.matSize;
		matrix = other.matrix;
		return *this;
	}

	Matrix expansion() {
		Matrix newMat{ matSize + 1, 0 };
		for (int i = 0; i < matSize; ++i) {
			for (int j = 0; j < matSize; ++j) {
				newMat.matrix[i][j] = matrix[i][j];
			}
		}
		newMat.matrix[matSize][matSize] = 1;
		return newMat;
	}

	Matrix reduction(int delCol, int delRow) {
		if (matSize == 1) {
			return Matrix();
		}

		int newMatSize = matSize - 1;
		Matrix redMat(newMatSize, 0);

		int tgRow{ };
		for (int i = 0; i < matSize; ++i) {
			int tgCol{ };
			if (i == delRow) {
				continue;
			}

			for (int col = 0; col < matSize; ++col) {
				if (col == delCol) {
					continue;
				}

				redMat.matrix[tgRow][tgCol] = matrix[i][col];
				++tgCol;
			}
			++tgRow;
		}

		return redMat;
	}
	

	int getSize() {
		return matSize;
	}

	int det() {
		return det(matSize, *this);
	}

private:
	int det(int size, Matrix& tgMat) {
		int rtVal{ };
		if (tgMat.getSize() == 1) {
			return tgMat.matrix[0][0];
		}

		int minorMatSize = size - 1;

		int delRow{ };
		int sign{ };
		for (int delCol = 0; delCol < size; ++delCol) {
			if (delCol % 2 == 1) {
				sign = -1;
			}
			else {
				sign = 1;
			}

			Matrix minorMat = tgMat.reduction(delCol, delRow);

			int detVal = det(minorMatSize, minorMat);
			int matVal = tgMat.matrix[delRow][delCol];
			rtVal += sign * matVal * detVal;
		}

		return rtVal;
	}

	void getMinorMatContents(Matrix& dest, Matrix& source, int tgR, int tgC) {
		int sourceSize = source.getSize();
		if (sourceSize == 1) {
			return;
		}

		int destCol{ };
		int destRow{ };
		for (int r = 0; r < sourceSize; ++r) {
			if (r == tgR) {
				continue;
			}

			for (int c = 0; c < sourceSize; ++c) {
				if (c == tgC) {
					continue;
				}

				dest.matrix[destRow][destCol] = source.matrix[r][c];
				++destCol;
			}

			++destRow;
		}
	}
};

int main()
{
	Matrix mat1{ 3 };
	std::string oper{ };
	Matrix mat2{ 3 };

	mat1.print();

	Matrix newMat = mat1.expansion();

	newMat.print();

	std::cout << newMat.det();

	//while (true) {
	//	char c{ };
	//	std::cin >> c;

	//	if (isdigit(c)) {
	//		int r = static_cast<int>(c - 48);
	//		if (r > 9 or r < 1) {
	//			std::cout << "숫자가 너무 크거나 작습니다." << std::endl;
	//			continue;
	//		}

	//		mat1.print();
	//		std::cout << "* 2 = " << std::endl;
	//		mat1 *= r;
	//		mat1.print();
	//		continue;
	//	}

	//	switch (c) {
	//	case 'm':
	//		break;
	//	case 'a':
	//		break;
	//	case 'd':
	//		break;

	//	}
	//}

	return 0;
}