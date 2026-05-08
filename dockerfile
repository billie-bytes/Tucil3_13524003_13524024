FROM ubuntu:24.04 AS builder
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    libglfw3-dev \
    libgl1-mesa-dev \
    xorg-dev \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . .
WORKDIR /app/build
RUN cmake .. && make all
CMD ["./IceSlide"]
