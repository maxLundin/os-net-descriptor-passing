//
// Created by max on 17/05/19.
//

#pragma once

#include <unistd.h>
#include <sstream>
#include "string"

class Pipe {
public:
    static const int PIPEFD_LEN = 2;

    Pipe();

    explicit Pipe(const int pipe_array[PIPEFD_LEN]);

    virtual void create() = 0;

    virtual void close() = 0;

    std::string to_string();

    std::string read();

    int getfd(int);

    void write(const std::string &str);

    virtual ~Pipe();


protected:
    int pipefd[PIPEFD_LEN]{};
};

class Pipe_real : public Pipe {
public:

    Pipe_real();

    explicit Pipe_real(const int pipe_array[PIPEFD_LEN]);

    void create() override;

    void close() override;

    ~Pipe_real() override;
};

class Pipe_img : public Pipe {
public:
    Pipe_img();

    explicit Pipe_img(const int pipe_array[PIPEFD_LEN]);

    void create() override;

    void close() override;

    ~Pipe_img() override;
};

Pipe *make_pipe(const std::string &str, int pos);


Pipe *make_pipe(int fd, int pos);


