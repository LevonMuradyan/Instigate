#include "functions.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <random>
#include <string.h>
#include <chrono>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

void init_logging()
{
    logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

    logging::add_file_log(
        keywords::file_name = "logfile.log",
        keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] [%ProcessID%] [%LineID%] %Message%"
    );

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::trace
    );

    logging::add_common_attributes();
}

void init_random_matrices(std::vector<std::bitset<100>>& matrices)
{
	auto start = std::chrono::steady_clock::now();
	size_t dict_size = matrices.size();
	std::default_random_engine dre(std::random_device{}());
	std::uniform_int_distribution<long long> dist(0, (1ll << 50) - 1);
	for (size_t i = 0; i < dict_size; ++i)
	{	
		matrices[i] = dist(dre);
		matrices[i] <<= 50;
		matrices[i] |= dist(dre);
	}
	auto end = std::chrono::steady_clock::now();
	BOOST_LOG_TRIVIAL(debug) << "init_random_matrices()  "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";
}

void read_dict_from_file(const std::string& dictionary_filename, std::vector<std::string>& dictionary)
{
	auto start = std::chrono::steady_clock::now();
	std::ifstream file(dictionary_filename);
	size_t counter = 0;
	if (file.is_open())
	{
		std::string line;
		size_t i = 0;
		while (std::getline(file, line))
		{
			dictionary[i++] = line;
			counter++;
		}
		dictionary.resize(counter);
		file.close();
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't open " << dictionary_filename << " for reading";
		std::cerr << "Couldn't open " << dictionary_filename << " for reading\n";
	}
	auto end = std::chrono::steady_clock::now();
	BOOST_LOG_TRIVIAL(debug) << "read_dict_from_file()  "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";
}

void read_text_from_file(const std::string& text_filename, std::vector<std::string>& text)
{
	auto start = std::chrono::steady_clock::now();
	std::ifstream file(text_filename);
	if (file.is_open())
	{
		std::string line;
		size_t i = 0, counter = 0;
		while (std::getline(file, line))
		{
			//std::cout << line << std::endl;
			//std::cout << line.size() << std::endl;
			size_t size = line.size();; 
			if(line[size - 1] < 'A' || line[size - 1] > 'Z' && 
				line[size - 1] < 'a' || line[size - 1] > 'z')
			{
				line.pop_back();
			}
				
			char* str = &line[0];
			char* pch;
			pch = strtok(str, " ,.-?");

			//std::cout << line << std::endl;
			//std::cout << line.size() << std::endl;
			
			while (pch != NULL)
			{
				for (size_t j = 0; j < strlen(pch); j++)
				{
					if (pch[j] >= 65 && pch[j] <= 92)
					{
						pch[j] = pch[j] + 32;
					}
				}
				//std::cout << pch << std::endl;
				//std::cout << strlen(pch) << std::endl;
				//std::cout << strlen(pch) << std::endl;
				text[i++] = pch;
				pch = strtok(NULL, " ,.-?");
				counter++;
			}			
		}
		text.resize(counter);
		file.close();
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "Couldn't open " << text_filename << " for reading";
		std::cerr << "Couldn't open " << text_filename << " for reading\n";
	}

	auto end = std::chrono::steady_clock::now();
	BOOST_LOG_TRIVIAL(debug) << "read_from_text_file()  "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";
}

void init_hashes(std::vector<size_t>& hashes, const std::hash<std::string>& hasher,
	const std::vector<std::string>& words)
{
	auto start = std::chrono::steady_clock::now();
	size_t size = hashes.size();
	if (size == words.size() - 2)
	{
		for (size_t i = 0; i < size; ++i)
		{
			hashes[i] = hasher(words[i] + "-" + 
							words[i + 1] + "-" + 
							words[i + 2]);
		}
	}
	else if (size == words.size() - 1)
	{
		for (size_t i = 0; i < size; ++i)
		{
			hashes[i] = hasher(words[i] + "-" + 
							words[i + 1]);
		}
	}
	else
	{
		for (size_t i = 0; i < size; ++i)
		{
			hashes[i] = hasher(words[i]);
		}
	}
	
	auto end = std::chrono::steady_clock::now();
	BOOST_LOG_TRIVIAL(debug) << "init_hashes()  "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";
}

