#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include <algorithm>
#include <vector>

// создание генератора случайных чисел
static std::random_device rd;
static std::mt19937 r_gen(rd());

double sech_dist(double alpha, double beta) {
	static std::uniform_real_distribution<> U_dist(0, 1);

	return (log(beta) - log(-log(U_dist(r_gen)))) / alpha;
}

std::vector<double> vector_sech_dist(double alpha, double beta, uint32_t k) {
	std::vector<double> x;
	x.reserve(k);

	for (int i = 0; i < k; ++i) {
		x.push_back(sech_dist(alpha, beta));
	}

	return x;
}

//оценка математического ожидания
double average(const std::vector<double>& vector) {
	double sum = 0;
	for (int i = 0; i < vector.size(); ++i) {
		sum += vector[i];
	}
	return sum / vector.size();
}


std::vector<double> sampling(const std::vector<double>& rand_vector) {   // ф-ция генерации повторной выборки на основе исходной выборки
	
	static std::uniform_int_distribution<> U_dist(0, rand_vector.size() - 1);

	std::vector<double> new_rand_vector;
	new_rand_vector.reserve(rand_vector.size());

	for (int i = 0; i < rand_vector.size(); ++i) {
		new_rand_vector.push_back(rand_vector[U_dist(r_gen)]);
	}

	return new_rand_vector;
}


std::vector<double> exp_val_vector(const std::vector<double>& rand_vector, uint32_t m) {
	std::vector<double> med_vec;
	med_vec.reserve(m);

	for (int i = 0; i < m; ++i) {
		std::vector<double>&& sampl = sampling(rand_vector);
		med_vec.push_back(average(sampl));
	}

	return med_vec;
}


double quantile(std::vector<double>& rand_vector, double p) {     // ф-ция вычисления квантиля
	std::sort(rand_vector.begin(), rand_vector.end());

	size_t k = rand_vector.size();

	return rand_vector[floor(k * p)];
}

#include <windows.h>

int main() {
	setlocale(LC_ALL, "Russian");
	uint32_t k = 100;
	uint32_t m = 1000;
	double alpha = 3;
	double beta = 1;
	double tau = 0.01;

	std::cout << "Истинное значение мат. ожидания: " << alpha << "\n";

	std::vector<double> x = vector_sech_dist(alpha, beta, k);
	std::cout << "Мат. ожидание по выборке x: " << average(x) << "\n";

	std::vector<double> exp_v = exp_val_vector(x, m);
	std::cout << "Среднее мат. ожидание по m выборкам: " << average(exp_v) << "\n";

	std::cout << "Нижняя граница для математического ожидания: " << quantile(exp_v, tau / 2) << "\n";
	std::cout << "Верхняя граница для матматематиеского ожидания: " << quantile(exp_v, 1 - tau / 2) << "\n";

	return 0;
}