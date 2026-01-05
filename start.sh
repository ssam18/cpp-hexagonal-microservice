#!/bin/bash

# Build and run script for Product Catalog Microservice

set -e

echo "==================================="
echo "Product Catalog Microservice"
echo "==================================="
echo ""

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}Building Docker images...${NC}"
docker compose build

echo ""
echo -e "${BLUE}Starting services...${NC}"
docker compose up -d

echo ""
echo -e "${GREEN}Services started successfully!${NC}"
echo ""
echo "MongoDB: http://localhost:27017"
echo "API Service: http://localhost:8080"
echo ""
echo "Available endpoints:"
echo "  - GET    http://localhost:8080/health"
echo "  - GET    http://localhost:8080/products"
echo "  - GET    http://localhost:8080/products/{id}"
echo "  - POST   http://localhost:8080/products"
echo "  - PUT    http://localhost:8080/products/{id}"
echo "  - DELETE http://localhost:8080/products/{id}"
echo ""
echo "To view logs: docker compose logs -f"
echo "To stop services: docker compose down"
echo ""
