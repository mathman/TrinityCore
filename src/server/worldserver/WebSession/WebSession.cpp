#include "World.h"
#include "WebOpcodes.h"
#include "WebSession.h"
#include <iostream>


void WebSessionThread(const std::string& host, const std::string& port)
{
    try
    {
        // Any program that uses asio need to have at least one io_service object
        boost::asio::io_service io_service;

        // Convert the server name that was specified as a parameter to the application, to a TCP endpoint.
        // To do this, we use an ip::tcp::resolver object.
        tcp::resolver resolver(io_service);

        // A resolver takes a query object and turns it into a list of endpoints.
        // We construct a query using the name of the server, specified in argv[1],
        // and the name of the service, in this case "daytime".
        tcp::resolver::query query(host.c_str(), port.c_str());

        // The list of endpoints is returned using an iterator of type ip::tcp::resolver::iterator.
        // A default constructed ip::tcp::resolver::iterator object can be used as an end iterator.
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        WebSession web(io_service, endpoint_iterator);

        TC_LOG_INFO("webserver", "Bound to %s:%s", host.c_str(), port.c_str());

        while (!World::IsStopped())
        {
            web.run();
        }
    }
    // handle any exceptions that may have been thrown.
    catch (std::exception& e)
    {
        TC_LOG_ERROR("webserver", "Couldn't bind to %s:%s with error : %s", host.c_str(), port.c_str(), e.what());
        exit(-1);
    }
}

void WebSession::run()
{
    if (closed)
        return;

    boost::array<int8, READ_BLOCK_SIZE> buffer;
    boost::system::error_code error;

    size_t len = socket_.read_some(boost::asio::buffer(buffer, READ_BLOCK_SIZE), error);

    if (error)
        do_close();

    WebByteBuffer message(len);
    message.append<int8>(buffer.data(), len);

    handler_buffer(message);
}

void WebSession::handler_buffer(WebByteBuffer buffer)
{
    _readBuffer = buffer;
    int opcode = _readBuffer.getOpcode();
    int lenght = _readBuffer.getLenghtData();
    TC_LOG_INFO("webserver", "Packet receive lenght %d, opcode %d, lenght message %d", _readBuffer.size(), opcode, lenght);

    ReadHandler();
}

void WebSession::ReadHandler()
{
    if (_readBuffer.getOpcode() >= NUM_MSG_TYPES_WEB)
    {
        TC_LOG_ERROR("webserver", "Received non-existed opcode %s from %s", GetOpcodeNameForLoggingWeb(_readBuffer.getOpcode()).c_str());
        return;
    }
    else
    {
        WebOpcodeHandler& opHandle = webOpcodeTable[_readBuffer.getOpcode()];
        (this->*opHandle.handler)();
    }
}

void WebSession::HandleCheckCoreName()
{
    std::string message = "TrinityCore";
    size_t size = message.length();
    WebByteBuffer buffer(size, CMSG_WEB_NAME_RESPONSE);
    buffer.WriteString(message);
    sendPacket(buffer);
}