#ifndef __WEBSESSION_H__
#define __WEBSESSION_H__

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "WebByteBuffer.h"

void WebSocketThread(const std::string& host, const std::string& port);

enum webOpcode
{
    WEB_DEFAULT = 0x00
};

class WebSocket
{
    public:
        WebSocket(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator) : io_service_(io_service), socket_(io_service), closed(false)
        {
            boost::asio::connect(socket_, endpoint_iterator);
        }

        void run();

        void sendPacket(WebByteBuffer buffer)
        {
            boost::system::error_code error;
            socket_.write_some(boost::asio::buffer(buffer.contents(), buffer.size()), error);

            if (error)
                do_close();
        }

        void do_close()
        {
            socket_.close();
            closed = true;
        }

    protected:
        void handler_buffer(WebByteBuffer buffer);
        void ReadHandler();

    private:
        WebByteBuffer _writeBuffer;
        WebByteBuffer _readBuffer;

        boost::asio::io_service& io_service_;
        tcp::socket socket_;

        bool closed;
};

#endif