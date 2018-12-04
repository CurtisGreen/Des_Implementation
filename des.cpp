#include "des.hpp"

//////////////////////////////////////////////
/// 			Utility	
//////////////////////////////////////////////

void Des::verbose_print(std::string text){
	if (verbose){
		std::cout << text << std::endl;
	}
}

// Xors each char in the string
std::string Des::xor_strings(std::string s1, std::string s2){
	if (s1.size() == s2.size()){
		std::string out;
		for (int i = 0; i < s1.size(); i++){
			int temp = int(s1[i])^int(s2[i]);
			out.push_back('0' + temp);
		}
		return out;
	}
	else {
		std::cout << "Xor: Strings are not the same size" << std::endl;
		return s1;
	}
}

// Convert string to binary string
std::string Des::string_to_bin(std::string input){
	std::string output = "";

	// Convert each char to binary and put each binary part into output
	for (int i = 0; i < input.size(); i++){
		output += (std::bitset<8>(input.c_str()[i])).to_string<char, std::string::traits_type,std::string::allocator_type>();
	}

	return output;
}

// Convert a string of binary numbers to their ascii equivalent
std::string Des::bin_to_string(std::string input){
	std::string output;
	std::bitset<8> bits;

	// Convert in 8 bit blocks
	for (int i = 0; i < input.size()/8; i++){
		std::string substring = input.substr(i*8, 8);
		bits = std::bitset<8>(substring.c_str());
		char c = char(bits.to_ulong());
		output += c;
	}

	return output;
}

std::string Des::hex_to_bin(std::string input){
	std::string output;
	for (int i = 0; i < input.size(); i++){
		std::stringstream ss;
		ss << std::hex << input[i];
		unsigned n;
		ss >> n;
		std::bitset<4> b(n);
		output += b.to_string();
	}
	return output;
}

std::string Des::bin_to_hex(std::string input){
	unsigned long long result = 0;
	for (int i = 0; i < input.size(); i++){
		result *= 2;
		result += input[i]== '1' ? 1 : 0;
	}

	std::stringstream ss;
	ss << "0x" << std::hex << std::setw(8) << std::setfill('0') << result;
	return ss.str();
}

//////////////////////////////////////////////
/// 			DES functions
//////////////////////////////////////////////

// Compute p-box expansion
std::string Des::expansion_p_box(std::string input){
	if (input.size() == 32){
		std::string output;
		output.resize(48);

		for (int i = 0; i < 48; i++){
			output[i] = input[Expansion[i]-1];
		}

		return output;
	}
	else {
		std::cout << "Expansion: input not length 32" << std::endl;
		return "";
	}
}

// Initial permutation before mangler
std::string Des::ip_permutation(std::string plaintext){
	if (plaintext.size() == 64){
		std::string output;
		output.resize(64);
		for (int i = 0; i < 64; i++){
			output[i] = plaintext[IP[i]-1];
		}
		return output;
	}
	else {
		std::cout << "IP: Plaintext not length 64" << std::endl;
		return "";
	}

}

// Inverse of ip_permutation
std::string Des::reverse_ip(std::string almost_encrypted_text){
	if (almost_encrypted_text.size() == 64){
		std::string output;
		output.resize(64);
		for (int i = 0; i < 64; i++){
			output[IP[i]-1] = almost_encrypted_text[i];
		}
		return output;
	}
	else {
		std::cout << "IP_inv: almost_encrypted_text not length 64" << std::endl;
		return "";
	}

}

// Position swap according to PC1 table
std::string Des::pc1_permutation(std::string key){
	if (key.size() == 64){
		std::string output;
		output.resize(56);
		for (int i = 0; i < 56; i++){
			output[i] = key[PC1[i]-1];
		}
		return output;
	}
	else {
		std::cout << "PC1: Key not length 64" << std::endl;
		return "";
	}
	
}

// Position swap according to PC2 table
std::string Des::pc2_permutation(std::string key){
	if (key.size() == 56){
		std::string output;
		output.resize(48);
		for (int i = 0; i < 48; i++){
			output[i] = key[PC2[i]-1];
		}
		return output;
	}
	else {
		std::cout << "PC2: Key not length 56" << std::endl;
		return "";
	}
	
}

