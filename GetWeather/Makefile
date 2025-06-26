BIN = GetWeather

GETWEATHER_SRC = main.cpp GetWeather.cpp GetWeather.h
GETWEATHER_BIN = GetWeather
GETWEATHER_INC = -I./dependencies/curl/include -I./dependencies/zlib/include -I./dependencies/openssl/include
GETWEATHER_LIB = -L./dependencies/curl/lib -L./dependencies/zlib/lib -L./dependencies/openssl/lib -lcurl -lz -lssl -lcrypto -lpthread

all: $(GETWEATHER_BIN)

$(GETWEATHER_BIN): $(GETWEATHER_SRC)
	arm-linux-gnueabi-g++ -O3 -Wall -std=c++17 $(GETWEATHER_INC) -o $(GETWEATHER_BIN) $(GETWEATHER_SRC) $(GETWEATHER_LIB) -static-libstdc++ -static-libgcc -ldl

clean:
	rm -f $(GETWEATHER_BIN)