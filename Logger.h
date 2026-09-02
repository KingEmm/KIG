#pragma once
#include <fstream>
#include <iostream>
#include "Event.h"

class Logger
{
private:

    std::ofstream file;

public:

    Logger();

    void write(const Event& e);

    ~Logger();
};

