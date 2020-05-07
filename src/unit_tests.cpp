#define BOOST_TEST_MODULE functions

#include "functions.hpp"
#include <unordered_set>
#include <bitset>
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#define max_dict_size 500000
#define max_term_size 20000
#define max_stop_words_size 200
#define matrix_size 100

std::hash<std::string> hasher;

const std::string dictionary_filename = "dictionary.txt";
std::vector<std::string> dictionary(max_dict_size);
std::vector<size_t> dict_hashes(max_dict_size);
size_t dict_size;

const std::string text_filename = "text1.txt";
std::vector<std::string> text(max_term_size);
std::vector<size_t> text_single_term_hashes(max_term_size);
std::vector<size_t> text_double_term_hashes(max_term_size);
std::vector<size_t> text_triple_term_hashes(max_term_size);
size_t text_size;

const std::string stop_words_filename = "stop_words.txt";
std::vector<std::string> stop_words(max_stop_words_size);
std::vector<size_t> stop_words_hashes(max_stop_words_size);
size_t stop_words_size;

std::vector<std::vector<size_t>> duplicates;
size_t duplicates_size;

std::vector<std::pair<size_t,size_t>> hash_table(max_dict_size);
std::vector<std::bitset<matrix_size>> matrices(max_dict_size);
std::map<std::string, size_t> words_count;
std::bitset<matrix_size> output(matrix_size);


BOOST_AUTO_TEST_CASE(read_dict_from_file_test)
{
    read_dict_from_file(dictionary_filename, dictionary);
    dict_size = dictionary.size(); 
    BOOST_REQUIRE_LE( dict_size, max_dict_size);
    for(size_t i = 0; i < dict_size; ++i)
    {
        BOOST_CHECK_NE(dictionary[i].size(), 0);
        for(size_t j = 0; j < dictionary[i].size(); ++j)
        {
            // Decimal 32 = [Space] character
            // Decimal 127 = [DEL] character
            // (32, 127) in this area characters are visible
            BOOST_WARN(dictionary[i][j] > 32 &&
                        dictionary[i][j] < 127);
        }
    }
}

BOOST_AUTO_TEST_CASE(read_text_from_file_test)
{
    read_text_from_file(text_filename, text);
    text_size = text.size();      
    BOOST_REQUIRE_LE( text_size, max_term_size);
    for(size_t i = 0; i < text_size; ++i)
    {
        BOOST_CHECK_NE(text[i].size(), 0);
        for(size_t j = 0; j < text[i].size(); ++j)
        {
            // Decimal 32 = [Space] character
            // Decimal 127 = [DEL] character
            // (32, 127) in this area characters are visible
            BOOST_WARN(text[i][j] > 32 &&
                    text[i][j] < 127);
        }
    }

    read_text_from_file(stop_words_filename, stop_words);
    stop_words_size = stop_words.size();
    BOOST_REQUIRE_LE( stop_words_size, max_term_size);
    for(size_t i = 0; i < stop_words_size; ++i)
    {
        BOOST_CHECK_NE(stop_words[i].size(), 0);
        for(size_t j = 0; j < stop_words[i].size(); ++j)
        {
            // Decimal 32 = [Space] character
            // Decimal 127 = [DEL] character
            // (32, 127) in this area characters are visible
            BOOST_WARN(stop_words[i][j] > 32 &&
                    stop_words[i][j] < 127);
        }
    }
}

BOOST_AUTO_TEST_CASE(init_random_matrices_test)
{
    matrices.resize(dict_size);
    init_random_matrices(matrices);
    size_t size = matrices.size();
    std::unordered_set<std::bitset<matrix_size>> un_set(matrices.begin(),matrices.end());
    BOOST_WARN_EQUAL(un_set.size(), size);
}

BOOST_AUTO_TEST_CASE(init_hashes_test)
{	
    dict_hashes.resize(dict_size);
    init_hashes(dict_hashes, hasher, dictionary);
    BOOST_CHECK_EQUAL(dict_hashes.size(), dict_size);
    size_t size = dict_hashes.size();
    std::unordered_set<std::bitset<matrix_size>> un_set(dict_hashes.begin(),dict_hashes.end());
    BOOST_WARN_EQUAL( un_set.size(), size );
    un_set.clear();

    text_single_term_hashes.resize(text_size);
    init_hashes(text_single_term_hashes, hasher, text);
    BOOST_WARN_EQUAL(text_single_term_hashes.size(), text_size);
    size = text_single_term_hashes.size();
    un_set = std::unordered_set<std::bitset<matrix_size>>(text_single_term_hashes.begin(),text_single_term_hashes.end());
    BOOST_WARN_LE( un_set.size(), size );
    un_set.clear();


    text_double_term_hashes.resize(text_size - 1);
    init_hashes(text_double_term_hashes, hasher, text);
    BOOST_CHECK_EQUAL(text_double_term_hashes.size(), text_size - 1);
    size = text_double_term_hashes.size();
    un_set = std::unordered_set<std::bitset<matrix_size>>(text_double_term_hashes.begin(),text_double_term_hashes.end());
    BOOST_WARN_LE( un_set.size(), size );
    un_set.clear();

    text_triple_term_hashes.resize(text_size - 2);
    init_hashes(text_triple_term_hashes, hasher, text);
    BOOST_CHECK_EQUAL(text_triple_term_hashes.size(), text_size - 2);
    size = text_triple_term_hashes.size();
    un_set = std::unordered_set<std::bitset<matrix_size>>(text_triple_term_hashes.begin(),text_triple_term_hashes.end());
    BOOST_WARN_LE( un_set.size(), size );
    un_set.clear();

    stop_words_hashes.resize(stop_words_size);
    init_hashes(stop_words_hashes, hasher, stop_words);
    BOOST_CHECK_EQUAL(stop_words_hashes.size(),stop_words_size);
    size = stop_words_hashes.size();
    un_set = std::unordered_set<std::bitset<matrix_size>>(stop_words_hashes.begin(),stop_words_hashes.end());
    BOOST_WARN_EQUAL( un_set.size(), size );
}

BOOST_AUTO_TEST_CASE(correct_dict_hashes_and_extract_duplicates_test)
{
    correct_dict_hashes_and_extract_duplicates(duplicates,dict_hashes);
    size_t duplicates_size = duplicates.size();
    BOOST_CHECK_EQUAL(duplicates_size, 0);
    BOOST_CHECK_EQUAL(dict_hashes.size(), dict_size);
    size_t size = dict_hashes.size();
    std::unordered_set<std::bitset<matrix_size>> un_set(dict_hashes.begin(),dict_hashes.end());
    BOOST_WARN_EQUAL( un_set.size(), size );
}

BOOST_AUTO_TEST_CASE(hash_function_test)
{	
    hash_table.resize(dict_size);
    hash_function(hash_table, dict_hashes);
    BOOST_CHECK_EQUAL(hash_table.size(), dict_hashes.size());
    for(size_t i = 0; i < dict_size; ++i)
    {
        BOOST_CHECK_NE(hash_table[i].first,0);
    }
}

BOOST_AUTO_TEST_CASE(search_and_calculate_matrices_test)
{	
    std::bitset<matrix_size> temp = output;
    search_and_calculate_matrices(output, words_count, text_single_term_hashes, text_double_term_hashes,
        text_triple_term_hashes, stop_words_hashes, hash_table, matrices, dictionary);
    BOOST_CHECK_NE(temp, output);
}
