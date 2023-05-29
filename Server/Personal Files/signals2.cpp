
#include "Server.hpp"

namespace irc {

    volatile std::sig_atomic_t Server::_signalStatus = 0;

    void Server::signalHandler(int signal) {
        if (signal)
            _signalStatus = signal;
    }

    Server::Server(const std::string& host, const int& port, const std::string& password)
        : _port(port), _status(0), _host(host)  {
        std::cout << "\033[33mParameter Constructor Called\033[0m" << std::endl;
        (void)password;
    }

    Server::~Server() {}

    void Server::config() {
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        std::signal(SIGQUIT, signalHandler);
        std::signal(SIGHUP, signalHandler);
        std::signal(SIGPIPE, SIG_IGN);
    }

    bool Server::isOnline() {
        if (_signalStatus && _status == 1)
            _status = _signalStatus;

        return _status == 1;
    }

    void Server::stop() {
        if (_status == 1) {
            _status = 0;
            // Perform any necessary cleanup
        }
    }

} // namespace irc


namespace irc {

    class Server {

        private:

            int                                 _port;
            int                                 _status;
            std::string                         _host;
            static volatile std::sig_atomic_t   _signalStatus;

            static void signalHandler(int signal);
            Server();

        public:
            Server( const std::string& host, const int& port, const std::string& password );
            ~Server();

            void config();
            bool isOnline();
            void stop();


    };
}