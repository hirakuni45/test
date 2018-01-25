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

	enum class type {
		idle,
		server_send_first,
		server_recv_first,
		client_send_first,
		client_recv_first,
	};

	// テスト・タイプの設定
//	type		type_ = type::idle;
//	type		type_ = type::server_send_first;
	type		type_ = type::server_recv_first;

	// 転送回数の設定
	uint32_t	test_loop_ = 10;

	// クローズを行う遅延時間（１０ｍｓ単位）
	bool		close_delay_ = 500;


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
	if(type_ == type::server_send_first || type_ == type::server_recv_first) {
		server = true;
	}

	bool first_send = false;
	if(type_ == type::server_send_first || type_ == type::client_send_first) {
		first_send = true;
	}

	switch(type_) {
	case type::idle:
		utils::format("TCP Test type idle (exit)\n");
		return 0;
		break;
	case type::server_send_first:
		utils::format("TCP Test type 'Server', 'Send First'\n");
		break;
	case type::server_recv_first:
		utils::format("TCP Test type 'Server', 'Recv First'\n");
		break;
	case type::client_send_first:
		utils::format("TCP Test type 'Client', 'Send First'\n");
		break;
	case type::client_recv_first:
		utils::format("TCP Test type 'Client', 'Recv First'\n");
		break;
	default:
		break;
	}

	// クライアントソケット作成
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		perror("TCP Test socket");
		return 1;
	}
	utils::format("TCP Test socket open (%s): (%d)\n")
		% (server ? "Server" : "client") % sock;

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
		if(bind(sock, (struct sockaddr*)&cl, sizeof(struct sockaddr_in)) < 0) {
			perror("TCP Test bind");
			close(sock);
			return 1;
		}
		utils::format("TCP Test bind (Server): '%s' %d (%d)\n")
			% get_ip_str(cl.sin_addr.s_addr) % htons(cl.sin_port) % sock;

		// リッスン
		if(listen(sock, 1) < 0) {
			perror("TCP Test listen");
			close(sock);
			return 1;
		}
		utils::format("TCP Test listen (Server): (%d)\n") % sock;

		// クライアントの接続を待つ
		{
			struct sockaddr_in cl = { 0 };
			socklen_t len = sizeof(struct sockaddr);
			fd = accept(sock, (struct sockaddr*)&cl, &len);
			close(sock);
			if(fd == -1) {
				perror("accept");
				return 1;
			}
			utils::format("TCP Test accept (Server): '%s', (%d)\n")
				% get_ip_str(cl.sin_addr.s_addr) % fd;
		}

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
			perror("TCP Test connect");
			return 1;
		}
		utils::format("TCP Client connect (%d)\n") % sock;

		fd = sock;
	}

	if(first_send) {
		utils::format("TCP Test Send/Recv %d loop (%s): (%d)\n")
			% test_loop_ % (server ? "Server" : "Client") % fd;

		for(uint32_t loop = 0; loop < test_loop_; ++loop) {
			// 送信
			char tmp[256];
			uint32_t len = (rand() & 15) + 8;
			for(uint32_t i = 0; i < len; ++i) { tmp[i] = 0x20 + (rand() & 63); }
			write(fd, tmp, len);
			tmp[len] = 0;
			utils::format("TCP Test Send (%s): '%s', %d bytes (%d)\n")
				% (server ? "Server" : "Client") % tmp % len % fd;

			// 受信
			uint32_t all = 0;
			while(all < (len * 5 / 7)) {
///				usleep(10000);  // 10ms

				char tmp[256];
				int ret = read(fd, tmp, sizeof(tmp));
				if(ret < 0) {
					perror("read");
					close(sock);
					return 1;
				}
				if(ret > 0) {
					tmp[ret] = 0;
				utils::format("TCP Test Recv (%s): '%s', %d bytes (%d)\n")
					% (server ? "Server" : "Client") % tmp % ret % fd;
					all += ret;
				}
			}
		}
	} else {
		utils::format("TCP Test Recv/Send %d loop (%s): (%d)\n")
			% test_loop_ % (server ? "Server" : "Client") % fd;

		for(uint32_t loop = 0; loop < test_loop_; ++loop) {
			// 受信
///			usleep(10000);  // 10ms

			char tmp[256];
			int len = read(fd, tmp, sizeof(tmp));
			if(len < 0) {
				perror("read");
				close(sock);
				return 1;
			}
			if(len > 0) {
				tmp[len] = 0;
				utils::format("TCP Test (%d)Recv (%s): '%s'  %d bytes desc(%d)\n")
					% loop % (server ? "Server" : "Client") % tmp % len % fd;
			}

			write(fd, tmp, len);
			tmp[len] = 0;
			utils::format("TCP Test (%d)Send (%s): '%s'  %d bytes desc(%d)\n")
				% loop % (server ? "Server" : "Client") % tmp % len % fd;
		}
	}

	if(close_delay_) {
		for(uint32_t i = 0; i < close_delay_; ++i) {
			usleep(10000);  // 10ms
		}
	}

	// 接続のクローズ
	if(close(fd) == -1) {
		perror("close");
		return 1;
	}
	utils::format("TCP Test close (%s): (%d)\n") % (server ? "Server" : "Client") % fd;
}
