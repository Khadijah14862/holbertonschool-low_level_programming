# AI Memory Visualizer

## Scope

This analysis covers the following programs:

- `programs/stack_example.c`
- `programs/aliasing_example.c`
- `programs/heap_example.c` as an additional heap-lifetime example

`crash_example.c` is not included because it is intended for the separate
crash-report task.

The programs were compiled with the provided Makefile and executed normally,
without Valgrind.

Commands used:

    cd programs
    make
    ./stack_example
    ./aliasing_example
    ./heap_example

The hexadecimal addresses printed by the programs are process-specific and may
change between runs. The analysis therefore uses symbolic names such as H1,
H2, and S1. Address equality and pointer relationships are more important than
the exact numeric addresses.

---

# 1. stack_example.c

## Program purpose

This program uses recursion to demonstrate that every active function call has
its own stack frame. It also calls `dump_frame()` twice for each recursive
depth, once before recursion and once while the recursion is unwinding.

No heap memory is allocated by this program.

## Important objects

### main frame

`main()` has an active stack frame from program entry until it returns. It
calls:

    walk_stack(0, 3)

### walk_stack frame

Each invocation of `walk_stack()` has separate parameter and local objects:

- `depth`
- `max_depth`
- `marker`

The value of `marker` is:

    depth 0: marker = 0
    depth 1: marker = 10
    depth 2: marker = 20
    depth 3: marker = 30

Each `marker` exists from entry into its particular `walk_stack()` invocation
until that invocation returns.

### dump_frame frame

Each invocation of `dump_frame()` creates new automatic objects:

- `label`
- `depth`
- `local_int`
- `local_buf[16]`
- `p_local`

Within one call:

    local_int = 100 + depth
    p_local = &local_int

Therefore, `p_local` aliases `local_int`. It does not point to a separate
integer.

`local_buf` is also stored in the active `dump_frame()` stack frame. Only its
first two bytes are initialized:

    local_buf[0] = 'A' + depth
    local_buf[1] = '\0'

The remaining bytes are not used.

## Memory map: first call at depth 0

While `dump_frame("enter", 0)` is active:

    STACK

    +--------------------------------------+
    | dump_frame frame                     |
    | label ----> "enter" string literal   |
    | depth = 0                            |
    | local_int = 100                      |
    | p_local ----> local_int              |
    | local_buf = "A"                      |
    +--------------------------------------+
    | walk_stack frame, depth 0            |
    | depth = 0                            |
    | max_depth = 3                        |
    | marker = 0                           |
    +--------------------------------------+
    | main frame                           |
    +--------------------------------------+

    HEAP

    No allocations.

The string literal `"enter"` has static storage duration. It is not contained
inside the `dump_frame()` stack frame. The local pointer parameter `label`
points to that literal.

When `dump_frame()` returns, its `local_int`, `local_buf`, `p_local`, `label`,
and parameter `depth` reach the end of their lifetimes. The `marker` belonging
to `walk_stack(0, 3)` remains alive.

## Memory map: maximum recursive depth

At the deepest point, four `walk_stack()` frames are active simultaneously:

    STACK, newest frame first

    +--------------------------------------+
    | walk_stack depth 3                   |
    | marker = 30                          |
    +--------------------------------------+
    | walk_stack depth 2                   |
    | marker = 20                          |
    +--------------------------------------+
    | walk_stack depth 1                   |
    | marker = 10                          |
    +--------------------------------------+
    | walk_stack depth 0                   |
    | marker = 0                           |
    +--------------------------------------+
    | main                                 |
    +--------------------------------------+

When `dump_frame("enter", 3)` is executing, its temporary frame is placed above
the depth-3 `walk_stack()` frame:

    +--------------------------------------+
    | dump_frame                           |
    | local_int = 103                      |
    | p_local ----> local_int              |
    | local_buf = "D"                      |
    +--------------------------------------+
    | walk_stack depth 3, marker = 30      |
    +--------------------------------------+
    | walk_stack depth 2, marker = 20      |
    +--------------------------------------+
    | walk_stack depth 1, marker = 10      |
    +--------------------------------------+
    | walk_stack depth 0, marker = 0       |
    +--------------------------------------+
    | main                                 |
    +--------------------------------------+

After this `dump_frame()` call returns, its frame is destroyed. The four
`walk_stack()` frames remain active until recursion starts unwinding.

## Unwinding

Depth 3 returns first, followed by depths 2, 1, and 0.

Before each `walk_stack()` call returns, it calls:

    dump_frame("exit", depth)

This creates a new `dump_frame()` invocation. It is not a continuation of the
earlier `"enter"` invocation.

The implementation may reuse the same stack addresses for the `"enter"` and
`"exit"` calls because the first set of local objects is already dead. Address
reuse does not extend an object's lifetime.

## Validation against normal output