// S-boxes shrink input from 48 to 32
std::string Des::shrink_s_box(std::string input){
	if (input.size() == 48){
		std::string output;
		for (int i = 0; i < 8; i++){
			// Row is first and last bit
			std::string temp;
			temp += input[i*6];
			temp += input[i*6+5];
			int row = std::stoi(temp, nullptr, 2);

			// Column is middle 4 bits
			temp = input.substr(i*6+1, 4);
			int column = std::stoi(temp, nullptr, 2);

			// Get value conversion from sbox table
			int s_box_out = (int)(SBOX[i][row*16 + column]);

			// Add to output
			output += std::bitset<4>(s_box_out).to_string();
		}
		return output;
	}
	else {
		std::cout << "Shrink: Input length not 48" << std::endl;
		return "";
	}
}

// Position swap according to PBOX table
std::string Des::straight_p_box(std::string input){
	if (input.size() == 32){
		std::string output;
		output.resize(32);
		for (int i = 0; i < input.size(); i++){
			int index = (int)PBOX[i]-1;
			output[i] = input[index];
		}
		return output;
	}
	else {
		std::cout << "Straight: Input length not 32" << std::endl;
		return "";
	}
}

// Shift key 1 or 2 left based on round #
std::vector<std::string> Des::shift_key(std::string input){
	if (input.size() == 28){
		std::vector<std::string> output;
		output.push_back(input);
		std::string prev_round;

		// Shift each round based on the last
		for (int round = 0; round < 16; round++){
			std::string curr_round;
			curr_round.resize(input.size());

			// Shift once, first time
			if (round == 0){
				curr_round[27] = input[0];
				for (int i = 0; i < input.size()-1; i++){
					curr_round[i] = input[i+1];
				}
			}
			// Shift once
			else if (round == 1 || round == 8 || round == 15){
				curr_round[27] = prev_round[0];
				for (int i = 0; i < prev_round.size()-1; i++){
					curr_round[i] = prev_round[i+1];
				}
			}
			// Shift twice
			else {
				curr_round[26] = prev_round[0];
				curr_round[27] = prev_round[1];
				for (int i = 0; i < prev_round.size()-2; i++){
					curr_round[i] = prev_round[i+2];
				}
			}

			// Store round and prep next
			output.push_back(curr_round);
			prev_round = curr_round;
		}
		return output;
	}
	else {
		std::cout << "Shift: Input not size 28" << std::endl;
		return std::vector<std::string>();
	}
}

// 64-bit key -> 16 48-bit subkeys
std::vector<std::string> Des::create_subkeys(std::string key){
	std::string key_permutation = pc1_permutation(key);

	std::string left_perm_key = key_permutation.substr(0, 28);
	std::string right_perm_key = key_permutation.substr(28, 55);

	std::vector<std::string> shifted_left = shift_key(left_perm_key);
	std::vector<std::string> shifted_right = shift_key(right_perm_key);

	// Combine left/right and perm through PC2
	std::vector<std::string> pc2_perms;
	for (int i = 1; i < shifted_right.size(); i++){
		std::string pc2_perm = pc2_permutation(shifted_left[i] + shifted_right[i]);
		pc2_perms.push_back(pc2_perm);
	}
	
	return pc2_perms;
}

std::string Des::mangler(std::string input, std::string key){

	// Expand to 48 bits
	std::string expanded = expansion_p_box(input);

	// Xor with key
	std::string xor_out = xor_strings(expanded, key);

	// Shrink to 32 bits
	std::string s_box_out = shrink_s_box(xor_out);

	// Permutation, no size change
	std::string out = straight_p_box(s_box_out);

	return out;
}

