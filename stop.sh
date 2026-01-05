#!/bin/bash

# Stop script for Product Catalog Microservice

set -e

echo "Stopping Product Catalog Microservice..."
docker compose down

echo "Services stopped successfully!"
