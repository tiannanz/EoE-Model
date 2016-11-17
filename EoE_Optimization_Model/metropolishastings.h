#pragma once
#include "stdafx.h"
#include "simulation.h"
#include <vector>
#include <boost/math/distributions/beta.hpp>

using namespace boost::math;

enum PSA_PARAMS
{
	P_MILK_OR_DAIRY,
	P_WHEAT,
	P_EGG,
	P_LEGUMES_SLASH_SOY,
	P_SEAFOOD,
	P_NUTS,
	P_UNKNOWN_ALLERGY,
	P_ONE_ALLERGY,
	P_TWO_ALLERGY,
	NUMBER_OF_PSA_PARAMS
};

class Metropolis_Hastings
{
public:
	Metropolis_Hastings(); //constructor just for warmup sets right now 
private:
	int warmup_sets;
	double step_multiplier; 
	bool print_all; 
private:
	std::vector<beta_distribution<>> beta_dist_of_probs;
	std::vector<double> step_test_probabilities; 
	std::vector<double> reference_probabilities; 
	std::vector<double> hold_delta_generators;
private: 
	void add_psa_probs_to_prob_object(std::vector<double> const &psa,
		Probabilities &p);
	void generate_step_sizes();
	void step_test_set_parameters();
	double probability_of_set(std::vector<double> const &probs);
	void print_set_all(std::vector<double> const &probs);
	void print_set_final(std::vector<double> const &probs);
public:
	std::vector<Probabilities> hold_probabilities; 
	std::vector<bool> duplicate;
	void run_metropolis_algorithm();
};
