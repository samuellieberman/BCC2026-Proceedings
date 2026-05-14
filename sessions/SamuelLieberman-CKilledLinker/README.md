# C Without a Build System or: How I Killed the Linker

## Samuel Lieberman

### Boise Code Camp 2026 - May 2nd, 2026

When a project grows it becomes more and more important to have a build system. However C actually has a surprising number of tricks for growing projects without a build system. This talk will cover Jumbo builds, preprocessor #pragmas, explicit runtime linking, and more, all with the goal of writing a native cross platform GUI program in C.

---

> [!WARNING]
> I DO NOT CLAIM TO OWN ALL OF THE WORK IN THIS FOLDER
> PLEASE RESPECT THE WISHES OF THE CREATORS

### The recording of the talk:
 * https://youtu.be/6IwXGkWK134

### My current test project based on these ideas:
 * https://github.com/samuellieberman/NAppGUI-Cross-Compiler

### Information on Winelib:
 * License: Winelib is distributed under the GNU Lesser General Public License.
 * [Winelib User's Guide](https://gitlab.winehq.org/wine/wine/-/wikis/Winelib-User's-Guide)
 * Installing:
```bash
sudo apt install libwine-dev wine64-tools
sudo apt install mingw-w64-common
```


### Information on LLVM's libc++:
 * License: The LLVM Project is under the Apache License v2.0 with LLVM Exceptions
 * Source: libcxx-21.1.4.src.tar.xz from https://github.com/llvm/llvm-project/releases
 * Installing:
```bash
sudo apt install libc++-14-dev
```

### Other
I have also copied and modified code from several other sources!

If you wish to modify and/or redistribute the code I wrote wholely on my own, you at least have my permission, but I would prefer you to look at my test project instead, [NAppGUI Cross Compiler](https://github.com/samuellieberman/NAppGUI-Cross-Compiler). The code I wrote in here is hastily patched together with a lot of manual hacks.

---

## Speaker: Samuel Lieberman

![Samuel Lieberman](Samuel_Lieberman.webp)

I program for fun. All my projects are thinly veiled excuses to learn more about software.