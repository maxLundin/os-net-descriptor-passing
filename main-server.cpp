//
// Created by max on 07.05.19.
//

#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <memory>
#include <vector>
#include <map>
#include <csignal>
#include <algorithm>
#include "helper.h"
#include "Socket.h"
#include "Fifo.h"

using Symb = Printer::Symbols;


bool flag = false;


void f_signal(UNUSED int a) {
    flag = true;
    std::cout << "Exiting..." << std::endl;
    throw std::runtime_error("Success exit");
}

int main(int argc, char **argv) {
    signal(SIGINT, f_signal);
    try {
        std::cout << "Socket setup" << std::endl;
        std::string soc_name;
        if (argc == 1) {
            std::cout << "No socket address provided using default" << std::endl;
            soc_name = "/tmp/server.socket";
        } else {
            soc_name = argv[1];
        }
        Socket socket;
        socket.create(soc_name, Socket::FLAG_SERVER);
        socket.bind();
        socket.listen();

        std::cout << "Socket setup completed" << std::endl;

        while (!flag) {
            socket.accept();
            Fifo my_input_fifo(Fifo::flag::FLAG_LISTEN);
            my_input_fifo.create();


            Fifo my_output_fifo(Fifo::flag::FLAG_SEND);
            my_output_fifo.create();

            socket.write(my_input_fifo.to_string());
            socket.write(my_output_fifo.to_string());
            Printer::printrln(std::cout, "Fifo sent: ", my_input_fifo.to_string());

            my_input_fifo.open();
            my_output_fifo.open();


            Request request = my_input_fifo.read();
            std::string res = my_input_fifo.raw_read(request.size);

            Request request1{};
            std::string ans("Hello, ");
            ans += res;

            request1.size = ans.size();
            request1.pid = getpid();

            my_output_fifo.write(&request1, sizeof(request1));
            my_output_fifo.write(std::string("Hello, ").append(res));
            my_output_fifo.close();
        }

    } catch (std::runtime_error &e) {
        if (strcmp(e.what(), "Success exit") != 0) {
            exit(EXIT_SUCCESS);
        }
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}