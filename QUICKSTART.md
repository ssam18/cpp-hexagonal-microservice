# Quick Start Guide

Get the Product Catalog Microservice running in 5 minutes!

## Prerequisites
- Docker
- Docker Compose
- curl (for testing)

## Step-by-Step Guide

### 1. Navigate to Project Directory
```bash
cd /home/samaresh/src/cpp-hexagonal-microservice
```

### 2. Start the Application
```bash
./start.sh
```

This will:
- Build the Docker image (first time takes ~5-10 minutes)
- Start MongoDB
- Start the Product Catalog service
- Initialize sample data

### 3. Wait for Services to Start
Watch the logs:
```bash
docker-compose logs -f
```

Look for:
```
product-service | Application started successfully!
```

Press `Ctrl+C` to exit logs view.

### 4. Test the API

**Health Check:**
```bash
curl http://localhost:8080/health
```

**Get All Products:**
```bash
curl http://localhost:8080/products | jq '.'
```

**Run Full Test Suite:**
```bash
./test-api.sh
```

### 5. Explore the API

**Get products by category:**
```bash
curl "http://localhost:8080/products?category=Electronics" | jq '.'
```

**Create a new product:**
```bash
curl -X POST http://localhost:8080/products \
  -H "Content-Type: application/json" \
  -d '{
    "name": "My Product",
    "description": "A great product",
    "price": 39.99,
    "stock": 50,
    "category": "Electronics"
  }' | jq '.'
```

### 6. View Logs
```bash
# Application logs
docker-compose logs -f product-service

# MongoDB logs
docker-compose logs -f mongodb

# All logs
docker-compose logs -f
```

### 7. Access MongoDB
```bash
docker exec -it product-catalog-mongodb mongosh
```

Then in the MongoDB shell:
```javascript
use product_catalog
db.products.find().pretty()
```

### 8. Stop the Application
```bash
./stop.sh
```

Or:
```bash
docker-compose down
```

## Troubleshooting

### Port 8080 already in use
Change the port in `docker-compose.yml`:
```yaml
ports:
  - "8081:8080"  # Use 8081 instead
```

### Cannot connect to Docker
```bash
sudo systemctl start docker
```

### Services fail to start
```bash
docker-compose down
docker-compose up --build
```

### Clear all data
```bash
docker-compose down -v
```

## Next Steps

1. **Read the Documentation**
   - [README.md](README.md) - Full documentation
   - [ARCHITECTURE.md](ARCHITECTURE.md) - Architecture details
   - [API_EXAMPLES.md](API_EXAMPLES.md) - API usage examples

2. **Explore the Code**
   - Start with `src/main.cpp` - Application entry point
   - Check `include/domain/` - Domain entities
   - Look at `include/adapters/` - HTTP and DB adapters

3. **Modify the Code**
   - Add new fields to Product
   - Implement a Category entity
   - Add authentication
   - Add pagination

4. **Test Changes**
   ```bash
   docker-compose down
   docker-compose up --build
   ```

## Common Commands

```bash
# Start services
./start.sh

# Stop services
./stop.sh

# Run tests
./test-api.sh

# View logs
docker-compose logs -f

# Rebuild after code changes
docker-compose up --build

# Clean everything
docker-compose down -v
docker system prune -a
```

## API Endpoints Summary

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/health` | Health check |
| GET | `/products` | Get all products |
| GET | `/products?category=X` | Get products by category |
| GET | `/products/{id}` | Get product by ID |
| POST | `/products` | Create product |
| PUT | `/products/{id}` | Update product |
| DELETE | `/products/{id}` | Delete product |

## Project Structure

```
cpp-hexagonal-microservice/
├── include/              # Header files
│   ├── adapters/        # HTTP & DB adapters
│   ├── domain/          # Core business logic
│   ├── service/         # Application services
│   ├── dto/             # Data transfer objects
│   ├── config/          # Configuration
│   └── utils/           # Utilities
├── src/                 # Implementation files
├── docker-compose.yml   # Docker orchestration
├── Dockerfile          # Container image
├── CMakeLists.txt      # Build configuration
└── *.sh                # Helper scripts
```

## Architecture Diagram

```
HTTP Request → ProductHandler → ProductService → ProductRepository → MongoDB
                 (Adapter)        (Business)      (Interface)      (Adapter)
```

## Support

For detailed information, see:
- [README.md](README.md) - Complete documentation
- [ARCHITECTURE.md](ARCHITECTURE.md) - Architecture patterns
- [API_EXAMPLES.md](API_EXAMPLES.md) - API examples

---

**You're all set! 🚀**

The Product Catalog Microservice is now running and ready to use.
