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
#include "Pipe.h"

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
            Pipe_real my_input_pipe{};
            my_input_pipe.create();


            Pipe_real my_output_pipe{};
            my_output_pipe.create();

            socket.write_fd(my_input_pipe.getfd(1));
            socket.write_fd(my_output_pipe.getfd(0));
            Printer::printrln(std::cout, "Pipe sent: ", my_input_pipe.to_string());
            Printer::printrln(std::cout, "Pipe sent: ", my_output_pipe.to_string());

            auto request = my_input_pipe.read();

            std::string ans("Hello, ");
            ans += request;

            my_output_pipe.write(ans);
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