The output shows that:

- `p_local` and `&local_int` have the same address during each call.
- Every active recursive level has a different `marker` address.
- The same address may appear again in a later `dump_frame()` call.
- The recursion reaches depth 3 and then unwinds in reverse order.

On the tested system, deeper frames may appear at lower numeric addresses.
This is an implementation observation, not a portable rule guaranteed by C.

---

# 2. aliasing_example.c

## Program purpose

This program allocates an integer array on the heap, creates two pointers to
the same allocation, frees the allocation through one pointer, and then uses
the other pointer.

The final accesses are use-after-free errors.

## Stage A: entry into main

    STACK

    +----------------------------+
    | main                       |
    | a = NULL                   |
    | b = NULL                   |
    | n = 5                      |
    +----------------------------+

    HEAP

    Empty.

The pointer variables `a` and `b` are stack objects. A pointer variable can be
on the stack while the object it later points to is on the heap.

## Stage B: make_numbers(n)

`make_numbers(5)` creates its own stack frame:

    STACK

    +----------------------------+
    | make_numbers               |
    | n = 5                      |
    | i                          |
    | arr = NULL, then H1        |
    +----------------------------+
    | main                       |
    | a = NULL                   |
    | b = NULL                   |
    | n = 5                      |
    +----------------------------+

The call:

    malloc(5 * sizeof(int))

creates heap allocation H1.

After initialization:

    HEAP

    H1: live allocation containing five integers

    index:   0    1    2    3    4
    value:   0   11   22   33   44

`arr` points to the first integer in H1.

When `make_numbers()` returns, its local variables `arr`, `i`, and parameter
`n` cease to exist. The heap allocation does not disappear when the function
returns. Its address is returned to `main`, and ownership is transferred to
`a`.

## Stage C: after a = make_numbers(n)

    STACK

    +----------------------------+
    | main                       |
    | a -----------+             |
    | b = NULL     |             |
    | n = 5        |             |
    +--------------|-------------+
                   |
                   v
    HEAP          H1: [0, 11, 22, 33, 44]
                  status: live

At this point, `a` is the only pointer in `main` that refers to H1. It is the
logical owner responsible for eventually freeing the allocation.

## Stage D: after b = a

The assignment copies the pointer value, not the integer array.

    STACK

    +----------------------------+
    | main                       |
    | a -----------+             |
    | b -----------+-------------+
    | n = 5        |             |
    +--------------|-------------+
                   |
                   v
    HEAP          H1: [0, 11, 22, 33, 44]
                  status: live

`a` and `b` are aliases. They point to exactly the same allocation.

Therefore:

    a[2] == b[2] == 22

There are not two arrays, and assigning `b = a` does not duplicate ownership.
For this analysis, `a` remains the owner and `b` is a non-owning alias.

## Stage E: after free(a)

The call:

    free(a)

ends the lifetime of H1.

    STACK

    +----------------------------+
    | main                       |
    | a: invalid/dangling value  |
    | b: invalid/dangling value  |
    | n = 5                      |
    +----------------------------+

    HEAP

    H1: deallocated
    No live integer array exists at this address.

Freeing the allocation through `a` invalidates all pointers into that
allocation, including `b`. The numeric bits printed for `b` may still look like
the old address on a particular machine, but they no longer identify a live
object that the program may access.

The allocator may reuse the block, place bookkeeping data in it, or return it
to the operating system.

## Stage F: invalid accesses

The expression:

    b[2]

is equivalent to:

    *(b + 2)

It attempts to read an integer from an allocation whose lifetime has ended.
This is an invalid read and undefined behavior.

The statement:

    b[3] = 1234;

attempts to write into the same freed allocation. This is an invalid write and
undefined behavior.

The later expression:

    b[3]

is another invalid read.

The program may:

- print an old-looking value,
- print a different value,
- crash,
- appear to work,
- or behave differently after recompilation.

A successful-looking run does not make these accesses valid.

## Ownership conclusion

Before `free(a)`:

- H1 has one allocation.
- `a` is the logical owner.
- `b` is an alias.

After `free(a)`:

- H1 no longer exists as a live C object.
- Neither `a` nor `b` may be dereferenced.
- Setting only `a = NULL` would not repair `b`.
- No second `free(b)` should be performed because H1 was already freed.

---

# 3. heap_example.c

## Program purpose

This program creates two `Person` objects. Each person requires two independent
heap allocations:

1. one allocation for the `Person` structure;
2. one allocation for the copied name.

The program correctly releases Bob's allocations but releases only Alice's
structure, leaking Alice's name.

## Static-storage objects

The arguments `"Alice"` and `"Bob"` are string literals. They have static
storage duration.

The parameter `name` in `person_new()` points to one of those literals while
the function copies its characters into a new heap allocation.

## Creating Alice

