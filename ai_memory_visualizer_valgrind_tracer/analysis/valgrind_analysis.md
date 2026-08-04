# Valgrind and AI Memory Tracer

## Scope

This analysis covers:

- `programs/heap_example.c`
- `programs/aliasing_example.c`

These are the programs assigned to the memory-tracer task.

The programs were compiled using the provided Makefile:

```text
-Wall -Wextra -Werror -pedantic -std=gnu89 -g
```

The `-g` option allows Valgrind to associate reports with source files,
functions, and line numbers.

The following commands were used:

```bash
cd programs
make clean
make

valgrind --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --num-callers=20 \
    ./heap_example

valgrind --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --num-callers=20 \
    ./aliasing_example
```

Exact process IDs, hexadecimal addresses, and some summary totals can vary
between machines and Valgrind versions. The error type, source line, block
size, and lifetime relationship are the important evidence.

---

# 1. heap_example.c

## Normal program behavior

The program creates two `Person` objects:

- Alice, age 30
- Bob, age 41

Each person consists of two separate heap allocations:

1. A `Person` structure.
2. A dynamically allocated copy of the person's name.

The conceptual heap state after both calls to `person_new()` is:

```text
STACK: main

alice ----------> H1
bob ------------> H3

HEAP

H1: Person structure
    name --------> H2
    age = 30

H2: "Alice\0"

H3: Person structure
    name --------> H4
    age = 41

H4: "Bob\0"
```

## Allocation history

For Alice:

```text
H1 = allocation for the Person structure
H2 = allocation of 6 bytes for "Alice\0"
```

For Bob:

```text
H3 = allocation for the Person structure
H4 = allocation of 4 bytes for "Bob\0"
```

The name allocations are created by:

```c
p->name = (char *)malloc(len + 1);
```

## Bob's deallocation

Bob is released correctly:

```c
free(bob->name);
free(bob);
```

This ends the lifetimes of both H4 and H3.

After these calls, `bob` still contains its old address, but it is a dangling
pointer and must not be dereferenced. The program does not use it again.

## Alice's partial deallocation

Alice is passed to:

```c
person_free_partial(alice);
```

That function performs only:

```c
free(p);
```

This releases H1, the `Person` structure, but does not release H2, the
separately allocated name.

Immediately before H1 is freed, the only application pointer to H2 is stored
inside:

```c
alice->name
```

After H1 is freed, the program no longer has a valid pointer through which it
can call:

```c
free(alice->name);
```

Attempting to obtain the name pointer from `alice` after that point would
itself require reading a freed structure.

## Valgrind classification

Valgrind reports a leak associated with the allocation in `person_new()`:

```text
6 bytes in 1 blocks are definitely lost
    malloc
    person_new (heap_example.c:20)
    main (heap_example.c:49)
```

The exact addresses and stack-trace formatting may differ, but the relevant
allocation is:

```c
p->name = (char *)malloc(len + 1);
```

For `"Alice"`:

```text
len = 5
len + 1 = 6 bytes
```

The extra byte stores the terminating null character.

## Why the leak is definitely lost

At program exit, no valid pointer to Alice's name allocation remains.

Valgrind therefore classifies the six-byte block as:

```text
definitely lost
```

This is a leak caused by lost ownership.

It is not merely `still reachable`, because there is no valid root pointer to
the allocation.

It is not `indirectly lost`, because the `Person` structure that previously
contained the pointer was explicitly freed. It is not itself a leaked parent
block.

## Errors not reported for this execution

The normal successful path of `heap_example` does not perform:

- An invalid read.
- An invalid write.
- A double-free.
- A use of uninitialized data.

The detected problem is the six-byte memory leak.

A clean result for the other categories means those errors were not observed
on the executed path. It does not prove that every possible allocation-failure
path is correct.

## Lifetime chain

```text
1. person_new("Alice", 30) allocates H1.
2. person_new allocates H2 for the copied name.
3. H1.name stores the only application pointer to H2.
4. main receives H1 through alice.
5. person_free_partial(alice) frees H1 only.
6. The pointer stored in H1.name becomes unavailable.
7. H2 remains allocated.
8. At exit, Valgrind finds no pointer to H2.
9. H2 is reported as definitely lost.
```

