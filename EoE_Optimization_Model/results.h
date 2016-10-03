#pragma once
#include "stdafx.h"
#include <string>
#include "simulation.h"
#include <array>

class Strategy_Results
{
public:
	Strategy_Results();
	int total_patients; 
	int patients_with_unknown;
	int patients_we_simulate_whole_strategy;
	int total_egd;
	int total_egd_without_unknown;
	std::array<int, NUMBER_OF_ALLERGIES> patients_with_allergy; //+1 for unknown
	std::array<int, NUMBER_OF_ALLERGIES> patients_with_multiple_allergies; //+1 for unknown

	void print_results(std::string const &strategy_tag);
};

