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
        auto read_ans = socket.read();
        Printer::printrln(std::cout, "Got fifo: ", read_ans.second.get());
        auto read_ans1 = socket.read();
        Printer::printrln(std::cout, "Got fifo: ", read_ans1.second.get());

        Fifo request_in_fifo(std::string(read_ans.second.get(), read_ans.first), Fifo::flag::FLAG_SEND);
        Fifo request_out_fifo(std::string(read_ans1.second.get(), read_ans1.first), Fifo::flag::FLAG_LISTEN);
        request_in_fifo.open();
        request_out_fifo.open();


        Request request1{};
        request1.size = request.size();
        request1.pid = getpid();
        request_in_fifo.write(&request1, sizeof(request1));
        request_in_fifo.write(request);
        request_in_fifo.close();

        Request request2{};
        request2 = request_out_fifo.read();
        std::string answer = request_out_fifo.raw_read(request2.size);
        std::cout << answer << std::endl;
    } catch (std::runtime_error &e) {
        std::cerr << "Client failed" << std::endl;
        std::cerr << e.what() << std::endl;
    }

}