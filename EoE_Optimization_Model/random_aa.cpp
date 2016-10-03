#include "stdafx.h"
#include "random_aa.h"
#include <boost/random.hpp>

//int seed = 7; //Whatever seed your heart desires 
//std::mt19937 gen(seed);

int seed_boost = 100;
boost::mt19937 gen(seed_boost);

double uniform_rng()
{
	double a;
	boost::random::uniform_real_distribution<> dis(0, 1);
	a = dis(gen);
	return a;
}

double uniform_rng(double lower, double upper)
{
	double a;
	boost::random::uniform_real_distribution<> dis(lower, upper);
	a = dis(gen);
	return a;
}

double normal_rng(double mean, double standard_deviation)
{
	double a;
	boost::random::normal_distribution<> dis(mean, standard_deviation);
	a = dis(gen);
	return a;
}

int uniform_int_rng(int lower, int upper)
{
	int a;
	boost::random::uniform_int_distribution<> dis(lower, upper);
	a = dis(gen);
	return a;
}

bool check_small_double_equality(double a, double b)
{
	double epsilon = std::numeric_limits<double>::epsilon();
	return (abs(b - a) <= 5*epsilon);
}

double gamma_rng(double alpha, double beta)
{
	double a; 
	boost::random::gamma_distribution<> dis(alpha, beta);
	a = dis(gen);
	return a;
}

double beta_rng(double alpha, double beta)
{
	double answer; 
	double x = gamma_rng(alpha, 1);
	double y = gamma_rng(beta, 1);
	answer = x / (x + y);
	return answer; 
}