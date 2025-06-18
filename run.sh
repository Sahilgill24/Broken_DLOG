g++ cpp/bsgs.cpp  \
    -I/opt/homebrew/opt/gmp/include \
    -L/opt/homebrew/opt/gmp/lib \
    -lgmp \
    -o runme


./runme
# This is my command for MAC device, won't run on Linux or others