void correct_dict_hashes_and_extract_duplicates(
	std::vector<std::vector<size_t>>& duplicates, 
	std::vector<size_t>& hashes)
{
	auto start = std::chrono::steady_clock::now();
	size_t size = hashes.size();
	std::map<size_t, size_t> m;

	if (size < 2)
	{
		return;
	}

	for (size_t i = 0; i < size; ++i)
	{
		m[hashes[i]] = 0;
	}
	for (size_t i = 0; i < size; ++i)
	{
		if (m[hashes[i]])
		{
			size_t temp = hashes[i];
			while (m[temp])
			{
				temp++;
			}
			duplicates.push_back({ hashes[i], i, temp });
			hashes[i] = temp;
		}
		else
		{
			m[hashes[i]] = 1;
		}
	}
	auto end = std::chrono::steady_clock::now();
	BOOST_LOG_TRIVIAL(debug) << "correct_dict_hashes_and_extract_duplicates()  "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";
}

void correct_text_hashes(std::vector<size_t>& text_hashes,
	const std::vector<std::string>& text,
	const std::vector<std::vector<size_t>>& duplicates,
	const std::vector<std::string>& dictionary)
{
	auto start = std::chrono::steady_clock::now();
	size_t duplicate_size = duplicates.size();
	size_t text_size = text_hashes.size();
	for (size_t i = 0; i < duplicates.size(); ++i)
	{
		for (size_t j = 0; j < text_size; ++j)
		{
			if (text_hashes[j] == duplicates[i][0])
			{
				if (text[j] == dictionary[duplicates[i][1]])
				{
					text_hashes[j] = duplicates[i][2];
				}
			}
		}
	}
	auto end = std::chrono::steady_clock::now();
	BOOST_LOG_TRIVIAL(debug) << "correct_text_hashes()  "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";
}

void hash_function(std::vector<std::pair<size_t, size_t>>& hash_table,
	const std::vector<size_t>& hashes)
{
	auto start = std::chrono::steady_clock::now();
	size_t words_count = hashes.size();
	for (size_t i = 0; i < words_count; ++i)
	{
		size_t index = hashes[i] % words_count;
		while (hash_table[index].first)
		{
			index = (index + 1) % words_count;
		}
		hash_table[index] = std::make_pair(hashes[i], i);
	}
	auto end = std::chrono::steady_clock::now();
	BOOST_LOG_TRIVIAL(debug) << "hash_function()  "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";
}

void search_and_calculate_matrices(std::bitset<100>& output,
	std::map<std::string, size_t>& words_count,
	const std::vector<size_t>& text_single_term_hashes,
	const std::vector<size_t>& text_double_term_hashes,
	const std::vector<size_t>& text_triple_term_hashes,
	const std::vector<size_t>& stop_words_hashes,
	const std::vector<std::pair<size_t, size_t>>& hash_table,
	const std::vector<std::bitset<100>>& matrices,
	const std::vector<std::string>& dictionary)
{
	auto start = std::chrono::steady_clock::now();
	size_t text_size = text_single_term_hashes.size();
	size_t dict_size = dictionary.size();
	size_t matrix_size = output.size();
	std::vector<size_t> indexes(text_size);
	std::vector<const std::vector<size_t>*> terms(3);
	terms[0] = &text_single_term_hashes;
	terms[1] = &text_double_term_hashes;
	terms[2] = &text_triple_term_hashes;
	for(size_t k = 3; k > 0; --k)
	{
		for (size_t i = 0; i < text_size - k + 1; ++i)
		{
			bool b = true;
			for(size_t l = 0; l < k; ++l)
			{
				if (indexes[i + l])
				{
					b = false;
				}
			}
			if(k == 1)
			{
				for (size_t l = 0; l < stop_words_hashes.size(); ++l)
				{
					if (stop_words_hashes[l] == (*terms[0])[i])
					{
						indexes[i] = 1;
						b = false;
					}
				}
			}
			if (b)
			{
				size_t index = ((*terms[k - 1])[i]) % dict_size;
				for (size_t j = 0; j < dict_size; ++j)
				{
					if (hash_table[index].first == ((*terms[k - 1])[i]))
					{	
						for(size_t l = 0; l < k; ++l)
						{
							indexes[i + l] = 1;
						}
						output |= matrices[hash_table[index].second];
						size_t& value =	words_count[dictionary[hash_table[index].second]];
						value? value++ : value = 1;	
						break;
					}
					else
					{
						index = (index + 1) % dict_size;
					}
				}
			}
		}
	}

	auto end = std::chrono::steady_clock::now();
	BOOST_LOG_TRIVIAL(debug) << "search_and_calculate_matrices()  "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";
}

