# build the image using build.sh script

FROM dockertsareva/fuzzing-base-image

COPY . /

RUN cd /grpc_server \
    && bash build.sh    

RUN ln -s $(which grpc_cpp_plugin) /usr/bin/protoc-gen-grpc && \
    protoc --proto_path=/grpc_server/proto  --cpp_out=. --grpc_out=. myproto/address.proto myproto/addressbook.proto

RUN make 

ENTRYPOINT ["bash", "/entrypoint.sh"]
