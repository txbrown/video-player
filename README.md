# Video Player App

A prototype app using [wxWidgets](https://github.com/wxWidgets/wxWidgets) as the GUI framework and showcasing how to render and manipulate graphics with [Skia](https://github.com/google/skia).

It has simple features:

- play / pause
- skip forward and backwards
- buffering state / loading state

TODO:

- add tests
- import video
- playback video
- ...more

## Build

To build this repository with you will need cmake and ninja.

```bash
brew install cmake ninja
```

In the root of the repository run:

```bash
mkdir build
cd build
cmake ..
```

This might take a while as Skia is big.

Once completed, in the build directory, run:

```bash
cmake --build . 
```

# Run

```bash
# inside build directory
./video-player
```
