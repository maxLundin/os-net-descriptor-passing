//
// Created by max on 09.06.19.
//


#include "Pipe.h"
#include "helper.h"

Pipe::Pipe() {
    Printer::println(std::cout, "Pipe::Pipe");
    for (int &i : pipefd) {
        i = -1;
    }
}

void Pipe_real::close() {
    Printer::println(std::cout, "Pipe::close");
    std::string error_msg("Pipe::close Unable to close pipefd number: ");
    for (int fd : pipefd) {
        ::close(fd);
    }
}

void Pipe_real::create() {
    Printer::println(std::cout, "Pipe::create");
    checker(pipe(pipefd), "Unable to create pipe");
}


Pipe_real::~Pipe_real() {
    Printer::println(std::cout, "Pipe::~Pipe");
    try {
        close();
    } catch (std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
    }
}

Pipe::~Pipe() = default;

std::string Pipe::to_string() {
    Printer::println(std::cout, "Pipe::to_string");
    std::string str;
    for (int &i  : pipefd) {
        str.append(std::to_string(i)).append(" ");
    }
    return str;
}

std::string Pipe::read() {
    Printer::println(std::cout, "Pipe::read");
    ::close(pipefd[1]);
    char buf;
    std::string str;
    while (true) {
        int readres;
        checker(readres = ::read(pipefd[0], &buf, 1), "Pipe::read fail");
        if (readres <= 0) {
            break;
        }
        str += buf;
    }
    ::close(pipefd[0]);
    if (!str.empty() && str.back() != '\n')
        str += '\n';
    return str;
}

void Pipe::write(const std::string &str) {
    ::close(pipefd[0]);
    Printer::println(std::cout, "Pipe::write");
    checker(::write(pipefd[1], str.data(), str.size()), "Pipe::write fail");
    ::close(pipefd[1]);
}

Pipe::Pipe(const int *pipe_array) {
    Printer::println(std::cout, "Pipe::Pipe(int[])");
    for (int i = 0; i < PIPEFD_LEN; ++i) {
        pipefd[i] = pipe_array[i];
    }
}

int Pipe::getfd(int id) {
    return pipefd[id];
}

void Pipe_img::create() {
    throw std::runtime_error("Illegal operation Pipe_img::create");
}

void Pipe_img::close() {

}

Pipe_img::Pipe_img() = default;

Pipe_img::Pipe_img(const int *pipe_array) : Pipe(pipe_array) {

}

Pipe_real::Pipe_real() = default;

Pipe_real::Pipe_real(const int *pipe_array) : Pipe(pipe_array) {

}


Pipe_img::~Pipe_img() = default;

Pipe *make_pipe(const std::string &str, int pos) {
    Printer::println(std::cout, "make_pipe");
    int count = 0;
    size_t iter = 0;
    while (count < pos && iter < str.size()) {
        if (str[iter++] == '|') {
            ++count;
        }
    }
    std::string given = str.substr(iter);
    std::stringstream stringstream(given);
    int fd[Pipe::PIPEFD_LEN];
    for (int &i : fd) {
        stringstream >> i;
    }
    return new Pipe_img(fd);
}

Pipe *make_pipe(int fd, int pos) {
    Printer::println(std::cout, "make_pipe");
    int fd1[Pipe::PIPEFD_LEN];
    fd1[pos] = fd;
    if (pos == 1) {
        fd1[0] = -1;
    } else {
        fd1[1] = -1;
    }
    return new Pipe_real(fd1);
}

