#pragma once

#include "common.hpp"

#include <rpc/server.h>



namespace rpcchunkstream
{
template<class DataHandler>    
class server
{
public:
    server(DataHandler& handler, const uint16_t port)
    {
        _handler = handler;
        _server = std::unique_ptr<rpc::server>(new rpc::server(port));
        _server->bind(STREAM_OPEN, &this->stream_open);
        _server->bind(STREAM_CLOSE, &this->stream_close);
        _server->bind(STREAM_WRITE, &this->stream_write); 
        _server->bind(STREAM_READ, &this->stream_read);
        _server->bind(STREAM_REWIND, &this->stream_rewind);
        _server->bind(STREAM_FORWARD, &this->stream_forward);
    }
    
    server(DataHandler& handler, const std::string& addr, const uint16_t port) 
                                                                                 
    {
        _handler = handler;
        _server = std::unique_ptr<rpc::server>(new rpc::server(addr, port));        
    }

    void run()
    {
        _server->run(); 
    }

private:
    
    std::string stream_open(bool out = false)
    {
        std::string id; // Replace with UUID from stduuid
        _handler.start_stream(id, out);
        return id; 
    }

    bool stream_write(const std::string& stream_id, auto& chunk)
    {
        return _handler.write(stream_id, chunk); 
    }

    auto stream_read(const std::string& stream_id)
    {
        return _handler.read(stream_id); 
    }

    bool rewind(const std::string& stream_id, const size_t off)
    {
        return _handler.rewind(stream_id, off); 
    }

    bool foward(const std::string stream_id, const size_t off)
    {
        return _handler.forward(stream_id, off);
    }
    
    bool stream_close(const std::string& stream_id)
    {
        return _handler.close_stream(stream_id); 
    }

    
private:
    std::unique_ptr<rpc::server> _server;
    DataHandler _handler;
};
}
