# API Examples and Usage Guide

This document provides comprehensive examples for using the Product Catalog API.

## Base URL
```
http://localhost:8080
```

## Authentication
Currently, the API does not require authentication. In production, you would add authentication middleware in the adapters layer.

## Response Format

### Success Response
```json
{
  "id": "507f1f77bcf86cd799439011",
  "name": "Product Name",
  "description": "Product description",
  "price": 29.99,
  "stock": 100,
  "category": "Electronics",
  "status": "in-stock"
}
```

### Error Response
```json
{
  "code": 404,
  "message": "Product not found"
}
```

## Product Status Values
- `in-stock`: Stock > 10
- `low-stock`: Stock 1-10
- `out-of-stock`: Stock = 0

## API Endpoints

### 1. Health Check

Check if the service is running.

**Request:**
```bash
curl -X GET http://localhost:8080/health
```

**Response:**
```json
{
  "status": "healthy",
  "service": "product-catalog"
}
```

---

### 2. Get All Products

Retrieve all products from the catalog.

**Request:**
```bash
curl -X GET http://localhost:8080/products
```

**Response:**
```json
[
  {
    "id": "507f1f77bcf86cd799439011",
    "name": "Wireless Mouse",
    "description": "Ergonomic wireless mouse with USB receiver",
    "price": 29.99,
    "stock": 150,
    "category": "Electronics",
    "status": "in-stock"
  },
  {
    "id": "507f1f77bcf86cd799439012",
    "name": "Mechanical Keyboard",
    "description": "RGB mechanical keyboard with blue switches",
    "price": 89.99,
    "stock": 75,
    "category": "Electronics",
    "status": "in-stock"
  }
]
```

---

### 3. Get Products by Category

Filter products by category.

**Request:**
```bash
curl -X GET "http://localhost:8080/products?category=Electronics"
```

**Response:**
```json
[
  {
    "id": "507f1f77bcf86cd799439011",
    "name": "Wireless Mouse",
    "description": "Ergonomic wireless mouse with USB receiver",
    "price": 29.99,
    "stock": 150,
    "category": "Electronics",
    "status": "in-stock"
  }
]
```

**Supported Categories:**
- Electronics
- Accessories
- Gaming
- Office
- (any custom category)

---

### 4. Get Product by ID

Retrieve a specific product by its ID.

**Request:**
```bash
curl -X GET http://localhost:8080/products/507f1f77bcf86cd799439011
```

**Success Response (200 OK):**
```json
{
  "id": "507f1f77bcf86cd799439011",
  "name": "Wireless Mouse",
  "description": "Ergonomic wireless mouse with USB receiver",
  "price": 29.99,
  "stock": 150,
  "category": "Electronics",
  "status": "in-stock"
}
```

**Error Response (404 Not Found):**
```json
{
  "code": 404,
  "message": "Product not found"
}
```

---

### 5. Create Product

Add a new product to the catalog.

**Request:**
```bash
curl -X POST http://localhost:8080/products \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Gaming Mouse",
    "description": "High-precision gaming mouse with RGB lighting",
    "price": 59.99,
    "stock": 50,
    "category": "Gaming"
  }'
```

**Success Response (201 Created):**
```json
{
  "id": "507f1f77bcf86cd799439013",
  "name": "Gaming Mouse",
  "description": "High-precision gaming mouse with RGB lighting",
  "price": 59.99,
  "stock": 50,
  "category": "Gaming",
  "status": "in-stock"
}
```

**Error Response (400 Bad Request):**
```json
{
  "code": 400,
  "message": "Invalid product data"
}
```

**Validation Rules:**
- `name`: Required, non-empty string
- `description`: Optional string
- `price`: Required, must be >= 0
- `stock`: Required, must be >= 0
- `category`: Required, non-empty string

---

### 6. Update Product

Update an existing product.

**Request:**
```bash
curl -X PUT http://localhost:8080/products/507f1f77bcf86cd799439013 \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Gaming Mouse Pro",
    "description": "Professional gaming mouse with 16000 DPI",
    "price": 79.99,
    "stock": 30,
    "category": "Gaming"
  }'
```

**Success Response (200 OK):**
```json
{
  "id": "507f1f77bcf86cd799439013",
  "name": "Gaming Mouse Pro",
  "description": "Professional gaming mouse with 16000 DPI",
  "price": 79.99,
  "stock": 30,
  "category": "Gaming",
  "status": "in-stock"
}
```

**Error Response (404 Not Found):**
```json
{
  "code": 404,
  "message": "Product not found"
}
```

---

### 7. Delete Product

Remove a product from the catalog.

**Request:**
```bash
curl -X DELETE http://localhost:8080/products/507f1f77bcf86cd799439013
```

**Success Response (200 OK):**
```json
{
  "message": "Product deleted successfully"
}
```

**Error Response (404 Not Found):**
```json
{
  "code": 404,
  "message": "Product not found"
}
```

---

## Complete Workflow Example

### Scenario: Managing a new product

