/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 23:47:20 by gboof             #+#    #+#             */
/*   Updated: 2023/05/29 16:13:55 by cegbulef         ###   ########.fr       */
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
}

#endif