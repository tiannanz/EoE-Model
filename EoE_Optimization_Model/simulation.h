#pragma once

#include "stdafx.h"
#include <array>

enum possible_allergies
{
	MILK_OR_DAIRY,
	WHEAT,
	EGG,
	LEGUMES_SLASH_SOY,
	SEAFOOD,
	NUTS,
	NUMBER_OF_ALLERGIES
};

enum allergy_counter
{
	UNKNOWN_ALLERGY,
	ONE,
	TWO,
	THREE_OR_MORE,
	UNINITIALIZED
};

class Metropolis_Hastings; 

class Probabilities //probability of each allergy individually 
{
	friend class Metropolis_Hastings; 
public: //constructor
	Probabilities(); 
public: //public data types
	std::array<double, NUMBER_OF_ALLERGIES> probability_of_rxn;
    /*public probabilities for number of allergies*/ 
	double p_eliminate_allergies_still_unknown;
	double p_one_allergy ;
	double p_two_allergies;
	double p_three_or_more;

private: //private constant values for triggers
	double p_trigger_milk_dairy;
	double p_trigger_wheat;
	double p_trigger_eggs;
	double p_trigger_legume_soy;
	double p_trigger_seafood;
	double p_trigger_nuts;
};

class Patient
{
public: //constructors
	Patient(Probabilities const &probs);
	void reset_patient();
public: //data types
	//from probability of highest allergy --> lowest allergy
	int number_of_endoscopies;
	std::array<bool, NUMBER_OF_ALLERGIES> current_foods_in_diet; 
	std::array<bool, NUMBER_OF_ALLERGIES> patient_allergies;
	std::array<bool, NUMBER_OF_ALLERGIES> allergies_found_in_strategy; 
	allergy_counter how_many_allergies; //intermediate allergy counter 
	int real_num_allergies; //considers case for 3 or more 
private: //initialization functions 
	void generate_patient_allergies(Probabilities const &probs);
	void calculate_number_of_allergies(Probabilities const &probs); 
};
