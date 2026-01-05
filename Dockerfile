FROM ubuntu:22.04 AS builder

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg
WORKDIR /opt
RUN git clone https://github.com/Microsoft/vcpkg.git
WORKDIR /opt/vcpkg
RUN ./bootstrap-vcpkg.sh

# Set up vcpkg environment
ENV VCPKG_ROOT=/opt/vcpkg
ENV PATH="${VCPKG_ROOT}:${PATH}"

# Copy project files
WORKDIR /app
COPY CMakeLists.txt vcpkg.json ./
COPY include/ ./include/
COPY src/ ./src/

# Install dependencies using vcpkg
RUN vcpkg install

# Build the application
RUN cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
RUN cmake --build build --config Release

# Runtime stage
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libssl3 \
    && rm -rf /var/lib/apt/lists/*

# Copy the built application
COPY --from=builder /app/build/ProductCatalogService /usr/local/bin/

# Set environment variables (can be overridden)
ENV SERVER_ADDRESS=0.0.0.0
ENV SERVER_PORT=8080
ENV MONGO_URI=mongodb://mongodb:27017
ENV DATABASE_NAME=product_catalog

# Expose port
EXPOSE 8080

# Run the application
CMD ["ProductCatalogService"]
