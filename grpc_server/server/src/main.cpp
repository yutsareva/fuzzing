#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/server_builder.h>

#include <condition_variable>

#include <thread>
#include <iostream>
#include <memory>
#include <fstream>

class AddressBookService final : public expcmake::AddressBook::Service
{
public:
    virtual ::grpc::Status GetAddress(::grpc::ServerContext *context, const ::expcmake::NameQuerry *request, ::expcmake::Address *response)
    {
        if (request->name().rfind("Mike", 0) == 0) {
            std::abort();
        }
        if (request->name() == "Lucy") {
            std::abort();
        }
        if (!strcmp(request->name().c_str(), "Rick")) {
            std::abort();
        }

        response->set_name("Peter Peterson");
        response->set_zip("12345");
        response->set_country("Superland");

        return grpc::Status::OK;
    }
};

std::mutex mtx;
std::condition_variable cv;
bool serverStarted = false;
AddressBookService service;

void runServer(std::unique_ptr<grpc::Server> &server)
{
    grpc::ServerBuilder builder;
    // builder.AddListeningPort("unix:/tmp/grpc.sock", grpc::InsecureServerCredentials());
    // AddressBookService service;
    builder.RegisterService(&service);
    server = builder.BuildAndStart();

    {
        std::lock_guard<std::mutex> lock(mtx);
        serverStarted = true;
    }
    cv.notify_one();

    server->Wait();
}

__AFL_FUZZ_INIT();

int main()
{
    #ifdef __AFL_HAVE_MANUAL_CONTROL
    __AFL_INIT();
    #endif

    unsigned char *input = __AFL_FUZZ_TESTCASE_BUF;  // must be after __AFL_INIT
                                                    // and before __AFL_LOOP!
    std::unique_ptr<grpc::Server> server;
    std::thread serverThread(runServer, std::ref(server));

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []
                { return serverStarted; });
    }
    // auto channel = grpc::CreateChannel("unix:/tmp/grpc.sock", grpc::InsecureChannelCredentials());
    // std::unique_ptr<expcmake::AddressBook::Stub> stub = expcmake::AddressBook::NewStub(channel);

      while (__AFL_LOOP(1000)) {
        int len = __AFL_FUZZ_TESTCASE_LEN;
        auto buf = input;
        while (true)
        {
            uint32_t size;
            if (len < sizeof(size)) {
                break;
            }
            std::memcpy(&size, buf, sizeof(size));
            buf += sizeof(size);
            len -= sizeof(size);

            if (size == 0 || size > len) {
                break;
            }

            expcmake::NameQuerry query;
            if (!query.ParseFromArray(buf, size)) {
                break;
            }
            buf += size;
            len -= size;

            expcmake::Address result;

    //        grpc::ClientContext context;
    //        grpc::Status status = stub->GetAddress(&context, query, &result);
            grpc::ServerContext context;
            auto status = service.GetAddress(&context, &query, &result);
        }
    }

    if (server)
    {
       server->Shutdown();
       serverThread.join();
    }

    return 0;
}
