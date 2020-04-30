#include "functions.hpp"
#include <iostream>
#include <time.h>
#include <string.h>
#include <chrono>

#define max_dict_size 500000
#define max_term_size 20000
#define max_stop_words_size 200
#define matrix_size 100

int main() 
{
	auto start = std::chrono::steady_clock::now();
	const std::string dictionary_filename = "dictionary.txt";
	std::vector<std::string> dictionary(max_dict_size);
	read_dict_from_file(dictionary_filename, dictionary); // 2s
	size_t dict_size = dictionary.size();

	const std::string text_filename = "text1.txt";	
	std::vector<std::string> text(max_term_size);	
	read_from_text_file(text_filename, text);
	size_t text_size = text.size();

	const std::string stop_words_filename = "stop_words.txt";
	std::vector<std::string> stop_words(max_stop_words_size);
	read_from_text_file(stop_words_filename, stop_words);
	size_t stop_words_size = stop_words.size();

	std::hash<std::string> hasher;
	std::vector<std::vector<size_t>> duplicates;

	std::vector<size_t> dict_hashes(dict_size);
	init_hashes(dict_hashes, hasher, dictionary);
	correct_dict_hashes_and_extract_duplicates(duplicates,dict_hashes); // 3.7s

	std::vector<size_t> text_single_term_hashes(text_size);
	init_hashes(text_single_term_hashes, hasher, text);
	correct_text_hashes(text_single_term_hashes, text, duplicates, dictionary);
	
	std::vector<size_t> text_double_term_hashes(text_size - 1);
	init_hashes(text_double_term_hashes, hasher, text);
	correct_text_hashes(text_double_term_hashes, text, duplicates, dictionary);

	std::vector<size_t> stop_words_hashes(stop_words_size);
	init_hashes(stop_words_hashes, hasher, stop_words);
	correct_text_hashes(stop_words_hashes, stop_words, duplicates, dictionary);

	std::vector<std::bitset<100>> matrices(dict_size);
	init_random_matrices(matrices); // 1.4

	std::vector<std::pair<size_t,size_t>> hash_table(dict_size);
	hash_function(hash_table, dict_hashes); // 4.8

	/*std::vector<std::pair<size_t, size_t>> stop_words_hash_table(stop_words_size);
	hash_function(stop_words_hash_table, stop_words_hashes);*/

	std::bitset<100> output(matrix_size);
	std::vector<size_t> words_count;
	search_and_calculate_matrices(output, text_single_term_hashes, text_double_term_hashes, 
		stop_words_hashes, hash_table, matrices, dictionary);  // 4.7

	//extract_words_count(words_count, dictionary);
	std::cout << std::endl;
	std::cout << "Dictionary size = " << dict_size << std::endl;
	std::cout << "Input Words count = " << text_single_term_hashes.size() << std::endl;
	std::cout << "The result is" << std::endl;
	print_bitset_vector(output);
	std::cout << std::endl;
	auto end = std::chrono::steady_clock::now();
	std::cout << "Elapsed time in milliseconds : "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << std::endl;
	return 0;
}