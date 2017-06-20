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

namespace {

}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	// struct sockaddr_in 作成
	struct sockaddr_in cl = { 0 };
	cl.sin_family = PF_INET;
	// 接続ポート３０００
	cl.sin_port = htons(3000);
	// 接続先は、サーバーのアドレスを指定
	cl.sin_addr.s_addr = inet_addr("192.168.3.2");

	// クライアントソケット作成
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		perror("socket");
		return 1;
	}

	utils::format("socket: %08X (%d)\n")
		% static_cast<uint32_t>(cl.sin_addr.s_addr)
		% static_cast<int>(htons(cl.sin_port));

	// クライアントの接続を待つ
	int ret = connect(sock, (struct sockaddr *)&cl, sizeof(cl));
	if(ret < 0) {
		close(sock);
		perror("connect");
		return 1;
	}
	std::cout << "connect: " << ret << std::endl;

	std::string line;
	bool term = false;
	while(!term) {
		// 受信
		char buffer[4096];
		int recv_size = read(sock, buffer, sizeof(buffer));
		if(recv_size == -1) {
			perror("read");
			close(sock);
			return 1;
		}

		// 内容を解析して表示
		for(int i = 0; i < recv_size; ++i) {
			char ch = buffer[i];
			if(ch == '\r') continue;
			if(ch == '\n') {
				std::cout << line << std::endl;
				if(line == "end") term = true;
				line.clear();
			} else {
				line += ch;
			}
		}
	}

	// 接続のクローズ
	if(close(sock) == -1) {
		perror("close");
		return 1;
	}
}
