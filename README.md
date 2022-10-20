# C-RemoteShellContorole



	int Reverse = Bind({lhost}, {lport});
		switch(Reverse){
			case -1:
				puts("sock error");
				close(sock);
				return -1;
			case -2:
				puts("bind error");
				close(sock);
				return -2;
			case -3:
				puts("accept error");
				close(sock);
				return -3;
		};
