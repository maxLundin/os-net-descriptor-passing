//
// Created by valentinnodan on 17/05/19.
//

#pragma once

#include <unistd.h>
#include <sstream>
#include "string"

class Request {
public:
    pid_t pid;
    size_t size;

    std::string to_string() {
        return std::string(std::to_string(pid)).append(" ").append(std::to_string(size));
    }
};

class Fifo {
public:
    enum class flag {
        FLAG_LISTEN,
        FLAG_SEND,
    };
private:
    std::string to_string(Fifo::flag fl1) {
        switch (fl1) {
            case Fifo::flag::FLAG_LISTEN:
                return std::string("Listen");
            case flag::FLAG_SEND:
                return std::string("Send");
        }
        return "";
    }

public:

    explicit Fifo(Fifo::flag flag);

    explicit Fifo(const std::string &in, Fifo::flag flag);

    void create();

    void open();

    void close();

    std::string to_string();

    Request read();

    std::string raw_read(size_t size);

    void write(const std::string &str);

    void write(void *ptr, size_t size);

    ~Fifo();


public:
    char suffix[20]{};
protected:
    std::string name;
    int fd = -1;
    pid_t pid = -1;
    Fifo::flag fl;
};



