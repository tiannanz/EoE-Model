#include "stdafx.h"
#include "results.h"
#include <fstream>


Strategy_Results::Strategy_Results() :
	total_patients(0),
	patients_with_unknown(0),
	patients_we_simulate_whole_strategy(0),
	total_egd(0),
	total_egd_without_unknown(0),
	patients_with_allergy{},
	patients_with_multiple_allergies{}
{
}



void Strategy_Results::print_results(std::string const &strategy_tag)
{
	std::ofstream file; 
	file.open("../Results/" + strategy_tag);
	/*header*/
	file <<
		"total patients," <<
		"patients with unknown," <<
		"patients we simulate whole strategy," <<
		"total egd," <<
		"total egd without unknown patients," <<
		"pts with dairy," << "pts with wheat,"<<
		"pts with egg," << "pts with soy,"<<
		"pts with seafood,"<< "pts with nuts," <<
		"pts with 1 allergy," << "pts with 2 alleriges," <<
		"pts with 3 allergies," << "pts with 4 allergies,"<<
		"pts with 5 alleriges,"<<"pts with 6 allergies,"<<
		"average endoscopies not including patients with unknown"
		"\n";
	file <<
		total_patients << "," <<
		patients_with_unknown << "," <<
		patients_we_simulate_whole_strategy << "," <<
		total_egd << "," <<
		total_egd_without_unknown << ",";
	for (int i = 0; i < NUMBER_OF_ALLERGIES; i++)
	{
		file << patients_with_allergy[i] << ",";
	}
	for (int i = 0; i < NUMBER_OF_ALLERGIES; i++)
	{
		file << patients_with_multiple_allergies[i] << ",";
	}
	file << double(total_egd_without_unknown) / patients_we_simulate_whole_strategy;
	file.close();
}
