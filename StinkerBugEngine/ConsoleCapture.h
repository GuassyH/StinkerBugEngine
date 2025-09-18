#include <iostream>
#include <sstream>
#include <streambuf>
#include <vector>
#include <string>


// NOT MY CODE, FOUND ONLINE WITH ANONYMOUS POSTER 

class ConsoleCapture : public std::streambuf {
public:
    ConsoleCapture() {
        originalBuf = std::cout.rdbuf(this); // Redirect cout
    }

    ~ConsoleCapture() {
        std::cout.rdbuf(originalBuf); // Restore cout on destruction
    }

    const std::vector<std::string>& GetLines() const {
        return lines;
    }

    void Clear() {
        lines.clear();
    }

protected:
    virtual int overflow(int ch) override {
        if (ch == '\n') {
            lines.push_back(buffer);
            buffer.clear();
        }
        else {
            buffer += static_cast<char>(ch);
        }
        return ch;
    }

private:
    std::streambuf* originalBuf;
    std::vector<std::string> lines;
    std::string buffer;
};