## Ownership conclusion

`Person` logically owns its `name` allocation. Therefore, its complete
destructor should release the owned object before releasing the structure:

```c
free(p->name);
free(p);
```

The task requires analysis rather than modification, so the original source
was not changed.

---

# 2. aliasing_example.c

## Normal program behavior

`make_numbers(5)` allocates an array containing:

```text
index:  0   1   2   3   4
value:  0  11  22  33  44
```

The allocation size is:

```text
5 * sizeof(int)
```

On the tested platform, where `sizeof(int)` is 4, this is a 20-byte heap
block.

After:

```c
a = make_numbers(n);
b = a;
```

the memory state is:

```text
STACK: main

a -----------+
             |
b -----------+------> H1

HEAP

H1: [0, 11, 22, 33, 44]
    size: 5 integers
    status: live
```

`b = a` copies the pointer value. It does not copy the array.

Therefore, `a` and `b` are aliases to the same heap allocation.

## Valid accesses before free

This statement is valid:

```c
printf("  a=%p b=%p a[2]=%d b[2]=%d\n",
       (void *)a, (void *)b, a[2], b[2]);
```

At this time H1 is still live, and both `a[2]` and `b[2]` refer to the same
initialized integer.

## Lifetime ending at free(a)

The program then executes:

```c
free(a);
```

This ends the lifetime of H1.

The resulting state is:

```text
STACK: main

a = dangling pointer
b = dangling pointer

HEAP

H1: freed allocation
    no live integer array exists here
```

`free(a)` acts on the allocation, not merely on the variable named `a`.

Because `b` refers to the same allocation, it becomes invalid at exactly the
same moment.

## Report 1: invalid read of b[2]

The first invalid access occurs at:

```c
printf("  reading b[2]=%d\n", b[2]);
```

Valgrind reports a diagnostic equivalent to:

```text
Invalid read of size 4
    at main (aliasing_example.c:40)

Address ... is 8 bytes inside a block of size 20 free'd
    free
    main (aliasing_example.c:36)

Block was alloc'd
    malloc
    make_numbers (aliasing_example.c:11)
    main (aliasing_example.c:28)
```

### Classification

```text
Invalid read
Use-after-free
```

### Memory object involved

H1, the five-element integer array allocated by `make_numbers()`.

### Why the offset is eight bytes

`b[2]` is equivalent to:

```c
*(b + 2)
```

With four-byte integers:

```text
2 * 4 = 8 bytes
```

The address is inside the old 20-byte allocation, but the allocation has
already been freed. Being within its former boundaries does not make the
access valid.

## Report 2: invalid write to b[3]

The next invalid operation is:

```c
b[3] = 1234;
```

Valgrind reports a diagnostic equivalent to:

```text
Invalid write of size 4
    at main (aliasing_example.c:42)

Address ... is 12 bytes inside a block of size 20 free'd
    free
    main (aliasing_example.c:36)

Block was alloc'd
    malloc
    make_numbers (aliasing_example.c:11)
    main (aliasing_example.c:28)
```

### Classification

```text
Invalid write
Use-after-free
```

### Why the offset is twelve bytes

For index 3:

```text
3 * sizeof(int)
3 * 4 = 12 bytes
```

The write targets storage whose lifetime ended at `free(a)`.

The allocator may reuse this memory for another allocation or for internal
metadata. Writing through `b` may therefore corrupt unrelated allocator or
program state.

## Report 3: invalid read of b[3]

The final invalid access is:

```c
printf("  wrote b[3]=%d\n", b[3]);
```

Valgrind reports another:

```text
Invalid read of size 4
    at main (aliasing_example.c:43)

Address ... is 12 bytes inside a block of size 20 free'd
```

### Classification

```text
Invalid read
Use-after-free
```

The fact that the previous invalid write stored `1234` does not restore the
object's lifetime. Reading the value back remains invalid.

## Leak result

The array allocation itself is freed once:

```c
free(a);
```

Therefore, the program is not expected to report a leak for H1.

`b` does not represent a second allocation and must not be freed separately.
Calling `free(b)` after `free(a)` would create a double-free.

## Uninitialized-memory result

No uninitialized-value warning is expected on this execution path.

Every array element is initialized by:

