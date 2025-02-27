<div align="center">
<img src="assets/img.png" alt="icon" height=300 />
<h1>Cheeseburger</h1>
</div>


A rewrite of the popular utility [*LOLCAT*](https://github.com/busyloop/lolcat) in C++. The rationale is that lolcat is
often used in init scripts and such. It can at times be rather time-consuming to run it, since it's written in Ruby. Now
it's *BLAZINGLY FAST* in C++

## Building

You'll need *cmake*, boosts *program_options*, and a C++ 20 compatible compiler. Thereafter, just run

```bash
cmake -S . -B build && cmake --build build --parallel 8
```

You'll now have the executable in `build/cheese`

## Acknowledgement

Both the project and the codebase are heavily inspired by the [*LOLCAT*](https://github.com/busyloop/lolcat) project. 
