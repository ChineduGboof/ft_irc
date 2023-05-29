/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 23:47:23 by gboof             #+#    #+#             */
/*   Updated: 2023/05/29 16:19:02 by cegbulef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

namespace utils {
    
    int atoi(const std::string& val) {
        size_t pos = 0;
        int num = std::stoi(val, &pos);

        if (pos != val.size())
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
}   // namespace utils
