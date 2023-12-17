# Unit

A unit should have a name of theoretically up to any length and be
UTF-8 encoded. A unit should have "dependencies" that are the units
it depends upon.

For instance, the unit `km` kilometers would depend on `m` meters.
The unit `W` watts depends on `V` volts and `A` amperes, so it has two
dependencies. A unit like `s` seconds is a base unit and has zero dependencies.

If a unit has only 1 dependency, it must implement a function
converting a value of this unit to a value of the dependency unit.

If a unit has more than 1 dependencies, it must not implement conversion
functions since it may not be possible to do so (e.g. `mi / h` or `s * m ^^ 2`
since the powers do not match).

