#ifndef __WEBSESSION_H__
#define __WEBSESSION_H__

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "WebByteBuffer.h"

void WebSessionThread(const std::string& host, const std::string& port);

class WebSession
{
    public:
        WebSession(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator) : socket_(io_service), closed(false)
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
            else
                TC_LOG_INFO("webserver", "send packet");
        }

        void Handle_NULL() { }
        void Handle_ClientSide() { }
        void HandleCheckCoreName();

        bool isActive() { return m_active; }

    private:
        void do_close()
        {
            socket_.close();
            closed = true;
        }

        void handler_buffer(WebByteBuffer buffer);

        void ReadHandler();

        void setActive(bool active) { m_active = active; }

        WebByteBuffer _writeBuffer;
        WebByteBuffer _readBuffer;

        tcp::socket socket_;

        bool closed;
        bool m_active;
};

#endif