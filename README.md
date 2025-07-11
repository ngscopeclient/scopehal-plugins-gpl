# scopehal-plugins-gpl
Plugins for ngscopeclient and libscopehal that depend on GPL libraries and thus can't be part of the main software distribution.

The plugin source code itself is generally under the same 3-clause BSD license as ngscopeclient (meaning you can use this code as an example for building your own non-GPL plugins), but once compiled and linked against the GPL libraries, the resulting binaries are covered by GPL.

This plugin does not currently have any CI or packaging flow so you need to install it manually. It only is tested on Linux since the GPIB driver is currently the only thing here and that depends on linux-gpib.

## Building

### Install dependencies

Install libscopehal (follow build instructions in https://github.com/ngscopeclient/scopehal-apps/)

Install linux-gpib (nominally optional, but the plugin is useless without it since right now GPIB is the only implemented component)

### Build the plugin itself

Replace SCOPEHAL_INCLUDE_DIR and SCOPEHAL_LIB_DIR with the appropriate paths on your system (there is currently no auto detection)

```
mkdir release-build
cd release-build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DSCOPEHAL_INCLUDE_DIR=/ceph/fast/home/azonenberg/code/scopehal-apps/lib/scopehal -DSCOPEHAL_LIB_DIR=/ceph/fast/home/azonenberg/code/scopehal-apps/release-build/lib/scopehal
make
```

### Install the plugin

Copy or symlink `libscopehal-plugins-gpl.so` to one of the following locations;

* `/usr/lib/scopehal/plugins`
* `/usr/local/lib/scopehal/plugins`
* `~/.scopehal/plugins`
* ngscopeclient binary directory (if not under `/usr`)
