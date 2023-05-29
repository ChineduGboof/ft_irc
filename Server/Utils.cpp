/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 23:47:23 by gboof             #+#    #+#             */
/*   Updated: 2023/05/29 13:14:22 by cegbulef         ###   ########.fr       */
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

}