#### Step 1: Create a product
```bash
RESPONSE=$(curl -s -X POST http://localhost:8080/products \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Wireless Headphones",
    "description": "Noise-cancelling wireless headphones",
    "price": 149.99,
    "stock": 25,
    "category": "Electronics"
  }')

echo $RESPONSE
PRODUCT_ID=$(echo $RESPONSE | jq -r '.id')
echo "Created product with ID: $PRODUCT_ID"
```

#### Step 2: Retrieve the product
```bash
curl -X GET http://localhost:8080/products/$PRODUCT_ID | jq '.'
```

#### Step 3: Update stock after sale
```bash
curl -X PUT http://localhost:8080/products/$PRODUCT_ID \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Wireless Headphones",
    "description": "Noise-cancelling wireless headphones",
    "price": 149.99,
    "stock": 20,
    "category": "Electronics"
  }' | jq '.'
```

#### Step 4: Check category inventory
```bash
curl -X GET "http://localhost:8080/products?category=Electronics" | jq '.'
```

#### Step 5: Delete discontinued product
```bash
curl -X DELETE http://localhost:8080/products/$PRODUCT_ID | jq '.'
```

---

## Using with Different Tools

### Using with HTTPie
```bash
# Install HTTPie
sudo apt-get install httpie  # Ubuntu/Debian
brew install httpie          # macOS

# Get all products
http GET localhost:8080/products

# Create product
http POST localhost:8080/products \
  name="Test Product" \
  description="Test description" \
  price:=19.99 \
  stock:=100 \
  category="Test"

# Update product
http PUT localhost:8080/products/{id} \
  name="Updated Product" \
  description="Updated" \
  price:=24.99 \
  stock:=50 \
  category="Test"
```

### Using with Postman

1. **Import Collection**: Use the following JSON to create a Postman collection

2. **Environment Variables**:
   - `base_url`: `http://localhost:8080`
   - `product_id`: (save from create response)

3. **Pre-request Script** (for Create request):
   ```javascript
   // No setup needed
   ```

4. **Test Script** (for Create request):
   ```javascript
   pm.test("Status is 201", function() {
       pm.response.to.have.status(201);
   });
   
   pm.test("Product has ID", function() {
       var jsonData = pm.response.json();
       pm.expect(jsonData.id).to.exist;
       pm.environment.set("product_id", jsonData.id);
   });
   ```

### Using with Python requests
```python
import requests
import json

BASE_URL = "http://localhost:8080"

# Get all products
response = requests.get(f"{BASE_URL}/products")
products = response.json()
print(json.dumps(products, indent=2))

# Create product
new_product = {
    "name": "Python Test Product",
    "description": "Created from Python",
    "price": 29.99,
    "stock": 50,
    "category": "Test"
}
response = requests.post(
    f"{BASE_URL}/products",
    json=new_product
)
created = response.json()
product_id = created["id"]
print(f"Created product: {product_id}")

# Update product
updated_product = {
    "name": "Python Updated Product",
    "description": "Updated from Python",
    "price": 34.99,
    "stock": 40,
    "category": "Test"
}
response = requests.put(
    f"{BASE_URL}/products/{product_id}",
    json=updated_product
)
print(response.json())

# Delete product
response = requests.delete(f"{BASE_URL}/products/{product_id}")
print(response.json())
```

---

## Error Codes

| Code | Meaning | Common Causes |
|------|---------|---------------|
| 200 | OK | Successful GET, PUT, DELETE |
| 201 | Created | Successful POST |
| 400 | Bad Request | Invalid JSON, missing required fields, validation errors |
| 404 | Not Found | Product ID doesn't exist |
| 500 | Internal Server Error | Database connection issues, server errors |

---

## Rate Limiting

Currently, there is no rate limiting. For production, consider adding:
- Rate limiting middleware in the adapters layer
- API key authentication
- Request throttling

---

## Best Practices

1. **Always check response status codes**
2. **Handle errors gracefully**
3. **Use appropriate HTTP methods**
4. **Validate input on client side**
5. **Store sensitive data (API keys) securely**

---

## Monitoring

### Check Service Health
```bash
# Should return healthy status
curl http://localhost:8080/health

# If unhealthy, check logs
docker-compose logs product-service
```

### Check MongoDB Connection
```bash
docker exec -it product-catalog-mongodb mongosh

# In mongosh:
use product_catalog
db.products.find().pretty()
```

---

## Troubleshooting

### Service not responding
```bash
# Check if services are running
docker-compose ps

# Restart services
docker-compose restart

# View logs
docker-compose logs -f product-service
```

### Cannot connect to MongoDB
```bash
# Check MongoDB status
docker-compose logs mongodb

# Verify MongoDB is healthy
docker-compose ps
```

### Invalid JSON errors
- Ensure Content-Type header is set: `application/json`
- Validate JSON syntax with online tools
- Check for trailing commas, quotes

---

## Next Steps

- Add authentication (JWT, OAuth)
- Implement pagination for large result sets
- Add sorting and advanced filtering
- Implement caching (Redis)
- Add API versioning
- Implement rate limiting
