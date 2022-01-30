#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <map>
#include <tuple>
#include <algorithm>
using std::int64_t;

// Debugging Helper
void print(std::vector<std::tuple<char, int64_t, int64_t>>& arg) {
    for(int i = 0; i < arg.size(); ++i) {
        std::cout << std::get<0>(arg[i]) << ' ';
    }
    std::cout << '\n';
}
// End of Debugging Helper

int main()
{
    std::ifstream file_input;
    std::string file_name;
    std::cout << "Masukkan nama file \n";
    std::cout << "Contoh: ../test/tc1.txt \n\n";
    std::cout << ">>> ";
    std::cin >> file_name;
    std::cout << "\n";
    file_input.open(file_name);
    std::vector<std::string> word_matrix;
    std::vector<std::string> word_list;
    std::map<char, std::vector<std::tuple<int64_t, int64_t>>> word_pos;

    int64_t compare_count = 0;

    for(std::string line_in; std::getline(file_input, line_in);)
    {
        if(line_in == "") continue;

        if(line_in[1] == ' ')
        {
            std::string temp;
            for(int64_t i = 0; i < line_in.size(); i += 2)
            {
                temp.push_back(line_in[i]);
            }
            word_matrix.emplace_back(std::move(temp));
        }
        else
        {
            word_list.emplace_back(std::move(line_in));
            auto iter = word_pos.find(word_list.back()[0]);
            if(iter == word_pos.end())
            {
                word_pos[word_list.back()[0]] = std::vector<std::tuple<int64_t, int64_t>>{};
            }
        }
    }

    std::vector<std::vector<std::tuple<char, int64_t, int64_t>>> output;

    auto time_start = std::chrono::steady_clock::now();

    for(int64_t i = 0; i < word_matrix.size(); ++i)
    {
        for(int64_t j = 0; j < word_matrix[i].size(); ++j)
        {
            compare_count++;
            auto iter = word_pos.find(word_matrix[i][j]);
            if(iter != word_pos.end())
            {
                word_pos[word_matrix[i][j]].emplace_back(std::make_tuple(i, j));
            }
        }
    }

    for(int i = 0; i < word_list.size(); ++i)
    {
        std::string word = word_list[i];

        for(int j = 0; j < word_pos[word[0]].size(); ++j)
        {
            int64_t row_num;
            int64_t col_num;
            std::tie(row_num, col_num) = word_pos[word[0]][j];

            bool found = true;
            // Check to the top
            std::vector<std::tuple<char, int64_t, int64_t>> output_temp;
            for(int64_t index = 0; index < word.size(); ++index)
            {
                if(row_num - index < 0)
                {
                    found = false;
                    break;
                }
                compare_count++;
                if(word[index] == word_matrix[row_num-index][col_num])
                {
                    output_temp.emplace_back(std::make_tuple(word[index], row_num - index, col_num));
                }
                else
                {
                    found = false;
                    break;
                }
            }
            if(found)
            {
                output.emplace_back(std::move(output_temp));
                continue;
            }

            output_temp.clear();

            found = true;
            // Check to the left
            for(int64_t index = 0; index < word.size(); ++index)
            {
                if(col_num - index < 0)
                {
                    found = false;
                    break;
                }
                compare_count++;
                if(word[index] == word_matrix[row_num][col_num-index])
                {
                    output_temp.emplace_back(std::make_tuple(word[index], row_num, col_num-index));
                }
                else
                {
                    found = false;
                    break;
                }
            }
            if(found)
            {
                output.emplace_back(std::move(output_temp));
                continue;
            }

            output_temp.clear();

            found = true;
            // Check to the bottom
            for(int64_t index = 0; index < word.size(); ++index)
            {
                if(row_num + index >= word_matrix.size())
                {
                    found = false;
                    break;
                }
                compare_count++;
                if(word[index] == word_matrix[row_num+index][col_num])
                {
                    output_temp.emplace_back(std::make_tuple(word[index], row_num + index, col_num));
                }
                else
                {
                    found = false;
                    break;
                }
            }
            if(found)
            {
                output.emplace_back(std::move(output_temp));
                continue;
            }

            output_temp.clear();

            found = true;
            // Check to the right
            for(int64_t index = 0; index < word.size(); ++index)
            {
                if(col_num + index >= word_matrix[0].size())
                {
                    found = false;
                    break;
                }
                compare_count++;
                if(word[index] == word_matrix[row_num][col_num+index])
                {
                    output_temp.emplace_back(std::make_tuple(word[index], row_num, col_num+index));
                }
                else
                {
                    found = false;
                    break;
                }
            }
            if(found)
            {
                output.emplace_back(std::move(output_temp));
                continue;
            }

            output_temp.clear();

            found = true;
            // Check to top-left
            for(int64_t index = 0; index < word.size(); ++index)
            {
                if(col_num - index < 0 || row_num - index < 0)
                {
                    found = false;
                    break;
                }
                compare_count++;
                if(word[index] == word_matrix[row_num-index][col_num-index])
                {
                    output_temp.emplace_back(std::make_tuple(word[index], row_num-index, col_num-index));
                }
                else
                {
                    found = false;
                    break;
                }
            }
            if(found)
            {
                output.emplace_back(std::move(output_temp));
                continue;
            }

            output_temp.clear();

            found = true;
            // Check to top-right
            for(int64_t index = 0; index < word.size(); ++index)
            {
                if(col_num + index >= word_matrix[0].size() || row_num - index < 0)
                {
                    found = false;
                    break;
                }
                compare_count++;
                if(word[index] == word_matrix[row_num-index][col_num+index])
                {
                    output_temp.emplace_back(std::make_tuple(word[index], row_num-index, col_num+index));
                }
                else
                {
                    found = false;
                    break;
                }
            }
            if(found)
            {
                output.emplace_back(std::move(output_temp));
                continue;
            }

            output_temp.clear();

            found = true;
            // Check to bottom-left
            for(int64_t index = 0; index < word.size(); ++index)
            {
                if(col_num - index < 0 || row_num + index >= word_pos.size())
                {
                    found = false;
                    break;
                }
                compare_count++;
                if(word[index] == word_matrix[row_num+index][col_num-index])
                {
                    output_temp.emplace_back(std::make_tuple(word[index], row_num+index, col_num-index));
                }
                else
                {
                    found = false;
                    break;
                }
            }
            if(found)
            {
                output.emplace_back(std::move(output_temp));
                continue;
            }

            output_temp.clear();

            found = true;
            // Check to bottom-right
            for(int64_t index = 0; index < word.size(); ++index)
            {
                if(col_num + index >= word_matrix[0].size() || row_num + index >= word_matrix.size())
                {
                    found = false;
                    break;
                }
                compare_count++;
                if(word[index] == word_matrix[row_num+index][col_num+index])
                {
                    output_temp.emplace_back(std::make_tuple(word[index], row_num+index, col_num+index));
                }
                else
                {
                    found = false;
                    break;
                }
            }
            if(found)
            {
                output.emplace_back(std::move(output_temp));
                continue;
            }
            output_temp.clear();
        }
    }

    // Return hasil
    for(auto& result : output)
    {
        for(int i = 0; i < word_matrix.size(); ++i)
        {
            for(int j = 0; j < word_matrix[i].size(); ++j)
            {
                auto iter = std::find_if(result.begin(), result.end(), [&i, &j](std::tuple<char, int64_t, int64_t> el)
                {
                    return std::get<1>(el) == i && std::get<2>(el) == j;
                });
                if(iter != result.end())
                {
                    std::cout << std::get<0>(*iter);
                }
                else
                {
                    std::cout << '-';
                }
                std::cout << ' ';
            }
            std::cout << '\n';
        }
        std::cout <<"--------------------------------------------------------------------\n\n";
    }
    auto time_end = std::chrono::steady_clock::now();
    std::cout << "Waktu yang diperlukan untuk mengeksekusi program adalah " << std::chrono::duration_cast<std::chrono::duration<double>>(time_end - time_start).count() << " detik\n";
    std::cout << "Banyak operasi perbandingan kata yang dilakukan program ini adalah " << compare_count << " perbandingan \n\n";
}
