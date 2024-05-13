### NEONSHIFT ASCII

**ASCII Painter for the terminal / Speedpainting brain interface**\
**Prototype No1**

## Supports 2 colors!

### usage:

```
make
./neonshift
```

### key bindings:

**movement**\
**g** : vertical sweeping\
**b** : reverse sweeping\
**-/+** : adjust sweeping speed\
**spacebar** : pause\
**hjkl** : move

**painting**\
**z** : paint\
**c** : switch color\
**;** : stroke/spot\
**p** : invert

**functional**\
**q** : quit\
**s** : save\
/!\ Neonshift prototype saves to the 'painting' file only. If you want to keep your stuff, you need to copy that file.\
If the 'painting' file exists upon launch, it is loaded.

### notes:

- Make sure you report any issues you encounter with compiling and running the program
- Please send your best pieces in order to pin them here (no nsfw)

### demo:

![demo ascii](https://raw.githubusercontent.com/hllcngn/neonshift/main/demo_ascii.png)

### known issues:

- It doesn't actually use the Shift key.
- hjkl movement isn't desynchronized from sweeping which makes it hard to edit a painting at low fps. (will be fixed in prototype no2)

by d0pelrh
