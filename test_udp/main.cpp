//=====================================================================//
/*!	@file
	@brief	ソケット、UDP テスト・サンプル @n
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
	srand(static_cast<unsigned>(time(NULL)));

	// ソケット作成
	int sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock < 0) {
		perror("UDP socket");
		return 1;
	}

	// struct sockaddr_in 作成
	struct sockaddr_in cl = { 0 };
	cl.sin_family = PF_INET;
	// 接続ポート３０００
	cl.sin_port = htons(3000);
	// 接続先のアドレスを指定（指定しない事も可能）
	cl.sin_addr.s_addr = inet_addr("192.168.3.2");
	utils::format("UDP Open(%d): '%s' (%d)\n")
		% sock % get_ip_str(cl.sin_addr.s_addr) % htons(cl.sin_port);

	// ＵＤＰテスト側と同じにする
	int loop_num = 4;

	for(int loop = 0; loop < loop_num; ++loop) {
		// 送信
		uint32_t send_len = (rand() & 15) + 4;
		char send_tmp[32];
		for(uint32_t i = 0; i < send_len; ++i) {
			send_tmp[i] = 32 + (rand() & 63);
		}
		sendto(sock, send_tmp, send_len, 0, (struct sockaddr *)&cl, sizeof(cl));
		utils::format("UDP Client sendto: '%s', %d bytes\n") % send_tmp % send_len;

		// 受信
		char recv_tmp[32];
		socklen_t addrlen = sizeof(struct sockaddr);
		int recv_len = recvfrom(sock, recv_tmp, sizeof(recv_tmp), 0, (struct sockaddr *)&cl, &addrlen);
		if(recv_len >= 0) {
			recv_tmp[recv_len] = 0;
			utils::format("UDP Client recvfrom: '%s', %d bytes\n") % recv_tmp % recv_len;
		}
	}

	// 接続のクローズ
	utils::format("UDP Close(%d)\n") % sock;
	if(close(sock) == -1) {
		perror("close");
		return 1;
	}
}
