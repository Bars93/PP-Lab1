#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <cstdint>
#include <vector>
#include <random>
#include <exception>

#define VERBOSE_CERR 1
#define VAR_STRING(x) "\'" ## #x ## "\'"
typedef std::vector<std::thread> vThread;

const uint16_t NUM_OF_THREADS_DEFAULT = static_cast<uint16_t>(
	std::thread::hardware_concurrency());
const uint16_t MATRIX_DIM_X = 42;
const uint16_t MATRIX_DIM_Y = 42;

#define SLIM_INDEX_BY_ROW(mat, dx, dy) mat[MATRIX_DIM_X * dy + dx]
#define SLIM_INDEX_BY_COL(mat, dx, dy) mat[MATRIX_DIM_Y * dx + dy]
constexpr int32_t slimMatrixSize(int32_t _dim_x, int32_t _dim_y) 
{
	return _dim_x * _dim_y;
}
void fillMatrix(int32_t *slimMatrix, const uint32_t offset, const uint32_t elemCount) 
{
	// not need for seeding (aka srand(time(NULL)))
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int32_t> dist(INT16_MIN, INT16_MAX);
	for (uint32_t index = offset; index < elemCount; index++) 
	{
		slimMatrix[index] = dist(mt);
#if VERBOSE_CERR
		std::cerr << "Generated number " << slimMatrix[index];
		std::cerr << " in index \'" << index << "\'" << std::endl;
#endif
	}
}
int main(int argc, char *argv[]) 
{
	uint16_t num_of_threads = (argc > 1 ? 
		static_cast<uint16_t>(std::atoi(argv[1])) : ::NUM_OF_THREADS_DEFAULT);
	vThread threads;
	int32_t *matL = nullptr, *matR = nullptr;
	std::cout << "Allocating memory..." << std::endl;
	try {
		matL = new int32_t[::slimMatrixSize(MATRIX_DIM_X, MATRIX_DIM_Y)];
		matR = new int32_t[::slimMatrixSize(MATRIX_DIM_X, MATRIX_DIM_Y)];
	}
	catch (std::bad_alloc &e) {
		std::cout << "Memory allocation error!" << std::endl;
#if VERBOSE_CERR
		std::cerr << e.what() << std::endl;
#endif
		delete[] matL;
		delete[] matR;
		std::cin.get();
		std::exit(EXIT_FAILURE);
	}
	std::cout << "Filling matrices..." << std::endl;
	std::cout << "Fill matrix " << VAR_STRING(matL) << std::endl;
	::fillMatrix(matL, 0, MATRIX_DIM_X * MATRIX_DIM_Y);
	std::cout << "Fill matrix " << VAR_STRING(matR) << std::endl;
	::fillMatrix(matR, 0, MATRIX_DIM_X * MATRIX_DIM_Y);


	std::cout << "Delete allocated memory..." << std::endl;
	delete[] matL;
	delete[] matR;
	std::cin.get();
	return EXIT_SUCCESS;
}