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

namespace {

}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	// サーバーソケット作成
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) {
		perror("socket");
		return 1;
	}

	// struct sockaddr_in 作成
	struct sockaddr_in sa = {0};
	sa.sin_family = AF_INET;
	// 接続ポート３０００
	sa.sin_port = htons(3000);
	// 接続先は、任意とする。
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	// バインド
	if(bind(sock, (struct sockaddr*) &sa, sizeof(struct sockaddr_in)) == -1) {
		perror("bind");
		close(sock);
		return 1;
	}

	// リッスン
	if(listen(sock, 128) == -1) {
		perror("listen");
		close(sock);
		return 1;
	}

	// クライアントの接続を待つ
	int fd = accept(sock, NULL, NULL);
	if(fd == -1) {
		perror("accept");
		close(sock);
		return 1;
	}

	std::string line;
	bool term = false;
	while(!term) {
		// 受信
		char buffer[4096];
		int recv_size = read(fd, buffer, sizeof(buffer) - 1);
		if(recv_size == -1) {
			perror("read");
			close(fd);
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
	if(close(fd) == -1) {
		perror("close");
		close(sock);
		return 1;
	}

	close(sock);
}
