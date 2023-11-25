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

/**
 * Perform custom mutations on a given input
 *
 * @param[in] data pointer returned in afl_custom_init for this fuzz case
 * @param[in] buf Pointer to input data to be mutated
 * @param[in] buf_size Size of input data
 * @param[out] out_buf the buffer we will work on. we can reuse *buf. NULL on
 * error.
 * @param[in] add_buf Buffer containing the additional test case
 * @param[in] add_buf_size Size of the additional test case
 * @param[in] max_size Maximum size of the mutated output. The mutation must not
 *     produce data larger than max_size.
 * @return Size of the mutated output.
 */
// extern "C" size_t afl_custom_fuzz(MyMutator *mutator, uint8_t *buf, size_t buf_size, uint8_t **out_buf, uint8_t *add_buf, size_t add_buf_size, size_t max_size)
// {
//     if (buf_size <= 4)
//     {
//         std::cerr << "too small buffer, parsing failed" << std::endl;
//         return 0;
//     }
//     expcmake::Address input;
//     bool parse_ok = input.ParseFromArray(buf + 4, buf_size - 4);
//     if (!parse_ok)
//     {
//         std::cerr << "parsing failed" << std::endl;
//         return 0;
//     }

//     mutator->Mutate(&input, max_size);

//     std::string s;
//     input.SerializeToString(&s);

//     // 4 bytes for the size, plus the protobuf message size
//     size_t total_size = s.size() + 4;

//     *out_buf = static_cast<uint8_t *>(afl_realloc(reinterpret_cast<void **>(out_buf), total_size));
//     if (!*out_buf)
//     {
//         std::cerr << "afl_realloc failed" << std::endl;
//         ;
//         return 0;
//     }

//     // Write the size as a 4-byte value
//     uint32_t size = static_cast<uint32_t>(s.size());
//     memcpy(*out_buf, &size, 4);

//     // Copy the protobuf data
//     memcpy(*out_buf + 4, s.data(), s.size());

//     return total_size;
// }

extern "C" size_t afl_custom_fuzz(MyMutator *mutator, uint8_t *buf, size_t buf_size, uint8_t **out_buf, uint8_t *add_buf, size_t add_buf_size, size_t max_size)
{
    if (buf_size <= 4)
    {
        std::cerr << "Buffer too small, parsing failed" << std::endl;
        return 0;
    }

    size_t offset = 0;
    std::vector<uint8_t> temp_buffer; // Temporary buffer to accumulate the output

    size_t idx = 0;

    while (offset < buf_size)
    {
        // Read the size of the current message
        uint32_t message_size = 0;
        if (offset + 4 > buf_size)
            break; // Ensure there's enough buffer for the size
        memcpy(&message_size, buf + offset, 4);
        offset += 4;

        // Check if there's enough buffer for the message
        if (offset + message_size > buf_size)
            break;

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
