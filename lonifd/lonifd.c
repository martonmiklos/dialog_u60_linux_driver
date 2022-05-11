#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/if_packet.h>
#include <linux/udp.h>
#include <errno.h>
#include <ifaddrs.h>
#include <syslog.h>
#include <stdint.h>

static char londevice[IFNAMSIZ];
static int setup_serial_interface(char* devstr, int ldisc) {
    int fd;
    struct termios tio;
    bzero(&tio, sizeof(tio));
    tio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    tio.c_iflag = IGNPAR;
    tio.c_oflag = 0;
    tio.c_lflag = 0;
    tio.c_cc[VMIN] = 0;
    tio.c_cc[VTIME] = 1;

    fd = open(devstr, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("open");
        syslog(LOG_ERR, "Error opening device: %s\n", strerror(errno));
    }

    tcflush(fd, TCIFLUSH);

    if(tcsetattr(fd, TCSANOW, &tio) < 0) {
        perror("tcsetattr");
    }
    if (ioctl(fd, TIOCSETD, &ldisc) < 0) {
        perror("ioctl failed, make sure that the u50 module is loaded!");
        syslog(LOG_ERR, "Error setting device attributes: %s\n", strerror(errno));
    }
    return fd;
}

static void read_neuron_id(uint8_t *buf) {
	struct ifreq ifr = {0};
	int sockfd = socket(AF_PACKET, SOCK_RAW, htons(0x8950));
	//Read Memory Local Message
	uint8_t a[] = { 0x22, 0x13, 0x70, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x01, 0x00, 0x00, 0x06 };

	memcpy(ifr.ifr_name, londevice, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0)
        perror("ioctl");

    struct sockaddr_ll addr_ll = { 0 };
    addr_ll.sll_family = AF_PACKET;
    addr_ll.sll_ifindex = ifr.ifr_ifindex;

	if(bind(sockfd, (struct sockaddr*) &addr_ll, sizeof(struct sockaddr_ll)) < 0) 
		perror("bind");

	int readlen = 0;
	fd_set fds;
	struct timeval tv;
	int ret;
	
	do {
		int b;
		if ((b = write(sockfd, a, sizeof(a))) < 0) perror("write");
		uint8_t readbuf[32];
		memset(readbuf, 0, 32);

		FD_ZERO(&fds);
		FD_SET(sockfd, &fds);
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		if (select(sockfd+1, &fds, NULL, NULL, &tv))
			readlen = read(sockfd, readbuf, 32);
		if (readlen == 24) { //read memory response is 24 bytes, this could be parsed
                             //with a lot more headers brought in
			int i;
			for (i=0; i < 6; i++) 
				buf[i] = readbuf[i+17];
		}
	} while (readlen != 24); //expected response length
	close(sockfd);
}

#define NID_INDEX_FILE "/usr/lib/izot/nid_index"
int find_nid_index(uint8_t *nid, char* name) {
	FILE *f = fopen(NID_INDEX_FILE, "a+");
	if (f == NULL) return -1;
	int found = 0, max_index = 0;
	char indexstr[8], nidstr[18], searchnid[18];
	sprintf(searchnid, "%02x:%02x:%02x:%02x:%02x:%02x", 
		nid[0], nid[1], nid[2], nid[3], nid[4], nid[5]);
	while (fscanf(f, "%3s %17s", indexstr, nidstr) == 2) {
		if (!strcmp(searchnid, nidstr)) {
			found = 1;
			break;
		} else {
			max_index = strtol(indexstr, NULL, 10)+1;
		}
	}
	if (ferror(f)) {
		fclose(f);
		return -1;
	}
	if (!found) {
		fprintf(f, "%i %s\n", max_index, searchnid);
		sprintf(name, "lon%i", max_index);
	} else {
		sprintf(name, "lon%s", indexstr);
	}
	fclose(f);
	return 0;
}