```c
for (i = 0; i < n; i++)
    arr[i] = i * 11;
```

The reported problems are lifetime violations, not initialization failures.

## Complete causal chain

```text
1. malloc creates the 20-byte allocation H1.
2. a receives H1's address.
3. b = a creates a second alias to H1.
4. free(a) ends H1's lifetime.
5. Both a and b become dangling pointers.
6. b[2] reads four bytes from freed memory.
7. b[3] writes four bytes to freed memory.
8. b[3] is read again from freed memory.
9. Valgrind reports two invalid reads and one invalid write.
```

---

# 3. Critical review of AI explanations

## Incorrect AI explanation: alias independence

An initial AI explanation stated:

> Only `a` becomes invalid because `a` was passed to `free()`. The pointer `b`
> remains valid until it is separately freed.

This explanation is incorrect.

`a` and `b` are separate pointer variables, but they identify the same heap
object. The lifetime belongs to the allocated object, not to one particular
pointer variable.

When H1 is freed through `a`, every alias to H1 becomes invalid for
dereferencing. Therefore, `b` becomes a dangling pointer immediately.

The correction was verified using:

- The assignment `b = a`.
- The matching addresses printed for `a` and `b`.
- The memory map from `analysis/memory_maps.md`.
- Valgrind's report that the address used through `b` belongs to a block
  previously freed through `a`.

## Partially incorrect AI explanation: leak category

Another AI explanation stated:

> Alice's name is indirectly lost because it was referenced by the leaked
> Person structure.

This is inaccurate because Alice's `Person` structure is not leaked. It is
explicitly freed by `person_free_partial()`.

After that structure is freed, no valid pointer remains to the name
allocation. The name block is therefore `definitely lost`, not indirectly
lost.

An indirectly lost block would still be referenced by another heap block that
was itself lost. That is not the final memory state in this program.

## Misleading AI explanation: successful output

A third claim stated:

> Because the program printed `1234` after writing through `b`, the pointer was
> still usable.

This conclusion is invalid.

Undefined behavior does not guarantee an immediate crash. Freed memory may
temporarily retain old bytes, but its former contents provide no evidence that
the access is legal.

Valgrind's allocation and deallocation history is stronger evidence than the
program's accidental output.

---

# 4. Issue summary

| Program | Source location | Valgrind category | Precise cause |
|---|---:|---|---|
| `heap_example` | `person_new`, line 20 | Definitely lost leak | Alice's six-byte name allocation is not freed before the owning structure is freed |
| `aliasing_example` | `main`, line 40 | Invalid read of size 4 | `b[2]` reads from the array after `free(a)` ended its lifetime |
| `aliasing_example` | `main`, line 42 | Invalid write of size 4 | `b[3]` writes into the freed array |
| `aliasing_example` | `main`, line 43 | Invalid read of size 4 | The program reads `b[3]` from the same freed allocation |
| Both tested programs | Executed paths | No uninitialized-value report | Values used on these paths were initialized before their valid use |

---

# 5. Conclusions

Valgrind messages correspond directly to concrete memory states.

In `heap_example`, the problem is an ownership failure. The program frees the
outer structure before releasing the separately allocated object that it owns,
causing the inner allocation to become definitely lost.

In `aliasing_example`, the problem is a lifetime failure. Two pointer
variables alias one heap object. Freeing that object through either alias ends
its lifetime for every alias.

The key lessons are:

1. Copying a pointer does not copy the pointed-to object.
2. `free()` ends the lifetime of an allocation, not merely one pointer.
3. Every alias becomes dangling when the shared allocation is freed.
4. Freeing a structure does not recursively free allocations referenced by
   its fields.
5. Output after undefined behavior is not reliable evidence.
6. Valgrind output must be mapped back to allocation, ownership, aliasing, and
   lifetime events in the source code.

## References

- Valgrind Memcheck manual:
  https://valgrind.org/docs/manual/mc-manual.html

- SEI CERT C, MEM30-C, Do not access freed memory:
  https://wiki.sei.cmu.edu/confluence/display/c/MEM30-C.+Do+not+access+freed+memory

- Programs bundle:
  https://github.com/hbtn-edu/ai_memory_visualizer_valgrind_tracer
