#include "functions.h"
#include <iostream>
#include <time.h>
#include <string.h>
#include <chrono>

#define max_dict_size 500000
#define max_term_size 20000
#define matrix_size 100

int main() 
{
	srand(time(NULL));

	const std::string dictionary_filename = "dictionary.txt";
	std::vector<std::string> dictionary(max_dict_size);
	read_dict_from_file(dictionary_filename, dictionary); // 2.3s
	size_t dict_size = dictionary.size();

	const std::string text_filename = "text1.txt";	
	std::vector<std::string> text(max_term_size);	
	read_from_text_file(text_filename, text);
	size_t text_size = text.size();

	std::hash<std::string> hasher;
	std::vector<std::vector<size_t>> duplicates;
	std::vector<size_t> dict_hashes(dict_size);
	init_hashes(dict_hashes, hasher, dictionary);
	correct_dict_hashes_and_extract_duplicates(duplicates,dict_hashes); //4s

	std::vector<size_t> text_hashes(text_size);
	init_hashes(text_hashes, hasher, text);
	correct_text_hashes(text_hashes, text, duplicates, dictionary);

	std::vector<std::bitset<100>> matrices(dict_size);
	init_random_matrices(matrices);

	std::vector<std::pair<size_t,size_t>> hash_table(dict_size);
	hash_function(hash_table, dict_hashes);

	std::bitset<100> output(matrix_size);
	search_and_calculate_matrices(output, text_hashes, hash_table, matrices, dictionary);


	std::cout << std::endl;
	std::cout << "Dictionary size = " << dict_size << std::endl;
	std::cout << "Input Words count = " << text_hashes.size() << std::endl;
	//std::cout << "The result is" << std::endl;
	//print_bitset(output);
	std::cout << std::endl;
	return 0;
}