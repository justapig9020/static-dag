# Usage Guide for static-dag

## Introduction

The `static-dag` library is designed to facilitate the creation and management of Directed Acyclic Graphs (DAGs) in embedded environments. In this system, the smallest unit of management is the `DAGNode`. Users can embed `DAGNode` into any struct, making that struct a manageable node within the DAG.

This guide will walk you through the process of defining, creating, and managing nodes using the `static-dag` librar.

## Declaring a Struct with DAGNode

The library provides a `DAGNode` macro to assist users in embedding a `DAGNode` into any struct. This allows the struct to become a node within the DAG. The macro usage is as follows:

Place `DAGNode(name, in_degree);` within the structure definition to create a `DAGNode` member named `name` with a maximum in-degree of `in_degree`.

### Example

```c
struct Foo {
    char *bar;
    DAGNode(node, 3);
};

```

In this example, we declare a struct Foo that contains a DAGNode named node with an in-degree of 3. This means that the node can have up to 3 incoming edges in the DAG.

## Initializing a Node

The library provides a macro `DAGNodeInit` to initialize a node. It is crucial to initialize a node before using it in your DAG operations. The macro sets up the internal state of the node, allowing it to function correctly within the DAG.

### Usage

`DAGNodeInit(node, node_op)`

- `node`: The node to initialize (this corresponds to the `node` member in your custom structure).
- `node_op`: A function table that defines the operations for this node. The functions that can be registered in `node_op` will be introduced in the following sections.

## Adding relationships

To define the relationships between nodes in the DAG, you can use the `add_ancestor` function. This function adds an ancestor to the specified node, allowing you to build the directed connections within the graph.

### Usage

`add_ancestor(node, ancestor)`

- `node`: Pointer to the target node to which you want to add an ancestor.
- `ancestor`: Pointer to the node that you want to set as an ancestor of the target node.

## Removing a Node

To remove a node from the DAG, use the `remove_node` function. This function detaches the specified node from the graph, provided that it has no ancestors. If the node still has ancestors, it cannot be removed.

When a node is removed, its dependent relationships with its descendants are also removed, effectively isolating those nodes from the removed node.

### Usage

`remove_node(my_node)`

- `node`: Pointer to the node you want to remove from the DAG.

## `DAGop` Structure

The `DAGop` structure defines a set of operations (callbacks) that can be associated with a `DAGNode`. These operations allow you to customize how nodes behave in various situations, such as when they are freed, printed, or when all their ancestors have been removed. The `DAGop` structure is passed to `DAGNodeInit` during the initialization of a node.

### Available Operations

1. **`free`**: A callback function that is called when a node is freed. It can be used to clean up resources associated with the node.

   - **Signature**: `void (*free)(struct DAGNode *self);`
   - **Example Usage**: This function can be customized to release memory or reset the state of the node.

2. **`print`**: A callback function to print the node's details. This is useful for debugging or visualizing the state of the DAG.

   - **Signature**: `void (*print)(struct DAGNode *self);`
   - **Example Usage**: Define this function to output information about the node's current state, such as its ID or its relationships.

3. **`on_all_ancestors_freed`**: A callback function that is called when all ancestors of the node have been removed. This function allows you to define custom behavior for when a node becomes independent.
   - **Signature**: `void (*on_all_ancestors_freed)(struct DAGNode *self);`
   - **Example Usage**: This can be used to perform specific actions when the node no longer has any dependencies in the graph.

When defining a `DAGNode`, you will typically provide a `DAGop` structure with your custom implementations for these operations, allowing you to tailor the node's behavior to the needs of your application.
