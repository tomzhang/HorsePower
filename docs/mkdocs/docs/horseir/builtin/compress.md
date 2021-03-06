# compress

### Description

```no-highlight
@compress(boolean_vector, values)
```

- select from values based on the boolean vector (true values)
- the boolean vector and values have the same length

### Type Rules

High-level

```no-highlight
bool, Basic -> MaxType
_   , _     -> domain error
```

Table with details (See [type alias](../../../horseir/#types))

![compression](../types/compress.png)

### Shape Rules

[Shape right rules](../../../horseir/#shape-right)

### Examples

```no-highlight
    @compress((1,1,0):bool, (53,11,12):i32)
(53,11):i32
```

