//
// Created by max on 17/05/19.
//

#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include "Fifo.h"
#include "helper.h"


Fifo::Fifo(const std::string &inp, Fifo::flag flag) {
    Printer::println(std::cout, "Fifo::Fifo(string)");
    std::stringstream strstr(inp);
    strstr >> name;
    strstr >> pid;
    fl = flag;
}

Fifo::Fifo(Fifo::flag m_flag) : name(), fl(m_flag) {
    Printer::println(std::cout, "Fifo::Fifo");
}


void Fifo::close() {
    Printer::println(std::cout, "Fifo::close");
    if (fd != -1)
        ::close(fd);
}

void Fifo::create() {
    Printer::println(std::cout, "Fifo::create");
    umask(0);
    srand((int) time(0));
    name = ("/tmp/");
    name += std::to_string((pid == -1 ? getpid() : pid));
    name += "_";
    name += to_string(fl);
    std::cout << name << std::endl;
    checker(mkfifo(name.data(), S_IRUSR | S_IWUSR | S_IWGRP), "Unable to create fifo");
}


void Fifo::open() {
    Printer::println(std::cout, "Fifo::open");
    Printer::println(std::cout, to_string(fl));
    checker(fd = ::open(name.data(), (fl == Fifo::flag::FLAG_LISTEN ? O_RDONLY : O_WRONLY)), "Fifo open failed");
    Printer::println(std::cout, "Fifo::opened");
}

Fifo::~Fifo() {
    Printer::println(std::cout, "Fifo::Fifo");
    try {
        close();
    } catch (std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
    }
    unlink(name.data());
}

std::string Fifo::to_string() {
    Printer::println(std::cout, "Fifo::to_string");
    return std::string(name).append(" ").append(std::to_string((pid == -1 ? getpid() : pid)));
}

Request Fifo::read() {
    Printer::println(std::cout, "Fifo::read");
    Request request{};
    int readres;
    checker(readres = ::read(fd, &request, sizeof(request)), "Fifo::read fail");
    if (readres <= 0) {
        throw std::runtime_error("Read from local fifo fail");
    }
    return request;
}


void Fifo::write(const std::string &str) {
    Printer::println(std::cout, "Fifo::write");
    checker(::write(fd, str.data(), str.size()), "Error while writing");
}

void Fifo::write(void *ptr, size_t size) {
    Printer::println(std::cout, "Fifo::write");
    checker(::write(fd, ptr, size), "Error while writing");
}

std::string Fifo::raw_read(size_t size) {
    Printer::println(std::cout, "Fifo::raw_read");

    char c;
    std::string str;
    size_t counter = 0;
    while (::read(fd, &c, 1) > 0 && ++counter <= size) {
        str += c;
    }
    return str;
}
