# Agar.ai
AI simulation of agar.io

# How to
Build:
```
cmake -S . -B build
cmake --build build
```

Run tests:
```
cmake --build build --target agar.test
build/src/agar.test
```

Run simulation:
```
cmake --build build --target agar.ai
build/src/agar.ai
```
