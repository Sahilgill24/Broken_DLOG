g++ cpp/bsgs.cpp \
    -I/opt/homebrew/opt/openssl@3/include \
    -L/opt/homebrew/opt/openssl@3/lib \
    -I/opt/homebrew/opt/gmp/include \
    -L/opt/homebrew/opt/gmp/lib \
    -lgmp -lcrypto \
    -o runme



# This is for mac device , to test on Linux simply remove the -I and -L flags