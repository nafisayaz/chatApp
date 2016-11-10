
#include<iostream>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<future>

#define reset "\033[00;00m"
#define green "\033[00;32m"

void sender(int port_snd)
{
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	sockaddr_in my_addr;
	my_addr. sin_family = AF_INET;
	my_addr.sin_port =port_snd;
//	my_addr.sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET, "192.168.1.11", &my_addr.sin_addr);
	while( connect(sockfd, reinterpret_cast<sockaddr*>(&my_addr), sizeof(my_addr)) == -1 )
	{
		std::cout<<"Please wait!!\n" <<strerror(errno)<<"\n";
		sleep(2);
	}
	std::cout<<"Now connected !!\n\n"<<std::endl;

	while(true)
	{
		std::string message;
		std::getline(std::cin, message);

//		char message[1024];
//		gets(message, 1024, stdin);

		if( send(sockfd,/* message*/ message.c_str(), message.size(), /*sizeof (message),*/  0) == -1 )
		{
			std::cout<<"send() failed => "<<strerror(errno)<< "\n";
		}
	}
}

void receiver( int port_recv)
{
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	sockaddr_in my_addr;
	my_addr. sin_family = AF_INET;
	my_addr.sin_port =port_recv;
//	my_addr.sin_addr.s_addr = INADDR_ANY;
//	inet_pton(AF_INET, "localhost", &my_addr.sin_addr);
	inet_pton(AF_INET, "192.168.1.10", &my_addr.sin_addr);

	if( bind(sockfd, reinterpret_cast<sockaddr*>(&my_addr), sizeof(my_addr) ) == -1 )
	{
		std::cout<<"bind() failed => "<<strerror(errno)<< "\n";
		return;
	}
	
	if( listen(sockfd, 5) == -1)
	{
		std::cout<<"listen() failed => "<<strerror(errno)<< "\n";
		return;
	}

	sockaddr_in their_addr {};
	socklen_t  their_size {};

	int chatfd = accept(sockfd, reinterpret_cast<sockaddr*>(&their_addr), &their_size);
	char message[1024];
	while(true)
	{
			
			if(recv(chatfd, message, sizeof message, 0) == -1 )
				std::cout<<"recv() failed !! => "<<strerror(errno)<< "\n";
			else
				std::cout<<green<<message<<reset<<std::endl;

			bzero(message,1024);
	}
}

int main(int argc, char** argv)
{
	int sender_port = std::stoi(argv[2]);
	int receiver_port = std::stoi(argv[1]);

	std::future<void> f1 = std::async(std::launch::async, sender, receiver_port);
	std::future<void> f2 = std::async(std::launch::async, receiver, sender_port);
	f1.get();
	f2.get();
}


