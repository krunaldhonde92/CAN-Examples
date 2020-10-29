#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char **argv)
{
	int s;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}
	strcpy(ifr.ifr_name, "vcan0" );
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

	int loopback = 1; /* 0 = disabled, 1 = enabled (default) */

    setsockopt(s, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));
    
	setsockopt(s, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));

	int i = 0;

	frame.can_id = 0x555;
	frame.can_dlc = 1;
	frame.data[0]=i;

    srand(time(0)); 
  
	while(1)
	{
		i++;
		if(i == 251)
			break;
		
		frame.data[0]= i;

		if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
			perror("Write");
			return 1;
		}
		else
		{
			printf("Sent 12345\n");
		}
		usleep(rand()%1000);
	}

	if (close(s) < 0)
	{
		perror("Close");
		return 1;
	}

	return 0;
}
