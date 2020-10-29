#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h> 
#include <netdb.h>
#include <linux/can.h>
#include <linux/can/raw.h>

/*void func()
{
	for(int i = 0 ; i< 1000 ; i++)
		for(int j = 0 ; j< 1000 ; j++)
			for(int k = 0 ; k< 1000 ; k++);
			
	
}*/

int main(int argc, char **argv)
{
	int s, i; 
	int nbytes;
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
	
	int flags = fcntl(s, F_GETFL, 0);
	fcntl(s, F_SETFL, flags | O_NONBLOCK);
    srand(time(0)); 

	static int j = 0;
	while(1)
	{
		
/*		code for blocking
		nbytes = read(s, &frame, sizeof(struct can_frame));

		if (nbytes < 0) {
			perror("Read");
			return 1;
		}
		j++;
		printf("total packet : %d\n",j);
*/

		nbytes = read(s, &frame, sizeof(struct can_frame));

		if (nbytes > 0) {
			j++;
			printf("total packet : %d\n",j);
			printf("data1 : %d \n",frame.data[0]);
//			printf("Received : 0x%03X [%d] ",frame.can_id, frame.can_dlc);
		
			/*for (i = 0; i < frame.can_dlc; i++)
				printf("%c",frame.data[i]);
			printf("\r\n");*/

			//func();
		}
		else
		{
			printf("*\n");
		}
		usleep(rand()%10000);

	}

	if (close(s) < 0)
	{
		perror("Close");
		return 1;
	}
	return 0;
}
