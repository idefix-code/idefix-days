# Programming in Idefix: Data structures

<!-- toc -->

- [Scope](#scope)
- [CPU VS GPU: essentials](#cpu-vs-gpu-essentials)
- [Device / Host dichotomy](#device--host-dichotomy)
- [Arrays: `IdefixArrayND`](#arrays-idefixarraynd)
- [Loops and kernels: `idefix_for`](#loops-and-kernels-idefix_for)
  * [Important notes on kernel GPU portability](#important-notes-on-kernel-gpu-portability)

<!-- tocstop -->

## Scope
This session will focus on `idefix` idioms that are essential to make the code
*portable* (ensuring it builds and runs on many architectures, including CPUs and
GPUs). We will cover `IdefixArrayND<T>`, `idefix_for` and `idefix_reduce`. Backstage,
all these structures are powered by `Kokkos`, but it is rarely necessary to use `Kokkos`
directly to program in `idefix`.

As a preamble, we'll start by reviewing the fundamental differences between
CPU and GPU.

## CPU VS GPU: essentials

- CPUs execute instructions sequentially -> loops are sequential
- GPUs run *the same instruction* many times in parallel -> *kernels* are
  threaded (where "many" = 32, typically)

CPUs are much more versatile, but GPUs are much faster for the few tasks they are able
to perform.

> **Pop quizz !**
>
> choose your weapon

<details> <summary> CPU </summary>
<img src="img/sword.jpg" alt="sword" width="300">
</details>

<details> <summary> GPU </summary>
<img src="img/rake.jpg" alt="rake" width="300">
</details>

To keep the code portable, we'll need to write our *kernels* in a way that can
be compiled as sequential loops when targeting CPUs.

We'll see later how to achieve this with `idefix_for`.


## Device / Host dichotomy

Divide and conquer: GPUs are very efficient and fast *at certain tasks*, but there are
still a variety of things our program needs to do that are better left to CPUs, like
input/output (IO), calling functions. As we'll see, writing GPU-capable code also
adds complexity.

In order to achieve maximal performance *and* minimal complexity, we divide the code
we write in two parts:
- intensive loops are written as GPU kernels
- the overarching execution flow, as well as certain operations (like IO), are still
  written as pure C++.

**ILLUSTRATION HERE**

We call *host* the physical part of the machine that run the execution flow (always a
CPU), and we call *device* the part that executes intensive loops (which may be a GPU
*or* a CPU).

It is important to understand that host memory and device memory are separate: data that
exists in *host space* cannot directly be accessed from *device space*, and vice-versa.



## Arrays: `IdefixArrayND<T>`

Idefix arrays (aliases for `Kokkos::View`) are idefix's base container types. They
represent data **in device space**.

They come in 4 different dimensionalities
- `IdefixArray1D<T>`
- `IdefixArray2D<T>`
- `IdefixArray3D<T>`
- `IdefixArray4D<T>`

Where `T` is a placeholder typename for the contained data type (Idefix arrays are `C++`
*class templates*).

Note that `idefix.hpp`, which is included everywhere throughout idefix's code base,
defines a `real` type alias, which can represent `double` or `float` (single precision),
and is known at compile-time. So in practice we'll mostly use `IdefixArrayND<real>`.

Let's see how a 3D array of `double` is declared. First, in pure `C`
```cpp
const Nx = 16;
const Ny = 16;
const Nz = 16;
double[Nz, Ny, Nz] arr;
```

The equivalent `IdefixArray` would be
```cpp
#include "idefix.hpp"

const Nx = 16;
const Ny = 16;
const Nz = 16;
IdefixArray3D<double> arr(
    "my array", // array label
    Nz, Ny, Nx // array dimensions
);
```

> Note 1
>
> Arrays have *labels* (here, "my array")
> which are important when debugging (they are used in tracebacks), so make sure to
> make them *unique*.

> Note 2
>
> While C-arrays are not initialized on allocation, IdefixArrays are filled with 0 on
> allocation (unless specified otherwise)

> Note 3
>
> `IdefixHostArrayND<T>` are host-space arrays, they are used to *mirror* data in device
> space. Host<->Device synchronisation is only performed when needed.


## Loops and kernels: `idefix_for`

As an example, let's see how we would fill a 100-element 1D array with `1.0`.

First, in pure
`C++`:
```cpp
const int N = 256;
double[N] data;
for(int i=0, i<N; i++) {
  data[i] = 1.0;
}
```

Now using `idefix_for`:
```cpp
#include "idefix.hpp"

const int N = 256;
IdefixArray1D<real> arr(
  "my array", // array label
  N // array dimensions
);
idefix_for(
  "fill arr", // kernel label
  0, N, // kernel bounds (0 <= i < N)
  KOKKOS_LAMBDA(int i) {
    arr(i) = 1.0;
  }
);
```

> Just like arrays, kernels have labels, which are equally important for debugging.

> Note that C-array elements are accessed with `[]` while IdefixArray elements
> are accessed with `()`

> Memory allocation is very expensive. Most of the time you'll be processing
> `IdefixArray`s that are *already* allocated.

How about a 2D version of this ?
```cpp
#include "idefix.hpp"

const int Nx = 16;
const int Ny = 16;

IdefixArray2D<real> arr(
  "my array", // array label
  Ny, Nx // array dimensions
);
idefix_for(
  "fill arr", // kernel label
  0, Ny, // kernel j bounds (0 <= j < Ny)
  0, Nx, // kernel i bounds (0 <= i < Nx)
  KOKKOS_LAMBDA(int j, int i) {
    arr(j, i) = 1.0;
  }
);
```

> important: in `ND` (`N`>1), the contiguous index is the last one (here `i`)
> IdefixArrays use *Fortran-like* layout.

Generalizing to 3 and 4D is left as an exercise to the reader.


### Important notes on kernel GPU portability


> `this->` cannot be captured in a kernel (although it builds fine on CPU targets !)

> `if/else` (branching) is very expensive
