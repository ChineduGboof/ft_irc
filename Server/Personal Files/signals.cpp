#include <csignal>
#include <vector>

namespace {
    volatile std::sig_atomic_t gSignalStatus;

    void signalHandler(int signal) {
        if (signal != 0)
            gSignalStatus = signal;
    }
}

class Server {
private:
    enum ServerStatus { OFFLINE, ONLINE };
    int m_serverStatus;
    int m_socket;

public:
    void sigInit() {
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        std::signal(SIGQUIT, signalHandler);
        std::signal(SIGHUP, signalHandler);
        std::signal(SIGPIPE, SIG_IGN);
    }

    bool isOnline() {
        if (gSignalStatus && m_serverStatus == ONLINE)
            return true;

        if (gSignalStatus == SIGTERM || gSignalStatus == SIGINT ||
            gSignalStatus == SIGQUIT || gSignalStatus == SIGHUP) {
            stop();
        }

        return (m_serverStatus != 0);
    }

    void stop() {
        if (m_serverStatus) {
            m_serverStatus = OFFLINE;
            close(m_socket);
        }
    }
};