The first call:

    alice = person_new("Alice", 30);

creates:

    H1: Person structure
        H1.name ----> H2
        H1.age = 30

    H2: character array
        ['A', 'l', 'i', 'c', 'e', '\0']

During `person_new()`, the local pointer `p` is stored in its stack frame and
points to H1. The fields of H1 are stored on the heap.

When `person_new()` returns, local variables `p`, `i`, and `len` cease to
exist. H1 and H2 remain alive, and ownership is transferred to `alice`.

## Creating Bob

The second call creates two more allocations:

    H3: Person structure
        H3.name ----> H4
        H3.age = 41

    H4: character array
        ['B', 'o', 'b', '\0']

After both calls:

    STACK

    +--------------------------------+
    | main                           |
    | alice -----------> H1          |
    | bob -------------> H3          |
    +--------------------------------+

    HEAP

    H1: Person { name -> H2, age = 30 }
    H2: "Alice"

    H3: Person { name -> H4, age = 41 }
    H4: "Bob"

The structure and its name have different addresses because they are separate
allocations.

## Releasing Bob

The program executes:

    free(bob->name);
    free(bob);

First H4 is freed. Then H3 is freed.

Afterward, Bob has no live heap allocations. The stack pointer variable `bob`
still exists until `main()` returns, but it must not be dereferenced.

## Partially releasing Alice

The function:

    person_free_partial(alice);

executes only:

    free(p);

This frees H1, the `Person` structure. It does not automatically free H2.

Before freeing H1, its `name` field was the only stored pointer to H2.
After H1 is freed, the program has no remaining pointer that can be used to
free H2.

Final heap state:

    H1: freed
    H2: still allocated and unreachable
    H3: freed
    H4: freed

H2 contains six bytes for `"Alice"` including the terminating null character.
It is a memory leak.

A correct destructor would preserve the inner pointer long enough to free it:

    free(p->name);
    free(p);

This observation also applies to some allocation-failure cleanup paths that
call `person_free_partial()` on a fully created person.

---

# 4. Critical review of AI output

## Incorrect AI claim 1

Initial AI claim:

> The `local_int` shown during `"enter"` and `"exit"` at the same depth is the
> same variable because it has the same address.

### Why this is incorrect

The two values belong to two separate calls to `dump_frame()`.

The first `local_int` reaches the end of its lifetime when the `"enter"`
`dump_frame()` call returns. Later, the `"exit"` call creates a new
`local_int`.

The implementation is allowed to reuse the same stack storage because the
first object is no longer alive. Equal addresses at different times do not
prove that two objects have the same lifetime or identity.

### Corrected explanation

Each invocation of `dump_frame()` creates a distinct set of automatic
variables. The stack slot may be reused between non-overlapping calls.

## Incorrect AI claim 2

Initial AI claim:

> `free(a)` invalidates `a`, but `b` remains valid because it was not passed to
> `free()`.

### Why this is incorrect

`a` and `b` contain aliases to the same allocation H1. `free()` operates on the
allocated object, not on the pointer variable itself.

When H1's lifetime ends, every pointer into H1 becomes invalid for access.
Therefore, both `b[2]` and `b[3]` are use-after-free operations.

### Corrected explanation

Pointer variables are separate stack objects, but the heap object they
reference is shared. Deallocating the shared object invalidates all aliases.

## Incomplete AI claim 3

Initial AI claim:

> Freeing `alice` frees the complete Person.

### Why this is misleading

`free(alice)` releases only the allocation beginning at `alice`. C does not
recursively inspect pointer fields and free the objects to which they point.

Alice's `name` is stored in a separate allocation and must be freed explicitly.

### Corrected explanation

A composite heap object may own several allocations. Each allocation requires
a corresponding `free()` according to the ownership design.

---

# 5. Final conclusions

1. Stack objects belong to individual function invocations and remain valid
   only while those invocations are active.

2. Recursive calls create separate simultaneous stack frames.

3. Reusing a stack address does not restore or extend the lifetime of an older
   object.

4. Heap allocations remain alive after the allocating function returns, as
   long as they have not been freed.

5. Assigning one pointer to another creates an alias, not a copy of the pointed
   data.

6. `free()` ends the lifetime of the allocation for every alias.

7. A structure containing a pointer may own another allocation. Freeing the
   structure does not recursively free that allocation.

8. Output produced after undefined behavior cannot be used as proof that the
   memory access was valid.

## References

- Valgrind User Manual:
  https://valgrind.org/docs/manual/manual.html

- SEI CERT C, MEM30-C, Do not access freed memory:
  https://wiki.sei.cmu.edu/confluence/display/c/MEM30-C.%2BDo%2Bnot%2Baccess%2Bfreed%2Bmemory

- Program bundle:
  https://github.com/hbtn-edu/ai_memory_visualizer_valgrind_tracer