static void assign_default_address(void) {
	struct ifreq ifr = {0};
	char addr[16];
	uint8_t nidbuf[6];
	int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (s < 0) {
		perror("socket");
		exit(1);
	}

	char ifbuf[IFNAMSIZ];
	memset(ifbuf, 0, IFNAMSIZ);
	memcpy(ifr.ifr_name, londevice, IFNAMSIZ);
	ioctl(s, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
	//Set to running so neuron id can be read
	if (ioctl(s, SIOCSIFFLAGS, &ifr) < 0) perror("run");
	
	read_neuron_id(nidbuf);
	int tries = 5;
	while (find_nid_index(nidbuf, ifbuf) && tries-- > 0) sleep(5);
	//neuron id read, set down so name can change and ip can be set
	ifr.ifr_flags &= !(IFF_UP | IFF_RUNNING);
	if (ioctl(s, SIOCSIFFLAGS, &ifr) < 0) { perror("set flags"); exit(4); }

	if (strlen(ifbuf)) {
		//found a interface name this should be using, change it to use it
		memcpy(ifr.ifr_newname, ifbuf, IFNAMSIZ);
		if (ioctl(s, SIOCSIFNAME, &ifr) < 0) { perror("set name"); exit(3); }
		memcpy(londevice, ifbuf, IFNAMSIZ);
	}

	memcpy(ifr.ifr_name, londevice, IFNAMSIZ);
	//set a default ip and mask
	sprintf(addr, "192.168.1%s.1", londevice + strlen("lon"));
	syslog(LOG_INFO, "Assigning %s to %s\n", addr, londevice);
	struct sockaddr_in *in_addr = (struct sockaddr_in*)&ifr.ifr_addr;
	ifr.ifr_addr.sa_family = AF_INET;
	inet_pton(AF_INET, addr, &in_addr->sin_addr);
	if (ioctl(s, SIOCSIFADDR, &ifr) < 0) { perror("if set"); exit(2);}

	inet_pton(AF_INET, "255.255.255.0", ifr.ifr_addr.sa_data + 2);
	if (ioctl(s, SIOCSIFNETMASK, &ifr) < 0) { perror("mask set"); exit(2); }

	//done setting everything, set running again
	ioctl(s, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
	if (ioctl(s, SIOCSIFFLAGS, &ifr) < 0) { perror("run"); exit(2); }
}

static int setup_lon_iface(char* devstr) {
    struct ifreq ifr = {0};
    struct ifaddrs *ifaddr, *ifa;
	int found = 0;
    int lonsd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));
	//This iterates through each lonX device and queries what tty is being used.
	//The SIOCDEVPRIVATE is defined in the u50 module's ioctl handler.
    getifaddrs(&ifaddr);
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		char socketdev[64];
		if (strncmp(ifa->ifa_name, "lon", 3)) continue;

    	memcpy(ifr.ifr_name, ifa->ifa_name, IFNAMSIZ);
		if (ioctl(lonsd, SIOCDEVPRIVATE, &ifr) < 0) perror("ioctl");
		sprintf(socketdev, "/dev/%s", (char*)&ifr.ifr_data);
		if (!memcmp(devstr, socketdev, strlen(devstr))) {
			found = 1;
			memcpy(londevice, ifa->ifa_name, IFNAMSIZ);
			break;
		}
    }
    freeifaddrs(ifaddr);
	if (!found) return -1;
    memcpy(ifr.ifr_name, londevice, strlen(londevice));

	assign_default_address();
    return lonsd;
}

int main(int argc, char* argv[]) {
	int c;
    int ldisc = 28; //Defined in kernel module
    char *devstr = "/dev/ttyACM0";
    openlog("lonifd", 0, 0);
    syslog(LOG_MAKEPRI(LOG_USER, LOG_INFO), "lonifd 1.0\n");
    while ((c = getopt(argc, argv, "d:l:")) != -1) {
        switch (c) {
            case 'd':
                devstr = optarg;
                break;
            case 'l':
                if (isdigit(optarg[0]))
                    ldisc = atoi(optarg);
                break;
        }
    }

    daemon(0, 1);
    
    setup_serial_interface(devstr, ldisc);
	if (setup_lon_iface(devstr) < 0) return -1;
    for (;;) sleep(1);
    return 0;
}
