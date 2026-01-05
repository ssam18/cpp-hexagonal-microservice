# Hexagonal Architecture - Detailed Explanation

## What is Hexagonal Architecture?

Hexagonal Architecture, also known as **Ports and Adapters** architecture, was introduced by Alistair Cockburn. The main goal is to create loosely coupled application components that can be easily connected to their software environment through ports and adapters.

## Core Concepts

### 1. The Hexagon (Application Core)
The hexagon represents the application core containing:
- **Domain Entities**: Pure business objects (Product)
- **Business Logic**: Rules and operations
- **Ports (Interfaces)**: Boundaries of the application

### 2. Ports
Ports are interfaces that define how the application can be used or how it uses external services.

**Primary Ports (Driving/Inbound)**:
- Interfaces that allow external actors to interact with the application
- Example: `ProductService` interface used by HTTP handlers

**Secondary Ports (Driven/Outbound)**:
- Interfaces the application needs to interact with external services
- Example: `ProductRepository` interface for data persistence

### 3. Adapters
Adapters are implementations of ports that connect the application to the outside world.

**Primary Adapters (Driving/Inbound)**:
- Drive the application (HTTP REST API, CLI, GraphQL)
- Example: `ProductHandler` - REST API adapter using Boost.Beast

**Secondary Adapters (Driven/Outbound)**:
- Driven by the application (Database, Message Queue, External APIs)
- Example: `ProductRepositoryMongo` - MongoDB adapter

## In Our Product Catalog Microservice

```
                        External World
                              │
                              │
        ┌─────────────────────┼─────────────────────┐
        │                     │                     │
        │     HTTP Request    │                     │
        │                     ▼                     │
        │         ┌───────────────────┐             │
        │         │  ProductHandler   │ ◄───────────┤── Primary Adapter
        │         │  (Boost.Beast)    │             │   (Inbound/Driving)
        │         └───────────────────┘             │
        │                     │                     │
        │                     │ uses                │
        │                     ▼                     │
        │         ┌───────────────────┐             │
        │         │  ProductService   │ ◄───────────┤── Primary Port
        │         │    (Interface)    │             │   (Inbound)
        │         └───────────────────┘             │
        │                     │                     │
        │                     │ orchestrates        │
        │                     ▼                     │
        │    ╔═══════════════════════════════╗     │
        │    ║    APPLICATION CORE           ║     │
        │    ║                               ║     │
        │    ║  ┌─────────────────────────┐ ║     │
        │    ║  │   Domain Entities       │ ║     │
        │    ║  │   (Product)             │ ║     │
        │    ║  │   • Business Rules      │ ║     │
        │    ║  │   • Validation Logic    │ ║     │
        │    ║  └─────────────────────────┘ ║     │
        │    ║             │                 ║     │
        │    ║             ▼                 ║     │
        │    ║  ┌─────────────────────────┐ ║     │
        │    ║  │  ProductRepository      │ ║     │
        │    ║  │  (Interface/Port)       │ ║     │
        │    ║  └─────────────────────────┘ ║     │
        │    ║                               ║     │
        │    ╚═══════════════════════════════╝     │
        │                     │                     │
        │                     │ implements          │
        │                     ▼                     │
        │         ┌───────────────────┐             │
        │         │ProductRepositoryMo│ ◄───────────┤── Secondary Adapter
        │         │      (MongoDB)    │             │   (Outbound/Driven)
        │         └───────────────────┘             │
        │                     │                     │
        │                     │                     │
        └─────────────────────┼─────────────────────┘
                              │
                              ▼
                          MongoDB
```

## Benefits in Our Implementation

### 1. **Independence from Frameworks**
```cpp
// Domain entity doesn't depend on MongoDB, HTTP, or any framework
class Product {
    // Pure business logic
    bool isAvailable() const { return stock_ > 0; }
};
```

### 2. **Testability**
```cpp
// Easy to create mock implementations
class MockProductRepository : public ProductRepository {
    // In-memory implementation for testing
};

// Test the service without database
auto mockRepo = std::make_shared<MockProductRepository>();
auto service = std::make_shared<ProductService>(mockRepo);
```