void extract_words_count(const std::map<std::string, size_t>& m)
{
	typedef std::function<bool(std::pair<std::string, int>, std::pair<std::string, int>)> Comparator;

	Comparator compFunctor = [](std::pair<std::string, size_t> elem1 ,std::pair<std::string, size_t> elem2)
	{
		return elem1.second > elem2.second;
	};

	std::set<std::pair<std::string, int>, Comparator> setOfWords(
			m.begin(), m.end(), compFunctor);

	// Iterate over a set using range base for loop
	// It will display the items in sorted order of values
	for (std::pair<std::string, int> element : setOfWords)
		BOOST_LOG_TRIVIAL(info) << element.first << " :: " << element.second;
}

void print_hash_table(const std::vector<std::pair<size_t, size_t>>& hash_table, size_t begin, size_t end)
{
	for (size_t i = begin; i < end; ++i)
	{
		//std::cout << "hash = " << hash_table[i].first << "  index = " << hash_table[i].second << std::endl;
		std::cout << hash_table[i].first << " " << hash_table[i].second << std::endl;
	}
}

void print_duplicates(const std::vector<std::vector<size_t>>& duplicates,
	const std::vector<std::string>& dictionary)
{
	for (int i = 0; i < duplicates.size(); ++i)
	{
		for (int j = 0; j < duplicates[0].size(); ++j)
		{
			if (j == 1)
			{
				std::cout << "dictionary[" << duplicates[i][j] << "] = " << dictionary[duplicates[i][j]] << " ";
			}
			else
			{
				std::cout << duplicates[i][j] << " ";
			}

		}
		std::cout << std::endl;
	}
}

void print_matrix(const std::vector<size_t>& matrix)
{
	for (size_t j = 0; j < 10; ++j)
	{
		for (size_t i = j * 10; i < j * 10 + 10; ++i)
		{
			std::cout << matrix[i] << " ";
		}
		std::cout << std::endl;
	}
}

void print_words(const std::vector<std::string>& dictionary)
{
	for (size_t i = 0; i < dictionary.size(); ++i)
	{
		std::cout << dictionary[i] << std::endl;
	}
	std::cout << std::endl;
}

void print_vector(const std::vector<size_t>& vector)
{
	for (size_t i = 0; i < vector.size(); ++i)
	{
		std::cout << vector[i];
	}
	std::cout << std::endl;
}

void print_bitset(const std::bitset<100>& bitset)
{
	for (size_t j = 0; j < 10; ++j)
	{
		for (size_t i = j * 10; i < j * 10 + 10; ++i)
		{
			std::cout << bitset[i] << " ";
		}
		std::cout << std::endl;
	}
}

void print_bitset_vector(const std::bitset<100>& bitset)
{
	for (size_t j = 0; j < bitset.size(); ++j)
	{
		std::cout << bitset[j];
	}
	std::cout << std::endl;
}

bool contains_duplicates(std::vector<size_t> a)
{
	if (a.size() < 2)
	{
		return false;
	}
	sort(a.begin(), a.end());
	//std::cout << a[a.size() - 1] << std::endl;
	//std::cout << a[0] << std::endl;
	for (int i = 0; i < a.size() - 1; i++)
	{
		if (a[i] == a[i + 1])
		{
			std::cout << a[i] << std::endl;
		}
	}
	return false;
}

bool contains_duplicates_h(std::vector<size_t> a)
{
	if (a.size() < 2)
	{
		return false;
	}

	for (int i = 0; i < a.size() - 1; i++)
	{
		a[i] = a[i] % a.size();
	}

	sort(a.begin(), a.end());
	//std::cout << a[a.size() - 1] << std::endl;
	//std::cout << a[0] << std::endl;
	for (int i = 0; i < a.size() - 1; i++)
	{
		if (a[i] == a[i + 1])
		{
			std::cout << a[i] << std::endl;
		}
	}
	return false;
}

void init_random_matrices(std::vector<std::vector<size_t>>& matrices)
{
	size_t dict_size = matrices.size();
	size_t matrix_size = matrices[0].size();

	for (size_t i = 0; i < dict_size; ++i)
	{
		for (size_t j = 0; j < matrix_size; ++j)
		{
			matrices[i][j] = rand() % 2;
		}
	}
}

void read_from_file(const std::string& text_filename, std::string& text)
{
	std::ifstream file(text_filename);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			text += line;
			text += " ";
		}
		file.close();
	}
	else
	{
		std::cerr << "Couldn't open " << "text.txt" << " for reading\n";
	}
}