// Input: 64 bit (8 byte) plaintext and key (8 bits of key are redundant)
void Des::run_des(bool encrypt, bool decrypt, std::string text, std::string key,
	bool ascii, bool binary, bool hex, bool verb){

	verbose = verb;

	// Error conditions
	if (encrypt && decrypt){
		std::cout << "Please run with either encrypt(-e) or decypt(-d), not both" << std::endl;
		return;
	}
	else if (!encrypt && !decrypt){
		std::cout << "Please run with either encrypt(-e) or decypt(-d)" << std::endl;
		return;
	}
	else if (!ascii && !binary && !hex){
		std::cout << "Please run with binary(-b), hex(-h), or ascii(-a)" << std::endl;
		return;
	}
	else if (ascii && (text.size() != 8 || key.size() != 8)){
		std::cout << "Please enter an ascii key and text of length 8" << std::endl;
		return;
	}
	else if (hex && (text.size() != 16 || key.size() != 16)){
		std::cout << "Please enter an hex key and text of length 16, do prepend '0x'" << std::endl;
		return;
	}
	else if (binary && (text.size() != 64 || key.size() != 64)){
		std::cout << "Please enter a binary key and text of length 64" << std::endl;
		return;
	}

	// Success conditions
	else if (ascii){
		text = string_to_bin(text);
		key = string_to_bin(key);
		std::cout << "Message in binary: " << text << std::endl;
	}
	else if (hex){
		text = hex_to_bin(text);
		key = hex_to_bin(key);
		std::cout << "Message in binary: " << text << std::endl;
	}

	if (encrypt){

		// Initiate timer
		auto t1 = std::chrono::high_resolution_clock::now();

		// Create 16 subkeys of length 48bits
		std::vector<std::string> subkeys = create_subkeys(key);

		// Encode each 64 bit block
		std::string ip_permed = ip_permutation(text);
		verbose_print("Initial Permutation: " + ip_permed);

		// Split halves
		std::string initial_left = ip_permed.substr(0, 32);
		std::string initial_right = ip_permed.substr(32, 32);
		verbose_print("L0 = " + initial_left);
		verbose_print("R0 = " + initial_right);

		// Run 16 rounds of operations
		std::string left_half;
		std::string right_half;
		for (int round = 0; round < 16; round++){
			verbose_print("Round " + std::to_string(round) + ":");

			if (round == 0){
				std::string mangled = mangler(initial_right, subkeys[0]);
				std::string xor_out = xor_strings(initial_left, mangled);
				left_half = initial_right;
				right_half = xor_out;
			}
			else {
				std::string mangled = mangler(right_half, subkeys[round]);
				std::string xor_out = xor_strings(left_half, mangled);
				left_half = right_half;
				right_half = xor_out;
			}

			verbose_print("K" + std::to_string(round+1) + " = " + subkeys[round]);
			verbose_print("L" + std::to_string(round+1) + " = " + left_half);
			verbose_print("R" + std::to_string(round+1) + " = " + right_half + '\n');
		}

		// Reverse the initial swap & perm
		std::string final_output = reverse_ip(right_half + left_half);

		// Calculate runtime
		auto t2 = std::chrono::high_resolution_clock::now();
		std::cout << "Delta t2-t1: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count() << " nanoseconds" << std::endl;

		std::cout << "\nDES encrypted output: " << final_output << std::endl;
		if (ascii){
			std::cout << "ASCII encrypted output: " << bin_to_string(final_output) << std::endl;
		}
		else if (hex){
			std::cout << "Hex encrypted output: " << bin_to_hex(final_output) << std::endl;
		}
	}
	else if(decrypt){

		// Initiate timer
		auto t1 = std::chrono::high_resolution_clock::now();

		// Create 16 subkeys of length 48bits
		std::vector<std::string> subkeys = create_subkeys(key);

		// Decode each 64 bit block
		std::string ip_permed = ip_permutation(text);
		verbose_print("Initial Permutation: " + ip_permed);

		// Swap halves
		std::string initial_left = ip_permed.substr(32, 32);
		std::string initial_right = ip_permed.substr(0, 32);

		// Reverse 16 rounds
		std::string left_half, right_half;
		for (int round = 15; round >= 0; round--){
			verbose_print("Round " + std::to_string(round) + ":");

			if (round == 15){
				right_half = initial_left;
				std::string mangled = mangler(right_half, subkeys[round]);
				left_half = xor_strings(mangled, initial_right);
			}
			else {
				std::string prev_right = right_half;
				right_half = left_half;
				std::string mangled = mangler(right_half, subkeys[round]);
				left_half = xor_strings(mangled, prev_right);

			}

			verbose_print("K" + std::to_string(round+1) + " = " + subkeys[round]);
			verbose_print("L" + std::to_string(round+1) + " = " + left_half);
			verbose_print("R" + std::to_string(round+1) + " = " + right_half + '\n');
		}

		// Reverse the initial perm
		std::string final_output = reverse_ip(left_half + right_half);

		// Calculate runtime
		auto t2 = std::chrono::high_resolution_clock::now();
		std::cout << "Delta t2-t1: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count() << " nanoseconds" << std::endl;

		std::cout << "\nDES decrypted output " << final_output << std::endl;
		if (ascii){
			std::cout << "ASCII decrypted output " << bin_to_string(final_output) << std::endl;
		}
		else if (hex){
			std::cout << "Hex encrypted output: " << bin_to_hex(final_output) << std::endl;
		}
	}
}