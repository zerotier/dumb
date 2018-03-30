/* DUMB: Dumb User Mode Bridge */

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pcap/pcap.h>

static struct {
	pthread_t threads[2];
	pcap_t *interfaces[2];
} ifs;

static void ph(unsigned char *ifp,const struct pcap_pkthdr *hdr,const unsigned char *data)
{
	unsigned int i = (unsigned int)((uintptr_t)ifp);
	if ((hdr)&&(data))
		pcap_inject(ifs.interfaces[i ^ 1],data,hdr->len);
}

static void *thr(void *ifp)
{
	unsigned int i = (unsigned int)((uintptr_t)ifp);
	pcap_setdirection(ifs.interfaces[i],PCAP_D_IN);
	pcap_loop(ifs.interfaces[i],0,ph,(unsigned char *)ifp);
	return NULL;
}

int main(int argc,char **argv)
{
	char errbuf[1024];

	if (argc != 3) {
		fprintf(stderr,"Usage: %s <interface0> <interface1>\n",argv[0]);
		return 1;
	}

	for(int i=0;i<2;++i) {
		ifs.interfaces[i] = pcap_open_live(argv[i + 1],131072,1,1,errbuf);
		if (!ifs.interfaces[i]) {
			fprintf(stderr,"%s: FATAL: unable to open device %s\n",argv[0],argv[1]);
			return 1;
		}
		if (pthread_create(&(ifs.threads[i]),NULL,thr,(void *)((uintptr_t)i))) {
			fprintf(stderr,"%s: FATAL: pthread_create() failed\n",argv[0]);
			return 1;
		}
	}

	for(int i=0;i<2;++i)
		pthread_join(ifs.threads[i],NULL);

	return 0;
}
