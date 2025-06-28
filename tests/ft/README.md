## USB++ Functional Tests

Functional tests are executed with a custom build `lsusb` utility, statically linked with `libusb` and a custom `libusb`'s back-end.
The custom back-end returns statical `usbplusplus::ft::usbdevice` instances, fitted with the USB desctiptors under test.
The output of the tests is visually verified and added to the master data set for the purpose of comparison.
In a CI build the tests are build and run, their output is then compared against the master data.

As `lsusb` is not requesting string descriptors, an additional executable is built: `build/ftls` which lists the devices 
with strings _Manufacturer_ , _Product_ and _Serial Number_ .

`build/ftls` also facilitates descriptor data dumping, when `--dump` agrument is given 

### Adding a new functional test

1. Create a C++ source file (*.cpp) in `tests/ft`
2. Define USB descriptors to be tested
3. Instantiate `usbplusplus::ft::usbdevice` with those descriptors
4. Build the tests with `make`
5. Run `./build/ft` and identify the newly added USB device in the output
6. Run `./build/ft -v -s <bus>:<device>` and visually verify the output
   * `<bus>` and `<device>` are integer number assigned to the device
7. Save output to the `data` directory `./build/ft -v -s <bus>:<device> > data/<bus>:<device>`


