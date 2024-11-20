# Introduction

This report details the implementation of an image processing application using both multithreaded and single-threaded approaches. 

# Approach

In order to show `single-thread` and `multi-thread` operation on image dynamically for user, I need to build simple UI that shows every state of image in UI.

I used `C++` `Qt` desktop application builder.

In this directory, there is `app` and `src` folders. In order to see codes and how it is implemented, you can see `src` section.
Every single `cpp`, `.h` and `.ui` files are there. 

It can be built onto `executable` format, but for building, you should have `qmake`. You can download here if you want to build.
![Link](https://www.qt.io/offline-installers)

There will be app called `Qt Creator` after installation. In `src` section, you need to open `.pro` file. After changing build location file, everything will be bundled into `executable` format.


# How to process executable

For convenience reasons, I built application and it resides on `app` folder. It has source `.dll` and `.o` files to help it run without any errors.

This app is `cross platform app`, so it can be run on `Window/Linux/MacOS`. 

In order to run on `Windows`, there isn't any problem, just call as the following sample

- .\App-Image-v3.exe "C:\Users\TaharMasmaliyev\Downloads\MainBefore.jpg" 50 M

In `Linux/MacOs`, `wine` needs to be installed in order to run `executable`.

To install `wine`, here are the steps:

For `Linux` :
- sudo dpkg --add-architecture i386
- sudo apt install wine64 wine32

For `MacOs` :
- brew install --cask xquartz
- brew install --cask wine-stable

In these platforms, you need to provide input like this:
- wine .\App-Image-v3.exe ./MainBefore.jpg 50 M

# Implementation

Since `UI` also run on one thread, i did all computations on another `thread` in order not to let UI freeze when displaying image.

In `Single-Threaded` version, I added 1 thread to run all the computation and emit each iteration to main UI thread to display how the image process goes.

In `Multi-Threaded` version, I added thread pool for managing all thread. Since computers are limited with threads, I limit the thread count to `core count` in single PC, with this way, there won't be overhead.

For more details, you can check the code implementation. I well commented on every process.

After all process done, the resulting image is saved on `Current Directory` as `result.jpg`.
Another detail is that, when passed image size that is bigger than the monitor window size, then image is rescaled to max window size in order to display perfectly.