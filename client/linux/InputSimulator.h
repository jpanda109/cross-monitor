//
// Created by jason on 5/13/15.
//

#ifndef CSRC_INPUTSIMULATOR_H
#define CSRC_INPUTSIMULATOR_H

#include <X11/extensions/XTest.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>


class InputSimulator {

private:

    /**
     * private constructor to ensure singleton-property
     */
    InputSimulator();

public:

    /**
     * copy constructor + assignment deleted to retain singleton
     */
    InputSimulator(const InputSimulator &rhs) = delete;
    void operator=(const InputSimulator &rhs) = delete;

    /**
     * get current instance of InputSimulator
     */
    static InputSimulator *getInstance();

    ~InputSimulator();

    void processInput(const std::string &input);

    void moveMouse(const int &x, const int &y);

    void mouseButton(const unsigned int &button, bool down = true);

    void key(const unsigned int &keycode, bool down = true);

private:

    static InputSimulator *instance;

    Display *mDisplay;

};


#endif //CSRC_INPUTSIMULATOR_H