### 3. **Swappable Adapters**
Want to use PostgreSQL instead of MongoDB? Just create:
```cpp
class ProductRepositoryPostgres : public ProductRepository {
    // PostgreSQL implementation
};
```
No changes needed in domain or service layers!

### 4. **Multiple Interfaces**
Want to add a GraphQL API? Just create:
```cpp
class GraphQLHandler {
    std::shared_ptr<ProductService> service_;
    // GraphQL implementation
};
```

## Dependency Flow

```
HTTP Handler ──uses──> ProductService ──uses──> ProductRepository (interface)
                                                        ▲
                                                        │
                                                  implements
                                                        │
                                              ProductRepositoryMongo
```

**Key Point**: Dependencies point INWARD toward the domain.

## Comparison: Traditional vs Hexagonal

### Traditional Layered Architecture
```
Controller → Service → Repository → Database
    ↓           ↓           ↓
  Tight coupling across layers
```

### Hexagonal Architecture
```
HTTP Adapter → Service → Repository Interface ← MongoDB Adapter
                            ↑
                      (Dependency Inversion)
```

## File Organization

```
include/
  domain/           # Core domain (NO external dependencies)
    Product.h
    ProductRepository.h (interface)
  
  service/          # Business logic (depends only on domain)
    ProductService.h
  
  adapters/         # External world adapters
    ProductHandler.h      (Primary - HTTP)
    ProductRepositoryMongo.h (Secondary - DB)
```

## When to Use Hexagonal Architecture

✅ **Use When:**
- Complex business logic
- Multiple interfaces (REST, GraphQL, CLI)
- Need to swap technologies
- High testability requirement
- Long-term maintenance

❌ **Avoid When:**
- Simple CRUD applications
- Rapid prototyping
- Small, short-lived projects

## Real-World Examples

1. **E-commerce Platform**
   - Ports: OrderService, PaymentService
   - Primary Adapters: REST API, Mobile API, Admin Web
   - Secondary Adapters: MySQL, Redis, PayPal API, Stripe API

2. **IoT Platform**
   - Ports: DeviceService, DataService
   - Primary Adapters: MQTT Listener, HTTP API, WebSocket
   - Secondary Adapters: TimescaleDB, InfluxDB, AWS IoT

## Common Patterns

### Dependency Injection
```cpp
// Wire dependencies at startup (main.cpp)
auto repository = std::make_shared<ProductRepositoryMongo>(uri, db);
auto service = std::make_shared<ProductService>(repository);
auto handler = std::make_shared<ProductHandler>(service);
```

### Interface Segregation
```cpp
// Small, focused interfaces
class ProductRepository {
    virtual Product findById(string id) = 0;
    // Only what's needed
};
```

### Repository Pattern
```cpp
// Abstract data access
class ProductRepository {
    // Hide implementation details
    // Could be SQL, NoSQL, In-Memory, File
};
```

## Testing Strategy

### Unit Tests
```cpp
// Test domain entities
TEST(ProductTest, IsAvailable) {
    Product p("1", "Test", "Desc", 10.0, 5, "Cat");
    ASSERT_TRUE(p.isAvailable());
}
```

### Integration Tests
```cpp
// Test with real database
auto repo = std::make_shared<ProductRepositoryMongo>(testUri, testDb);
auto service = std::make_shared<ProductService>(repo);
// Test actual CRUD operations
```

### End-to-End Tests
```bash
# Test HTTP API
curl -X GET http://localhost:8080/products
```

## Further Reading

- [Alistair Cockburn - Hexagonal Architecture](https://alistair.cockburn.us/hexagonal-architecture/)
- [Clean Architecture by Robert C. Martin](https://blog.cleancoder.com/uncle-bob/2012/08/13/the-clean-architecture.html)
- [Domain-Driven Design by Eric Evans](https://www.domainlanguage.com/ddd/)
- [Ports and Adapters Pattern](https://herbertograca.com/2017/11/16/explicit-architecture-01-ddd-hexagonal-onion-clean-cqrs-how-i-put-it-all-together/)
