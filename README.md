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

# TODOs/Ideas
- Change decay logic to be based on age rather than mass.
- Add/Update sensors to detect closest (both edge and center distance)
  bubble in the 8 main directions.
- Add genetic similarity sensor.
- Enable dynamic population size.
- Add statistical dashboards.
