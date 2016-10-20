#include "stdafx.h"
#include "metropolishastings.h"
#include "random_aa.h"
#include "global_parameters.h"

/*
Define the beta distribution as 
alpha = outcomes, and beta = n - outcomes. 
In order to implement into the model, because
boost does not have a native RNG for beta functions, 
we instead use that if x = gamma(a, c) and 
y = gamma(b, c), then x / (x + y ) = beta ( a, b ). 
*/


Metropolis_Hastings::Metropolis_Hastings() :

	warmup_sets(1000),
	hold_probabilities{},
	duplicate{}
{
	hold_probabilities.reserve(Parameters::PSA_ITERATIONS);
	duplicate.reserve(Parameters::PSA_ITERATIONS);
}

density Metropolis_Hastings::generate_new_set(Probabilities &p)
{
	p.p_trigger_milk_dairy = beta_rng(10, 10);
	p.p_trigger_wheat = beta_rng(10, 10);
	p.p_trigger_eggs = beta_rng(10, 10);
	p.p_trigger_legume_soy = beta_rng(10, 10);
	p.p_trigger_seafood = beta_rng(10, 10);
	p.p_trigger_nuts = beta_rng(10, 10);
	return calculate_density(p);
}

density Metropolis_Hastings::calculate_density(Probabilities &p)
{
	double f_x = p.p_trigger_milk_dairy*p.p_trigger_wheat*p.p_trigger_eggs*
		p.p_trigger_legume_soy*p.p_trigger_seafood*p.p_trigger_nuts;
	return f_x; 
}

void Metropolis_Hastings::update_probability_vector(Probabilities const &p, 
	const bool &is_it_a_duplicate)
{
	hold_probabilities.push_back(p);
	duplicate.push_back(is_it_a_duplicate);
}

store_parameter_sets_from_psa Metropolis_Hastings::run_algorithm()
{
	bool not_duplicate = false; 
	bool duplicate = true;

	Probabilities fallback, test_set; 
	update_probability_vector(fallback, not_duplicate); //add initial unaltered set
	density current_fallback_density = generate_new_set(fallback); /*initialization*/

	for (int i = 0; i < Parameters::PSA_ITERATIONS; i++)
	{	
		density possible_density = generate_new_set(test_set);
		acceptance_ratio ar = possible_density / current_fallback_density;
		if (ar >= 1) //immediately accept the new set
		{
			current_fallback_density = possible_density;
			fallback = test_set; //the new fallback is the current test set
			update_probability_vector(fallback, not_duplicate); //update the vector 
		}
		else
		{
			double x = uniform_rng();
			if (x < ar) /*accept*/
			{
				current_fallback_density = possible_density; 
				fallback = test_set; 
				update_probability_vector(fallback, not_duplicate);
			}
 			else /*fail to accept, have a duplicate*/
			{
				update_probability_vector(fallback, duplicate);
			}
		}
	}
}

