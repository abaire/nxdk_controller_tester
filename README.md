# ControllerTester

Basic XBOX utility to test game controller functionality.

## Building

1. Check out the [nxdk](https://github.com/XboxDev/nxdk) as a sibling to this directory.
1. make

## Editing with CLion

### Source level debugging

#### Creating a CLion build target:

1. Create a new `Embedded GDB Server` target
1. Set the Target to `all`
1. Set the Executable to `main.exe`
1. Set `Download executable` to `None`
1. Set `'target remote' args` to `127.0.0.1:1234`
1. Set `GDB Server` to the path to the xemu binary
1. Set `GDB Server args` to `-s -S` (the `-S` is optional and will cause xemu to wait for the debugger to connnect)

#### Enabling `.gdbinit`:

The .gdbinit file contains a few settings necessary to allow CLion to connect to xemu.
By default, CLion only parses the `.gdbinit` file in the user's home directory, see
https://www.jetbrains.com/help/clion/configuring-debugger-options.html#gdbinit-lldbinit
for instructions on enabling project-local `.gdbinit` parsing.


## Resources

The X360 font comes from https://fontmeme.com/xbox-360-font/
