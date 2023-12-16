# ANSLL
Another Not So Low Level Language compiler.

The ANSLL compiler and language are works in progress.

It is not currently in a working state.

## Build

```shell
make
```

## Compile ANSLL

Compile files by passing a directory containing `*.ansll` files to the
compiler. This example compiles the `stdlib` folder.
```shell
./bin/ansllc ./stdlib
```

You can get verbose output with the `-v` flag:
```shell
./bin/ansllc -v ./stdlib
```

***Currently object files are produced but they are empty.***

## Idea

The ANSLL language enforces 'units', similar to types.
The language is meant to be zero-overhead and work
well alongside C, however enforcing units and being good
for math work. The standard libraries would be extensive with
calculations and algorithms.

Sample code (in the future) might look like this:
```ansll

module sample::mathstuff;

import std::units;
import std::trig;

# A standalone unit (e.g. seconds, meters, etc.)
unitdef Chairs;
# A unit that is defined by another (e.g. kilometers, inches, watts, etc.)
unitdef SqChairs Chairs ^^ 2;

# Variables are implicitly 64-bit "doubles" signed floating-point numbers
# But they can be explicitly stated written as if they were a unit
fn calculate($freq Hz) Hz ft
{
    # Order of operations, 12 squared then multiply by freq then add 3
    $somethin := 3 Hz ft + $freq * 12 ft ^^ 2;

    # This would give an error because units don't match
    # you can also state a unit for the local variable explicitly (optional)
    #        > Error cannot add values of unit 'V' and 'V A'
    # $wattage W := -2.7 V + 12 V * 15 A;

    # If a unit has a direct conversion it can be converted like this:
    $meters m := <m>(27 ft);

    return $somethin;
}

```

The language should eventually have a separate standard and compiler,
just as C and C++ do.

