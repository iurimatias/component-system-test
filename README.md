# Component System Demo

A Qt-based application demonstrating dynamic component loading using plugins.

## Building

The build script supports building components separately:

```bash
# Build everything (main app and all plugins)
./build.sh all

# Build only the main application
./build.sh main

# Build only the counter plugin
./build.sh counter

# Build only the color changer plugin
./build.sh colorchanger
```

Each plugin will be built into the `build/plugins` and can be copied into `build/app/plugins`

## Running

To run the application, use:

```bash
./run.sh
```
