#!/bin/bash

# Test script for Product Catalog Microservice

set -e

BASE_URL="http://localhost:8080"

echo "==================================="
echo "Testing Product Catalog API"
echo "==================================="
echo ""

# Test health endpoint
echo "1. Testing health endpoint..."
curl -s -X GET "$BASE_URL/health" | jq '.'
echo -e "\n"

# Get all products
echo "2. Getting all products..."
curl -s -X GET "$BASE_URL/products" | jq '.'
echo -e "\n"

# Get products by category
echo "3. Getting products by category (Electronics)..."
curl -s -X GET "$BASE_URL/products?category=Electronics" | jq '.'
echo -e "\n"

# Create a new product
echo "4. Creating a new product..."
PRODUCT_DATA='{
  "name": "Test Product",
  "description": "This is a test product",
  "price": 19.99,
  "stock": 100,
  "category": "Test"
}'
RESPONSE=$(curl -s -X POST "$BASE_URL/products" \
  -H "Content-Type: application/json" \
  -d "$PRODUCT_DATA")
echo "$RESPONSE" | jq '.'
PRODUCT_ID=$(echo "$RESPONSE" | jq -r '.id')
echo -e "\nCreated product with ID: $PRODUCT_ID\n"

# Get the created product
echo "5. Getting the created product..."
curl -s -X GET "$BASE_URL/products/$PRODUCT_ID" | jq '.'
echo -e "\n"

# Update the product
echo "6. Updating the product..."
UPDATE_DATA='{
  "name": "Updated Test Product",
  "description": "This product has been updated",
  "price": 24.99,
  "stock": 50,
  "category": "Test"
}'
curl -s -X PUT "$BASE_URL/products/$PRODUCT_ID" \
  -H "Content-Type: application/json" \
  -d "$UPDATE_DATA" | jq '.'
echo -e "\n"

# Delete the product
echo "7. Deleting the product..."
curl -s -X DELETE "$BASE_URL/products/$PRODUCT_ID" | jq '.'
echo -e "\n"

# Verify deletion
echo "8. Verifying deletion (should return 404)..."
curl -s -X GET "$BASE_URL/products/$PRODUCT_ID" | jq '.'
echo -e "\n"

echo "==================================="
echo "All tests completed!"
echo "==================================="
