# crypto-market-watcher

A high-performance C++ application for real-time crypto price ingestion, metric computation, alerting, and terminal dashboard visualization.

This project is designed as a portfolio-quality example of:

- Modern C++ (C++20)
- Real-time systems & multithreading
- Clean separation of networking, domain logic, and UI
- In-memory data structures and metrics
- Unit testing and CI

## Build

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
ctest
```

## License

MIT
