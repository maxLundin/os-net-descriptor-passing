//
// Created by max on 07.05.19.
//

#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <memory>
#include <vector>
#include "helper.h"
#include "Socket.h"
#include "Fifo.h"
#include "Pipe.h"

using Symb = Printer::Symbols;

int main(int argc, char **argv) {
    std::cout << "Socket setup" << std::endl;
    std::string soc_name;
    if (argc == 1) {
        std::cout << "No socket address provided using default" << std::endl;
        soc_name = "/tmp/server.socket";
    } else {
        soc_name = argv[1];
    }
    Socket socket;

    socket.create(soc_name, Socket::FLAG_CLIENT);

    std::cout << "Socket setup completed" << std::endl;
    try {
        std::string str;
        std::string request;
        std::cin >> request;
        socket.connect();
        socket.write(request);
        auto read_ans = socket.read_fd();
        Printer::printrln(std::cout, "Got pipe: ", read_ans);
        auto read_ans1 = socket.read_fd();
        Printer::printrln(std::cout, "Got pipe: ", read_ans1);

        Pipe *pipe_write = make_pipe(read_ans, 1);
        Pipe *pipe_read = make_pipe(read_ans1, 0);

        pipe_write->write(request);

        auto ans = pipe_read->read();
        std::cout << ans << std::endl;
    } catch (std::runtime_error &e) {
        std::cerr << "Client failed" << std::endl;
        std::cerr << e.what() << std::endl;
    }

}