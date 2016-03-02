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

// Sub-matrix block coords in initial matrix
typedef struct __matrix_coords_t {
	struct {
		uint16_t x, y;
	}	lt,	// Left-top
		lb,	// Left-bottom
		rt,	// right-top
		rb;	// right-bottom
} matrix_coords;

const uint16_t NUM_OF_THREADS_DEFAULT = static_cast<uint16_t>(
	std::thread::hardware_concurrency());
const uint16_t MATRIX_DIM_X_DEFAULT = 42;
const uint16_t MATRIX_DIM_Y_DEFAULT = 42;

#define GET_FLAT_INDEX(mat, dim_size, dx, dy) mat[dim_size * dy + dx]

constexpr int32_t slimMatrixSize(int32_t, int32_t);
void fillMatrix(int32_t*, const uint32_t, const uint32_t);

int main(int argc, char *argv[]) 
{
	uint16_t num_of_threads = (argc > 1 ? 
		static_cast<uint16_t>(std::atoi(argv[1])) : ::NUM_OF_THREADS_DEFAULT);
	uint16_t dim_x = (argc > 2 ?
		static_cast<uint16_t>(std::atoi(argv[2])) : ::MATRIX_DIM_X_DEFAULT);
	uint16_t dim_y = (argc > 3 ?
		static_cast<uint16_t>(std::atoi(argv[3])) : ::MATRIX_DIM_Y_DEFAULT);
	int32_t *matL = nullptr, *matR = nullptr, *matRes = nullptr;

	std::cout << "Using threads count: " << num_of_threads << std::endl;
	std::cout << "Matrix dimesions: A = " << dim_x << "x" << dim_y;
	std::cout << "; B = " << dim_y << "x" << dim_x << std::endl;

	// calculate result matrix size


	std::cout << "Allocating memory..." << std::endl;
	try {
		matL = new int32_t[::slimMatrixSize(dim_x, dim_y)];
		matR = new int32_t[::slimMatrixSize(dim_x, dim_y)];
	}
	catch (std::bad_alloc &e) {
		std::cout << "Memory allocation error!" << std::endl;
#if VERBOSE_CERR
		std::cerr << "Message: " << e.what() << std::endl;
#endif
		delete[] matL;
		delete[] matR;
		std::cin.get();
		std::exit(EXIT_FAILURE);
	}
	std::cout << "Filling matrices..." << std::endl;
	std::cout << "Fill matrix " << VAR_STRING(matL) << std::endl;
	::fillMatrix(matL, 0, dim_x * dim_y);
	std::cout << "Fill matrix " << VAR_STRING(matR) << std::endl;
	::fillMatrix(matR, 0, dim_x * dim_y);

	// preparing threads data

	// clear memory and exit
	std::cout << "Delete allocated memory..." << std::endl;
	delete[] matL;
	delete[] matR;
	std::cin.get();
	return EXIT_SUCCESS;
}
// Simple calculatation of constant with multiply of dimensions sizes
constexpr int32_t slimMatrixSize(int32_t _dim_x, int32_t _dim_y)
{
	return _dim_x * _dim_y;
}

// Filling matrices with random values
void fillMatrix(int32_t *flatMatrix, const uint32_t offset, 
	const uint32_t elemCount)
{
	// not need for seeding (aka srand(time(NULL)))
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int32_t> dist(INT16_MIN, INT16_MAX);
	for (uint32_t index = offset; index < elemCount; index++)
	{
		flatMatrix[index] = dist(mt);
#if VERBOSE_CERR
		std::cerr << "Generated number " << flatMatrix[index];
		std::cerr << " in index \'" << index << "\'" << std::endl;
#endif
	}
}