#pragma once
#include "stdafx.h"
#include <string>

class Strategy_Results
{
public:
	Strategy_Results();
	int total_patients; 
	int patients_with_unknown;
	int patients_we_simulate_whole_strategy;
	int total_egd;
	int total_egd_without_unknown;
	std::array<int, 6> patients_with_allergy; 
	void print_results(std::string const &strategy_tag);
};

