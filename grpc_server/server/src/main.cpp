#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/server_builder.h>

#include <condition_variable>

#include <thread>
#include <iostream>
#include <memory>

class AddressBookService final : public expcmake::AddressBook::Service
{
public:
    virtual ::grpc::Status GetAddress(::grpc::ServerContext *context, const ::expcmake::NameQuerry *request, ::expcmake::Address *response)
    {
        std::cout << "Server: GetAddress for '" << request->DebugString() << "'." << std::endl;
        if (request->name().rfind("Mike", 0) == 0)
        {
            std::abort();
        }
        if (request->surname().rfind("Black", 0) == 0)
        {
            std::abort();
        }
        if (request->name().rfind("M", 0) == 0)
        {
            if (request->surname().rfind("T", 0) == 0)
            {
                std::abort();
            }
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

void runServer(std::unique_ptr<grpc::Server> &server)
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort("127.0.0.1:50051", grpc::InsecureServerCredentials());
    AddressBookService service;
    builder.RegisterService(&service);
    server = builder.BuildAndStart();

    {
        std::lock_guard<std::mutex> lock(mtx);
        serverStarted = true;
    }
    cv.notify_one();
    std::cout << "server started\n"
              << std::endl;

    server->Wait();
}

int main()
{
    std::unique_ptr<grpc::Server> server;
    std::thread serverThread(runServer, std::ref(server));

    // Wait until server has started
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []
                { return serverStarted; });
    }

    auto channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
    std::unique_ptr<expcmake::AddressBook::Stub> stub = expcmake::AddressBook::NewStub(channel);

    while (true)
    {
        std::cout << "reading size from stdin" << std::endl;

        // Read the 4-byte size prefix
        uint32_t size;
        std::cin.read(reinterpret_cast<char *>(&size), sizeof(size));
        if (std::cin.eof() || std::cin.fail())
        {
            std::cout << "reading from stdin EOF or error" << std::endl;
            break;
        }

        std::cout << "size: " << size << std::endl;
        std::string input;
        if (size > 0)
        {
            // Read the protobuf message
            input = std::string(size, '\0');
            std::cin.read(&input[0], size);
            if (std::cin.eof() || std::cin.fail())
            {
                std::cerr << "Failed to read the complete protobuf message" << std::endl;
                continue;
            }
        }

        expcmake::NameQuerry query;
        if (!query.ParseFromString(input))
        {
            std::cerr << "Failed to parse input" << std::endl;
            continue;
        }

        std::cout << "sending query: " << query.DebugString();

        expcmake::Address result;

        grpc::ClientContext context;
        grpc::Status status = stub->GetAddress(&context, query, &result);

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
    }

    if (server)
    {
        server->Shutdown();
        serverThread.join();
    }
    return 0;
}
