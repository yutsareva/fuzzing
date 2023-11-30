#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

#include <iostream>
#include "myproto/address.pb.h"
#include "myproto/addressbook.grpc.pb.h"

#include "alloc-inl.h"

#include <thread>

#include "libprotobuf-mutator/src/mutator.h"

#include <bits/stdc++.h>

class MyMutator : public protobuf_mutator::Mutator
{
};

extern "C" MyMutator *afl_custom_init(void *afl, unsigned int seed)
{
    MyMutator *mutator = new MyMutator();

    mutator->RegisterPostProcessor(
        expcmake::Address::descriptor(),
        [](google::protobuf::Message *message, unsigned int seed)
        {
            // tba
        });

    srand(seed);
    return mutator;
}

extern "C" size_t afl_custom_fuzz(MyMutator *mutator, uint8_t *buf, size_t buf_size, uint8_t **out_buf, uint8_t *add_buf, size_t add_buf_size, size_t max_size)
{
    if (buf_size <= 4)
    {
        std::cerr << "Buffer too small, parsing failed" << std::endl;
        return 0;
    }

    size_t offset = 0;
    std::vector<uint8_t> temp_buffer;

    size_t idx = 0;

    while (offset < buf_size)
    {
        uint32_t message_size = 0;
        if (offset + 4 > buf_size)
        {
            break;
        }
        memcpy(&message_size, buf + offset, 4);
        offset += 4;

        if (offset + message_size > buf_size)
        {
            break;
        }

        expcmake::NameQuerry input;
        bool parse_ok = input.ParseFromArray(buf + offset, message_size);
        if (!parse_ok)
        {
            std::cerr << "Parsing failed" << std::endl;
            break;
        }
        offset += message_size;

        mutator->Mutate(&input, max_size);

        std::cout << "query " << ++idx << ": " << input.DebugString() << "\n"
                  << std::endl;

        std::string s;
        input.SerializeToString(&s);

        // Append the size as a 4-byte value
        uint32_t size = static_cast<uint32_t>(s.size());
        temp_buffer.insert(temp_buffer.end(), reinterpret_cast<uint8_t *>(&size), reinterpret_cast<uint8_t *>(&size) + 4);

        // Append the protobuf data
        temp_buffer.insert(temp_buffer.end(), s.begin(), s.end());
    }

    size_t total_size = temp_buffer.size();
    if (total_size > 0)
    {
        *out_buf = static_cast<uint8_t *>(afl_realloc(reinterpret_cast<void **>(out_buf), total_size));
        if (!*out_buf)
        {
            std::cerr << "afl_realloc failed" << std::endl;
            return 0;
        }

        memcpy(*out_buf, temp_buffer.data(), total_size);
    }

    return total_size;
}

extern "C" void afl_custom_deinit(void *data)
{
    delete ((MyMutator *)data);
}
