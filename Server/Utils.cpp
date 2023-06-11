/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 23:47:23 by gboof             #+#    #+#             */
/*   Updated: 2023/06/11 17:19:18 by cegbulef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

namespace utils {
    
    int atoi(const std::string& val) {
        std::istringstream iss(val);
        int num;
        if (!(iss >> num) || !iss.eof())
            throw std::invalid_argument("Invalid int");
        return num;
    }

    std::string whitespace(const std::string& val) {
        if (val.empty()) {
            throw std::invalid_argument("Password cannot be empty.");
        }

        for (std::size_t i = 0; i < val.length(); ++i) {
            if (std::isspace(static_cast<unsigned char>(val[i]))) {
                throw std::invalid_argument("Password cannot contain whitespace.");
            }
        }
        return val;
    }

    // std::queue<std::string> splitByDelimiter(const std::string& val, const std::string& delim) {
    //     std::queue<std::string> ret;
    //     size_t startPos = std::string::npos;
    //     size_t endPos = 0;

    //     while ((endPos = val.find(delim, startPos + 1)) != std::string::npos) {
    //         std::string word = val.substr(startPos + 1, endPos - startPos - 1);
    //         ret.push(word);
    //         startPos = endPos;
    //     }

    //     // Add the remaining part of the string if any
    //     std::string lastWord = val.substr(startPos + 1);
    //     if (!lastWord.empty())
    //         ret.push(lastWord);

    //     return ret;
    // }


    std::vector<std::string> splitByDelimiter(const std::string& val, const std::string& delim) {
    std::vector<std::string> ret;
    size_t startPos = std::string::npos;
    size_t endPos = 0;

    while ((endPos = val.find(delim, startPos + 1)) != std::string::npos) {
        std::string word = val.substr(startPos + 1, endPos - startPos - 1);
        ret.push_back(word);
        startPos = endPos;
    }

    // Add the remaining part of the string if any
    std::string lastWord = val.substr(startPos + 1);
    if (!lastWord.empty())
        ret.push_back(lastWord);

    return ret;
}


    std::vector<std::string> splitBySpace(const std::string& val) {
        std::vector<std::string> words;
        std::istringstream iss(val);
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }

        return words;
    }

}   // namespace utils

