#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include "rc4.h"

int check_ip_address() {
    const char *target_ip = "1.2.3.4";
    struct ifaddrs *ifaddr, *ifa;
    char addr[INET_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return 0;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET) {
            continue;
        }

        struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
        inet_ntop(AF_INET, &(sa->sin_addr), addr, INET_ADDRSTRLEN);
        if (strcmp(addr, target_ip) == 0) {
            freeifaddrs(ifaddr);
            return 1; // IP address matches
        }
    }

    freeifaddrs(ifaddr);
    return 0; // IP address not found
}

int check_os_version() {
    struct utsname uname_data;

    if (uname(&uname_data) == -1) {
        perror("uname");
        return 0;
    }

    if (strcmp(uname_data.sysname, "Linux") == 0 && strcmp(uname_data.release, "4.15.0-20-generic") == 0) {
        return 1; // Matches Ubuntu 18.04 kernel version
    }

    return 0;
}

int check_ram() {
    struct sysinfo info;

    if (sysinfo(&info) == -1) {
        perror("sysinfo");
        return 0;
    }

    long total_ram_mb = info.totalram / 1024 / 1024;
    if (total_ram_mb == 3072) {
        return 1;
    }

    return 0;
}

int main() {

    if (check_ip_address() != 1)
        goto fail;
    
    if (check_os_version() != 1) 
        goto fail;

    if (check_ram() != 1) 
        goto fail;

    puts("Mon environnement semble OK !");
    printf("Voici le flag: %s\n", get_rc4_msg());
    return 0;

fail:
    puts("Perdu :)");
    return -1;
}
