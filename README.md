# static-dag

## Introduction

`static-dag` is a library for statically declaring a Directed Acyclic Graph (DAG), specifically designed for embedded environments where dynamic memory allocation is not available. This library provides a `DAGNode` that can be easily embedded into any structure, making it particularly useful for dependency control in embedded systems.

## Features

- **Static Declaration**: Allows for static definition of each node's in-degree, which is suitable for fixed embedded systems.
- **No Dynamic Memory Allocation**: Designed to work entirely without dynamic memory allocation, making it ideal for embedded systems that do not support this feature.
- **Flexible Integration**: The `DAGNode` can be embedded into any structure, providing a flexible solution for dependency management in various embedded applications.

## Requirements

- **Operating System**: This project currently works on **Linux**. If you need to compile on platforms other than Linux, please remove the test-related parts in `meson.build`.
- `Meson`
- `Ninja`

## Setup

To set up the project, use the following commands:

```bash
mkdir -p subprojects
meson wrap install gtest
meson setup build
```

## Compile

After setting up, compile the project with:

```bash
cd build
meson compile
```

## Testing

To run the tests, execute:

```bash
./dag-test
```

## Examples

The example will output a Mermaid-formatted markdown. You can use Mermaid to visualize the process of node addition and deletion.
To run an example using the static-dag library:

```bash
./dag-example
```

## Usage

For a detailed guide on how to develop applications using the `static-dag` library, please refer to the [Usage Guide](USAGE.md).

## License

This project is licensed under the MIT License - see the LICENSE file for details.
