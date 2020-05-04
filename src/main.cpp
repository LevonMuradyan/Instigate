#include "functions.hpp"
#include <iostream>
#include <time.h>
#include <string.h>
#include <chrono>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <boost/log/trivial.hpp>

#define max_dict_size 500000
#define max_term_size 20000
#define max_stop_words_size 200
#define matrix_size 100

int main()
{
	init_logging();
	
	auto start = std::chrono::steady_clock::now();
	const std::string dictionary_filename = "dictionary.txt";
	std::vector<std::string> dictionary(max_dict_size);
	read_dict_from_file(dictionary_filename, dictionary);
	size_t dict_size = dictionary.size();

	const std::string text_filename = "text1.txt";	
	std::vector<std::string> text(max_term_size);	
	read_text_from_file(text_filename, text);
	size_t text_size = text.size();

	const std::string stop_words_filename = "stop_words.txt";
	std::vector<std::string> stop_words(max_stop_words_size);
	read_text_from_file(stop_words_filename, stop_words);
	size_t stop_words_size = stop_words.size();

	std::hash<std::string> hasher;
	std::vector<std::vector<size_t>> duplicates;

	std::vector<size_t> dict_hashes(dict_size);
	init_hashes(dict_hashes, hasher, dictionary);
	correct_dict_hashes_and_extract_duplicates(duplicates,dict_hashes);
	size_t duplicates_size = duplicates.size();
	
	std::vector<size_t> text_single_term_hashes(text_size);
	init_hashes(text_single_term_hashes, hasher, text);
	if(duplicates_size)
	{
		correct_text_hashes(text_single_term_hashes, text, duplicates, dictionary);
	}
	
	std::vector<size_t> text_double_term_hashes(text_size - 1);
	init_hashes(text_double_term_hashes, hasher, text);
	if(duplicates_size)
	{
		correct_text_hashes(text_double_term_hashes, text, duplicates, dictionary);
	}

	std::vector<size_t> text_triple_term_hashes(text_size - 2);
	init_hashes(text_triple_term_hashes, hasher, text);
	if(duplicates_size)
	{
		correct_text_hashes(text_triple_term_hashes, text, duplicates, dictionary);
	}

	std::vector<size_t> stop_words_hashes(stop_words_size);
	init_hashes(stop_words_hashes, hasher, stop_words);
	if(duplicates_size)
	{
		correct_text_hashes(stop_words_hashes, stop_words, duplicates, dictionary);
	}
	
	std::vector<std::bitset<100>> matrices(dict_size);
	init_random_matrices(matrices);

	std::vector<std::pair<size_t,size_t>> hash_table(dict_size);
	hash_function(hash_table, dict_hashes);

	std::map<std::string, size_t> words_count;
	std::bitset<100> output(matrix_size);
	search_and_calculate_matrices(output, words_count, text_single_term_hashes, text_double_term_hashes,
		text_triple_term_hashes, stop_words_hashes, hash_table, matrices, dictionary);

	extract_words_count(words_count);
	BOOST_LOG_TRIVIAL(info) << "Dictionary size = " << dict_size;
	BOOST_LOG_TRIVIAL(debug) << "Duplicates count = " << duplicates.size();
	BOOST_LOG_TRIVIAL(info) << "Stop words count = " << stop_words.size();
	BOOST_LOG_TRIVIAL(info) << "Input words count = " << text_single_term_hashes.size();
	BOOST_LOG_TRIVIAL(info) << "The result is = " << output;

	auto end = std::chrono::steady_clock::now();
	BOOST_LOG_TRIVIAL(debug) << "Elapsed time in milliseconds : "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";
	return 0;
}