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

#include <stdio.h>
#include "esp_netif.h"

int gethostname(char *name, size_t len) {
  esp_netif_t *netif = NULL;
  esp_netif_ip_info_t ip;
  int i = esp_netif_get_nr_of_ifs();
  if (i == 1) {
    netif = esp_netif_next(netif);
    ESP_ERROR_CHECK(esp_netif_get_ip_info(netif, &ip));
    snprintf(name, len, IPSTR, IP2STR(&ip.ip));
  } else {
    snprintf(name, len, "esp32");
  }
  return 0;
}
