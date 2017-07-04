//=====================================================================//
/*!	@file
	@brief	ソケット、データ受け取りサンプル @n
			Copyright 2017 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>
#include "../test_format/format.hpp"

// UDP 通信の場合有効
// #define UDP

#define SEND
// #define RECV

namespace {

	const char* get_ip_str(uint32_t ip)
	{
		const uint8_t* p = reinterpret_cast<const uint8_t*>(&ip);
		static char tmp[64];
		utils::sformat("%d.%d.%d.%d", tmp, sizeof(tmp))
			% static_cast<int>(p[0])
			% static_cast<int>(p[1])
			% static_cast<int>(p[2])
			% static_cast<int>(p[3]);
		return tmp;
	}

}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	// クライアントソケット作成
#ifdef UDP
	int sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock < 0) {
		perror("UDP socket");
		return 1;
	}
#else
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		perror("TCP socket");
		return 1;
	}
#endif

	// struct sockaddr_in 作成
	struct sockaddr_in cl = { 0 };
	cl.sin_family = PF_INET;
	// 接続ポート３０００
	cl.sin_port = htons(3000);
	// 接続先のアドレスを指定（指定しない事も可能）
	cl.sin_addr.s_addr = inet_addr("192.168.3.2");

#ifdef UDP
	// 送信
	utils::format("UDP Send: %s (%d)\n")
		% get_ip_str(cl.sin_addr.s_addr)
		% static_cast<int>(htons(cl.sin_port));
	int len = 8;
	sendto(sock, "AbcdefgH", len, 0, (struct sockaddr *)&cl, sizeof(cl));
	utils::format("UDP Client sendto: %d bytes\n") % len;

	// 受信
	utils::format("UDP Recv: %s (%d)\n")
		% get_ip_str(cl.sin_addr.s_addr)
		% static_cast<int>(htons(cl.sin_port));
	char tmp[64];
	socklen_t addrlen = sizeof(struct sockaddr);
	int ret = recvfrom(sock, tmp, sizeof(tmp), 0, (struct sockaddr *)&cl, &addrlen);
	if(ret > 0) {
		tmp[ret] = 0;
		utils::format("UDP Client recvfrom: %d, '%s'\n") % ret % tmp;
	}

#else
	// クライアントの接続を待つ
	int ret = connect(sock, (struct sockaddr *)&cl, sizeof(cl));
	if(ret < 0) {
		close(sock);
		perror("connect");
		return 1;
	}
	std::cout << "client connect ok" << std::endl;

	std::string line;
	bool term = false;
	while(!term) {
		// 送信
		char tmp[32];
		for(int i = 0; i < (int)sizeof(tmp); ++i) { tmp[i] = 0x20 + (rand() & 63); }
		write(sock, tmp, sizeof(tmp));

		// 受信
		char buffer[4096];
		int recv_size = read(sock, buffer, sizeof(buffer));
		if(recv_size == -1) {
			perror("read");
			close(sock);
			return 1;
		}
		buffer[recv_size] = 0;
		utils::format("%s\n") % buffer;
	}
#endif

	// 接続のクローズ
	if(close(sock) == -1) {
		perror("close");
		return 1;
	}
}
