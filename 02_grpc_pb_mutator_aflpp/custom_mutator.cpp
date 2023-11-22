#include "custom_mutator.h"

using std::cin;
using std::cout;
using std::endl;

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

#include <iostream>
#include "myproto/address.pb.h"
#include "myproto/addressbook.grpc.pb.h"

#include <thread>
#include <chrono>

extern "C" void afl_custom_fuzz_send(MyMutator *mut, uint8_t *buf, size_t buf_size)
{
    expcmake::NameQuerry input;
    expcmake::Address result;
    bool parse_ok = input.ParseFromArray(buf, buf_size);
    if (!parse_ok)
    {
        std::cerr << "ERROR : afl_custom_fuzz_send" << parse_ok << std::endl;
        return;
    }
    else
    {
        std::cout << "sending: " << input.DebugString() << std::endl;
    }

    std::thread async_task([input, result]() mutable
                           {
                               std::this_thread::sleep_for(std::chrono::milliseconds(100));

                               auto channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
                               std::unique_ptr<expcmake::AddressBook::Stub> stub = expcmake::AddressBook::NewStub(channel);
                               grpc::ClientContext context;
                               grpc::Status status = stub->GetAddress(&context, input, &result);

                               if (status.ok())
                               {
                                   std::cout << "gRPC succeeded" << std::endl;
                               }
                               else
                               {
                                   std::cout << "gRPC Error - Code: " << status.error_code()
                                             << ", Message: " << status.error_message()
                                             << ", Details: " << status.error_details()
                                             << std::endl;
                               }

                               // std::cout << "Name: " << result.name() << std::endl;
                               // std::cout << "City: " << result.city() << std::endl;
                               // std::cout << "Zip:  " << result.zip() << std::endl;
                               // std::cout << "Street: " << result.street() << std::endl;
                               // std::cout << "Country: " << result.country() << std::endl;
                           });

    async_task.detach();
}

extern "C" MyMutator *afl_custom_init(void *afl, unsigned int seed)
{
    MyMutator *mutator = new MyMutator();

    mutator->RegisterPostProcessor(
        expcmake::Address::descriptor(),
        [](google::protobuf::Message *message, unsigned int seed)
        {
            // ?
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
extern "C" size_t afl_custom_fuzz(MyMutator *mutator,                    // return value from afl_custom_init
                                  uint8_t *buf, size_t buf_size,         // input data to be mutated
                                  uint8_t **out_buf,                     // output buffer
                                  uint8_t *add_buf, size_t add_buf_size, // add_buf can be NULL
                                  size_t max_size)
{
    // This function can be named either "afl_custom_fuzz" or "afl_custom_mutator"
    // A simple test shows that "buf" will be the content of the current test case
    // "add_buf" will be the next test case ( from AFL++'s input queue )
    expcmake::Address input;

    bool parse_ok = input.ParseFromArray(buf, buf_size);
    if (!parse_ok)
    {
        std::cerr << "parsing failed!!!";
        // Invalid serialize protobuf data. Don't mutate.
        // Return a dummy buffer. Also mutated_size = 0
        static uint8_t *dummy = new uint8_t[10]; // dummy buffer with no data
        *out_buf = dummy;
        return 0;
    }
    // mutate the protobuf
    mutator->Mutate(&input, max_size);

    std::string s;
    input.SerializeToString(&s);

    // Copy to a new buffer ( mutated_out )
    size_t mutated_size = s.size() <= max_size ? s.size() : max_size;
    uint8_t *mutated_out = new uint8_t[mutated_size + 1];
    memcpy(mutated_out, s.c_str(), mutated_size);

    // Assign the mutated data and return mutated_size
    *out_buf = mutated_out;
    return mutated_size;
}

extern "C" void afl_custom_deinit(void *data)
{
    delete ((MyMutator *)data);
    return;
}
