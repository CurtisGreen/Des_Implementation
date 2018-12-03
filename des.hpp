#ifndef DES_H
#define DES_H

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <sstream>
#include <vector>
#include <iomanip>

#include "des_data.hpp"

class Des {
public:
	void run_des(bool, bool, std::string, std::string, bool, bool, bool, bool);

	Des(bool encrypt, bool decrypt, std::string plaintext, std::string key,
	  bool ascii, bool binary, bool hex, bool verbose){
		run_des(encrypt, decrypt, plaintext, key, ascii, binary, hex, verbose);
	}

private:
	bool verbose = false;

	// Utility
	void verbose_print(std::string);
	std::string xor_strings(std::string, std::string);
	std::string string_to_bin(std::string);
	std::string bin_to_string(std::string);
	std::string hex_to_bin(std::string);
	std::string bin_to_hex(std::string);

	// DES functions
	std::string expansion_p_box(std::string);
	std::string ip_permutation(std::string);
	std::string reverse_ip(std::string);
	std::string pc1_permutation(std::string);
	std::string pc2_permutation(std::string);
	std::string shrink_s_box(std::string);
	std::string straight_p_box(std::string);
	std::vector<std::string> shift_key(std::string);

	// Main DES operations
	std::vector<std::string> create_subkeys(std::string);
	std::string mangler(std::string, std::string);
};

#endif