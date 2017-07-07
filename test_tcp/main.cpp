//=====================================================================//
/*!	@file
	@brief	TCP テスト @n
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
	srand(static_cast<unsigned>(time(NULL)));

	bool server = false;

	// クライアントソケット作成
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		perror("TCP socket");
		return 1;
	}
	utils::format("TCP Socket Open (%d)\n") % sock;

	int fd = -1;
	if(server) {
		// struct sockaddr_in 作成
		struct sockaddr_in cl = { 0 };
		cl.sin_family = PF_INET;
		// 接続ポート３０００
		cl.sin_port = htons(3000);
		// 接続先のアドレスを指定（指定しない事も可能）
		cl.sin_addr.s_addr = htonl(INADDR_ANY);

		// バインド
		if(bind(sock, (struct sockaddr*)&cl, sizeof(struct sockaddr_in)) == -1) {
			perror("bind");
			close(sock);
			return 1;
		}
		utils::format("TCP Bind: '%s' (%d)\n") % get_ip_str(cl.sin_addr.s_addr) % htons(cl.sin_port);

		// リッスン
		if(listen(sock, 1) == -1) {
			perror("listen");
			close(sock);
			return 1;
		}
		std::cout << "listen: " << sock << std::endl;

		// クライアントの接続を待つ
		fd = accept(sock, NULL, NULL);
		close(sock);
		if(fd == -1) {
			perror("accept");
			return 1;
		}
		std::cout << "accept: " << fd << std::endl;

	} else {
		// struct sockaddr_in 作成
		struct sockaddr_in cl = { 0 };
		cl.sin_family = PF_INET;
		// 接続ポート３０００
		cl.sin_port = htons(3000);
		// 接続先のアドレスを指定（指定しない事も可能）
		cl.sin_addr.s_addr = inet_addr("192.168.3.2");

		// クライアントの接続を待つ
		int ret = connect(sock, (struct sockaddr *)&cl, sizeof(cl));
		if(ret < 0) {
			close(sock);
			perror("TCP connect");
			return 1;
		}
		utils::format("TCP Client connect (%d)\n") % sock;

		fd = sock;
	}

	uint32_t loop_cnt = 10;
	utils::format("TCP Client Send/Recv loop (%d): %d\n") % fd % loop_cnt;
	for(uint32_t loop = 0; loop < loop_cnt; ++loop) {
		// 送信
		char tmp[256];
		uint32_t len = (rand() & 15) + 8;
		for(uint32_t i = 0; i < len; ++i) { tmp[i] = 0x20 + (rand() & 63); }
		write(fd, tmp, len);
		tmp[len] = 0;
		utils::format("TCP %s Send (%d): '%s', %d\n") % (server ? "Server" : "Client") % fd % tmp % len;

		// 受信
		{
			usleep(20000);

			char tmp[256];
			int ret = read(fd, tmp, sizeof(tmp));
			if(ret < 0) {
				perror("read");
				close(sock);
				return 1;
			}
			if(ret > 0) {
				tmp[ret] = 0;
				utils::format("TCP %s Recv (%d): '%s', %d\n")
					% (server ? "Server" : "Client") % sock % tmp % ret;
			}
		}
	}

#if 0
	for(int i = 0; i < 100; ++i) {  // 1 秒待ってからクローズ
		usleep(10000);
	}
#endif

	// 接続のクローズ
	if(close(fd) == -1) {
		perror("close");
		return 1;
	}
	utils::format("TCP %s Close (%d)\n") % (server ? "Server" : "Client") % fd;

#if 0
	for(int i = 0; i < 600; ++i) {
		usleep(10000);
	}
#endif
}
