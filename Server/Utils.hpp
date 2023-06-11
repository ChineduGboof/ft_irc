/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 23:47:20 by gboof             #+#    #+#             */
/*   Updated: 2023/06/11 17:19:05 by cegbulef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

# include <string>
# include <sstream>
# include <limits>
# include <exception>
# include <cstdlib>
# include <queue>
# include <ctime>
# include <algorithm>

namespace utils {
    int atoi(const std::string& val);
    std::string whitespace(const std::string& val);
    std::vector<std::string> splitByDelimiter(const std::string& val, const std::string& delim);
    std::vector<std::string> splitBySpace(const std::string& val);
    template <typename T>
    std::string convertToString( const T arg ) {
        std::stringstream str;

        str << arg;
        return str.str();
    };
    
}

#endif