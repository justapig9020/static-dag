# Requriments

- Meson
- Ninja
- Bear

# Setup

```
mkdir -p subprojects
meson wrap install gtest
meson setup build
```

# Compile

```
cd build
meson compile
```

# Test

```
./dag-test
```

# Example

```
./dag-example
```
