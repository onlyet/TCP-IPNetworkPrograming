#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	assert(argc == 2);
	char *host = argv[1];
	/*获取目标主机地址信息*/
	struct hostent *hostinfo = gethostbyname(host);		//网络字节序
	assert(hostinfo);
	/*获取daytime服务信息*/
	struct servent *servinfo = getservbyname("daytime", "tcp");		//网络字节序
	assert(servinfo);
	printf("daytime port is %d\n", ntohs(servinfo->s_port));

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = servinfo->s_port;
	/*因为h_addr_list本身是网络字节序，不需要转换字节序*/
	address.sin_addr = *(struct in_addr*)(*hostinfo->h_addr_list);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int result = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
	if (result) {
		printf("errno is %d, error string: %s", errno, strerror(errno));
		exit(1);
	}
	assert(result != -1);

	char buffer[128];
	result = read(sockfd, buffer, sizeof(buffer));
	assert(result > 0);
	buffer[result] = '\0';
	printf("the day time is %s", buffer);
	close(sockfd);
	return 0;
}