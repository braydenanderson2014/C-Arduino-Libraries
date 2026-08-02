#ifndef HOST_ARDUINO_SIM_PROCESS_H
#define HOST_ARDUINO_SIM_PROCESS_H

class Process {
public:
    Process() : _exitCode(0) {}

    void begin(const char*) {}
    void addParameter(const char*) {}
    void run() { _exitCode = 0; }

    int available() { return 0; }
    int read() { return -1; }
    int exitValue() const { return _exitCode; }

private:
    int _exitCode;
};

#endif
