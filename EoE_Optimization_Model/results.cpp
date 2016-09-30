#include "stdafx.h"
#include "results.h"
#include <fstream>

Strategy_Results::Strategy_Results()
{
	total_patients = 0;
	patients_with_unknown = 0;
	patients_we_simulate_whole_strategy = 0;
	total_egd = 0;
	total_egd_without_unknown = 0; 
	patients_with_allergy = {};
}

void Strategy_Results::print_results(std::string const &strategy_tag)
{
	std::ofstream file; 
	file.open("../Results/" + strategy_tag + ".csv");
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
		"\n";
	file <<
		total_patients << "," <<
		patients_with_unknown << "," <<
		patients_we_simulate_whole_strategy << "," <<
		total_egd << "," <<
		total_egd_without_unknown << "," << 
		patients_with_allergy[0] << "," << 
		patients_with_allergy[1] << "," << 
		patients_with_allergy[2] << "," << 
		patients_with_allergy[3] << "," << 
		patients_with_allergy[4] << "," << 
		patients_with_allergy[5];
	file.close();
}
