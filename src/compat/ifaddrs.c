/*
 libmapper-esp
 Copyright (C) 2020 Mathias Bredholt

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

#include "esp_netif.h"

int getifaddrs(struct ifaddrs **ifap) {
  // Get first network interface
  esp_netif_t *esp_netif = esp_netif_next(NULL);

  struct ifaddrs *tmp;
  tmp = NULL;

  while (esp_netif) {
    struct ifaddrs *ifa = (struct ifaddrs *)malloc(sizeof(struct ifaddrs));
    struct sockaddr_in *sa;

    memset(ifa, 0, sizeof(struct ifaddrs));

    // Flags
    ifa->ifa_flags = esp_netif_is_netif_up(esp_netif) ? IFF_UP : 0;

    // Name
    const int NAME_LENGTH = 6;
    ifa->ifa_name = malloc(NAME_LENGTH);
    esp_netif_get_netif_impl_name(esp_netif, ifa->ifa_name);

    // Address
    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(esp_netif, &ip_info);

    // Add ip address to struct
    ifa->ifa_addr = (struct sockaddr *)malloc(sizeof(struct sockaddr_in));
    sa = (struct sockaddr_in *)ifa->ifa_addr;
    sa->sin_family = AF_INET;
    sa->sin_addr.s_addr = ip_info.ip.addr;

    ifa->ifa_next = NULL;

    // If head save node, else link to previous
    if (tmp == NULL) {
      tmp = ifa;
    } else {
      tmp->ifa_next = ifa;
      tmp = tmp->ifa_next;
    }

    // Get next network interface
    esp_netif = esp_netif_next(esp_netif);
  }

  *ifap = tmp;

  return 0;
}

void freeifaddrs(struct ifaddrs *ifa) {
  struct ifaddrs *p, *q;
  for (p = ifa; p;) {
    free(p->ifa_name);
    if (p->ifa_addr) free(p->ifa_addr);
    if (p->ifa_dstaddr) free(p->ifa_dstaddr);
    if (p->ifa_netmask) free(p->ifa_netmask);
    if (p->ifa_data) free(p->ifa_data);
    q = p;
    p = p->ifa_next;
    free(q);
  }
}
