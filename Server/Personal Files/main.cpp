/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gboof <gboof@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 21:30:02 by gboof             #+#    #+#             */
/*   Updated: 2023/05/24 23:39:49 by gboof            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

// int main()
// {
//     struct sockaddr_in sa; // IPv4
//     struct sockaddr_in6 sa6; // IPv6

//     inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr)); // IPv4
//     inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr)); // IPv6


//     // IPv4:

//     char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string
//     struct sockaddr_in sa;      // pretend this is loaded with something

//     inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);

//     printf("The IPv4 address is: %s\n", ip4);


//     // IPv6:

//     char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
//     struct sockaddr_in6 sa6;    // pretend this is loaded with something

//     inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);

//     printf("The address is: %s\n", ip6);



//     int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP
//                 const char *service,  // e.g. "http" or port number
//                 const struct addrinfo *hints,
//                 struct addrinfo **res);
// }






