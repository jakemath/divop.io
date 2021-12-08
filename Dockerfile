FROM python:3.10.0
WORKDIR /src
COPY ./src/ /src/
RUN ls -l
RUN g++ -std=c++20 -c -fPIC /src/cpp/bigint.cpp
RUN g++ -std=c++20 bigint.o -shared -o /src/libbigint.so -W
RUN python -m pip install -r /src/requirements.txt
