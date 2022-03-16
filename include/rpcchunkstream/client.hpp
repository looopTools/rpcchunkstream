#pragma once

#include "common.hpp"

#include <rpc/client.h>

#include <cstdint>
#include <string>
#include <utility>

namespace rpcchunkstream
{
class client
{
public:
    client(const std::string addr, const uint16_t port)
    {
        _client = std::unique_ptr<rpc::client>(new rpc::client(addr, port)); 
    }

    std::string open_stream(bool read = false)
    {
        return _client->call(STREAM_OPEN, read).as<std::string>(); 
    }

    bool write(const std::string& stream_id, auto& chunk)
    {
        return _client->call(STREAM_WRITE, stream_id, chunk); 
    }

    /// Reads a chunk 
    template<typename T> std::pair<bool, T> read(const std::string& stream_id)
    {
        return _client->call(STREAM_READ, stream_id).as<std::pair<bool, T>>();
    }

    bool stream_close(const std::string& stream_id)
    {
        return _client->call(STREAM_CLOSE, stream_id).as<bool>(); 
    }

    bool rewind(const std::string& stream_id, size_t off)
    {
        return _client->call(STREAM_REWIND, stream_id, off).as<bool>();
    }

    bool forward(const std::string& stream_id, size_t off)
    {
        return _client->call(STREAM_FORWARD, stream_id, off).as<bool>();
    }    

private:

    std::unique_ptr<rpc::client> _client; 
    
};
}
