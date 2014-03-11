#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <netdb.h>
#include <pthread.h>

//�߳�ִ�к��������д
void *thr_fn(void *arg)
{
	int size,j;
    char recv_buf[1024];
	int *parg=(int *)arg;
	int new_fd=*parg;
	printf("new_fd=%d\n",new_fd);
	while((size=read(new_fd,recv_buf,1024))>0)
	{
		if(recv_buf[0]=='@')
			break;
		printf("Message from client(%d): %s\n",size,recv_buf);
		for(j=0;j<size;j++)
			recv_buf[j]=toupper(recv_buf[j]);
		write(new_fd,recv_buf,size);
	}
	close(new_fd);
	return 0;
}


int main(int argc,char *argv[])
{
    socklen_t clt_addr_len;
	int listen_fd;
	int com_fd;
	int ret;
	int i;
	static char recv_buf[1024];
	int len;
	int port;
	pthread_t tid;

	struct sockaddr_in clt_addr;
	struct sockaddr_in srv_addr;

	//������������ʱҪ�����˿���Ϣ���ö˿�Ϊ�����˿� 
	if(argc!=2)
	{
	    printf("Usage:%s port\n",argv[0]);
		return 1;
	}

	//�������Ķ˿� 
	port=atoi(argv[1]);

	//�����׽������ڷ������ļ��� 
	listen_fd=socket(PF_INET,SOCK_STREAM,0);
	if(listen_fd<0)
	{
	    perror("cannot create listening socket");
		return 1;
	}

	//�����ڷ��������׽�����Ϣ
	memset(&srv_addr,0,sizeof(srv_addr));
	srv_addr.sin_family=AF_INET;
	srv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	srv_addr.sin_port=htons(port);


	//�����������׽��ְ�
	ret=bind(listen_fd,(struct sockaddr *)&srv_addr,sizeof(srv_addr));
	if(ret==-1)
	{
	    perror("cannot bind server socket");
		close(listen_fd);
		return 1;
	}

	//����ָ���˿ڣ�����5���ͻ��� 
	ret=listen(listen_fd,5);
	if(ret==-1)
	{
	    perror("cannot listen the client connect request");
		close(listen_fd);
		return 1;
	}
	//��ÿ���������Ŀͻ��˴���һ���̣߳������������ͨ�� 
	//���ȵ���read������ȡ�ͻ��˷���������Ϣ 
	//����ת���ɴ�д���ͻؿͻ��� 
	//�����롰@��ʱ�������˳� 
	while(1)
	{
	    len=sizeof(clt_addr);
		com_fd=accept(listen_fd,(struct sockaddr *)&clt_addr,&len);
		if(com_fd<0)
		{
		    if(errno==EINTR)
			{
			    continue;
			}
			else
			{
			    perror("cannot accept client connect request");
				close(listen_fd);
				return 1;
			}
		}
		printf("com_fd=%d\n",com_fd);//��ӡ�������ӵĿͻ��˲������׽���
		if((pthread_create(&tid,NULL,thr_fn,&com_fd))==-1)
		{
		    perror("pthread_create error");
			close(listen_fd);
			close(com_fd);
			return 1;
		}
	}
	return 0;
}
