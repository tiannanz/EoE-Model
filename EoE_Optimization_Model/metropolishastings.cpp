#include "stdafx.h"
#include "metropolishastings.h"
#include "random_aa.h"
#include "global_parameters.h"
#include <fstream>

typedef unsigned int uint;

Metropolis_Hastings::Metropolis_Hastings() :
	warmup_sets(1000),
	step_multiplier(.45),
	print_all(true)
{
	// size all vectors to access directly 
	hold_probabilities.resize(Parameters::PSA_ITERATIONS);
	duplicate.resize(Parameters::PSA_ITERATIONS);

	beta_dist_of_probs.resize(NUMBER_OF_PSA_PARAMS);
	step_test_probabilities.resize(NUMBER_OF_PSA_PARAMS);
	hold_delta_generators.resize(NUMBER_OF_PSA_PARAMS);
	reference_probabilities.resize(NUMBER_OF_PSA_PARAMS);

	beta_dist_of_probs[P_MILK_OR_DAIRY] = beta_distribution<>(45, (100 - 45));
	beta_dist_of_probs[P_WHEAT] = beta_distribution<>(65, (100 - 65));
	beta_dist_of_probs[P_EGG] = beta_distribution<>(25, (100 - 25));
	beta_dist_of_probs[P_LEGUMES_SLASH_SOY] = beta_distribution<>(15, (100 - 15));
	beta_dist_of_probs[P_SEAFOOD] = beta_distribution<>(14, (100 - 14));
	beta_dist_of_probs[P_NUTS] = beta_distribution<>(10, (100 - 10));
	beta_dist_of_probs[P_UNKNOWN_ALLERGY] = beta_distribution<>(15, (100 - 15));
	beta_dist_of_probs[P_ONE_ALLERGY] = beta_distribution<>(50, (100 - 50));
	beta_dist_of_probs[P_TWO_ALLERGY] = beta_distribution<>(25, (100 - 25));

	for (uint i = 0; i < NUMBER_OF_PSA_PARAMS; i++)
	{
		step_test_probabilities[i] = mean(beta_dist_of_probs[i]);
		reference_probabilities[i] = mean(beta_dist_of_probs[i]);
		hold_delta_generators[i] = 0; 
	}

	add_psa_probs_to_prob_object(step_test_probabilities, hold_probabilities[0]);
	duplicate[0] = false; 
	print_set_final(step_test_probabilities);
	print_set_all(step_test_probabilities);
}

void Metropolis_Hastings::add_psa_probs_to_prob_object(std::vector<double> const &psa,
	Probabilities &p)
{
	p.p_trigger_milk_dairy = psa[P_MILK_OR_DAIRY];
	p.p_trigger_wheat = psa[P_WHEAT];
	p.p_trigger_eggs = psa[P_EGG];
	p.p_trigger_legume_soy = psa[P_LEGUMES_SLASH_SOY];
	p.p_trigger_seafood = psa[P_SEAFOOD];
	p.p_trigger_nuts = psa[P_NUTS];
	p.p_eliminate_allergies_still_unknown = psa[P_UNKNOWN_ALLERGY];
	p.p_one_allergy = psa[P_ONE_ALLERGY];
	p.p_two_allergies = psa[P_TWO_ALLERGY];

	p.p_three_or_more = 1 - p.p_two_allergies - p.p_one_allergy;
}

void Metropolis_Hastings::generate_step_sizes()
{
	for (uint i = 0; i < NUMBER_OF_PSA_PARAMS; i++)
	{
		double step_deviation = step_multiplier*standard_deviation(beta_dist_of_probs[i]);
		hold_delta_generators[i] = normal_rng(0, step_deviation);
	}
}

void Metropolis_Hastings::step_test_set_parameters()
{
	for (uint i = 0; i < NUMBER_OF_PSA_PARAMS; i++)
	{
		step_test_probabilities[i] += hold_delta_generators[i];
	}
}

double Metropolis_Hastings::probability_of_set(std::vector<double> const &probs)
{
	double alpha_return = 1;

	for (uint i = 0; i < NUMBER_OF_PSA_PARAMS; i++)
	{
		alpha_return *= pdf(beta_dist_of_probs[i], probs[i]);
	}

	return alpha_return;
}

void Metropolis_Hastings::print_set_all(std::vector<double> const &probs)
{
	static bool not_yet_called = true;
	if (not_yet_called)
	{
		std::ofstream parameter_output;
		parameter_output.open("../Results/all_sensitivity_parameter_sets.csv");
		//header file
		parameter_output << "p_milk_dairy, p_wheat, p_eggs, p_legumes_soy, p_seafood, p_nuts, \
p_unknown_allergy, p_one_allergy, p_two_allergy,";
		parameter_output.close();
		not_yet_called = false;
	}

	std::ofstream output;
	output.open("../Results/all_sensitivity_parameter_sets.csv", std::ios::app);

	output << "\n";
	for (uint i = 0; i < NUMBER_OF_PSA_PARAMS; i++)
	{
		output << probs[i] << ",";
	}

	output.close();
}

void Metropolis_Hastings::print_set_final(std::vector<double> const &probs)
{
	static bool not_yet_called = true;
	if (not_yet_called)
	{
		std::ofstream parameter_output;
		parameter_output.open("../Results/final_sensitivity_parameter_sets.csv");
		//header file
		parameter_output << "p_milk_dairy, p_wheat, p_eggs, p_legumes_soy, p_seafood, p_nuts, \
p_unknown_allergy, p_one_allergy, p_two_allergy,";
		parameter_output.close();
		not_yet_called = false;
	}

	std::ofstream output;
	output.open("../Results/final_sensitivity_parameter_sets.csv", std::ios::app);

	output << "\n";
	for (uint i = 0; i < NUMBER_OF_PSA_PARAMS; i++)
	{
		output << probs[i] << ",";
	}
	output.close();
}

void Metropolis_Hastings::run_metropolis_algorithm()
{
	int warmup = 0;

	while (warmup < warmup_sets)
	{
		double alpha_ref = probability_of_set(reference_probabilities);
		generate_step_sizes();
		step_test_set_parameters();
		double alpha_new = probability_of_set(step_test_probabilities);
		double acceptance_prob = alpha_new / alpha_ref;
		double x = uniform_rng();
		if (x < acceptance_prob)
		{
			reference_probabilities = step_test_probabilities;
		}
		else
		{
			step_test_probabilities = reference_probabilities;
		}
		print_set_all(reference_probabilities);
		warmup++;
	}
	
	for (int j = 1; j < Parameters::PSA_ITERATIONS; j++)
	{
		double alpha_ref = probability_of_set(reference_probabilities);
		generate_step_sizes();
		step_test_set_parameters();
		double alpha_new = probability_of_set(step_test_probabilities);
		double acceptance_prob = alpha_new / alpha_ref;
		double x = uniform_rng();
		if (x < acceptance_prob)
		{
			reference_probabilities = step_test_probabilities;
			duplicate[j] = false; 
		}
		else
		{
			step_test_probabilities = reference_probabilities;
			duplicate[j] = true; 
		}

		add_psa_probs_to_prob_object(reference_probabilities,
			hold_probabilities[j]);

		print_set_all(reference_probabilities);
		print_set_final(reference_probabilities);
	}
}
