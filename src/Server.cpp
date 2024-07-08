/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:35 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/02 11:52:58 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>

// PUBLIC FUNCTIONS

Server::Server(const std::string &address, int port) : _address(address),
  _port(port), _server_fd(-1), _epoll_fd(-1)
{
  _initServer();
}

Server::~Server()
{
  /* verificar se o os fds sao diferentes de -1 para dar close */
  if (_server_fd != -1)
  {
    close(_server_fd);
  }

  if (_epoll_fd != -1)
  {
    close(_epoll_fd);
  }
}

void Server::_initServer()
{

  /*   criando o socket/fd e verificando se ele continua -1 (se continuar, erro) */
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_server_fd == -1) {
		throw std::runtime_error("Failed to create socket");
	}

  /* configurando o socket/fd do servidor para poder ser reutilizado assim que
   * for fechado, eh bom para casos em que voce precisa reiniciar o servidor
   * sem esperar em que o sistema operacional liberar a porta automaticamente
   * (normalmente tem um tempo para isso ocorrer)
   **/
  int opt = 1; // argumento para ser usado com o novo valor para setar.
  if (setsockopt(_server_fd, SOL_SOCKET, &opt, sizeof(opt)) == -1)
  {
    throw std::runtime_error("Failed to set socket options");
  }

  /* estrutura para usar com o protocolo IPv4 e contem campos para especificar o endereco de IP
   * numero da porta dentro outras coisas.
  **/
  sockaddr_in server_addr;

  /* inicializando todo os bytes para do server_addr para 0 para assegurar que todos os campos vao inicar
  * com o mesmo valor
  **/
  memset(&server_addr, 0, sizeof(server_addr));

  /* sin_famlily = Address Family Internet - sever para especificar que esta usando IPV4 */
  server_addr.sin_family = AF_INET;
  
  /* a funcao inet_addr recebe uma representacao de uma string(char *) com um endereco IPv4 e retorna
   * a representacao binaria do ip para operacoes de rede.
   * e essa linha so esta setando o endereco de ip para o ip passado via string para a inet_addr
   **/
  server_addr.sin_addr.s_addr = inet_addr(_address.c_str());

  /* variavel da struct server_addr em que voce indica a porta que sera utilizada
   * a funcao htons ("Host TO Network Short") converte o numero da porta para bytes de ordem de host
   * ela garante que o numero da porta estara corretamente interpretado independente da arquitetura
   * do sistema.
   * */
  server_addr.sin_port = htons(_port);

  /* a funcao bind associa o socket referido pelo fd '_server_fd' com o endereco de IP local
   * e com a porta especificada na estrutura 'server_addr' e informa ao sistema que  a aplicacao
   * quer receber dados no ip e na porta direcionada.
   **/
  if (bind(_server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1)
  {
    throw std::runtime_error("Failed to bind socket");
  }

  /* apos o binding a funcao listen server parr colocar o socket no estado de espera.
   * Significa que o socket esta pronto para aceitar as requisicoes de conexao de
   * entrada dos clientes.
   **/
  if (listen(_server_fd, MAX_CONNECTIONS) == -1)
  {
    throw std::runtime_error("Failed to listen socket");
  }

}

}
// PRIVATE FUNCTIONS

void	Server::_createServerSocket(void) {

void	Server::_bindServer(void) {
	if (bind(this->_serverSocket, (struct sockaddr *)&this->_address,
			sizeof(this->_address)) == -1) {
	throw std::runtime_error("Error binding the server");
	}
}

void	Server::_listen(void) {
	if (listen(this->_serverSocket, MAX_CONNECTIONS) == -1) {
		throw std::runtime_error("Error listening the server");
	}
}

void	Server::_acceptClient(void) {
	Client *client = new Client();

	client->connectToServer(this->_serverSocket);
	this->_clients.push_back(client);
}

void	Server::_polling(void) {
  struct epoll_event ev, events[MAX_EVENTS];
	int conn_sock, nfds, epollfd;

	epollfd = epoll_create1(0);
	if (epollfd == -1) {
		throw std::runtime_error("Error polling");
	}
	ev.events = EPOLLIN;
	ev.data.fd = this->_serverSocket;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, this->_serverSocket, &ev) == -1) {
		throw std::runtime_error("Error polling the server socket");
	}
	for (;;) {
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			throw std::runtime_error("Error polling events");
		}
		for (int n = 0; n < nfds; ++n) {
			if (events[n].data.fd == this->_serverSocket) {
				this->_acceptClient();
				conn_sock = this->_clients.back()->getClientSocket();
			}
			ev.events = EPOLLIN | EPOLLET;
			ev.data.fd = conn_sock;
			if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
				throw std::runtime_error("Error adding client to server poll");
			}
			else {
				//RESOLVER OS EVENTOS DO NOSSO SERVER
				std::cout << "teste" << std::endl;
				//do_use_fd(events[n].data.fd);
			}
		}
	}
}
