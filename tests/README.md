## USB++ Tests

USB++ is tested with the following test categories:

- compile time tests
- unit tests
- functional tests


### Compile Time Tests

| Directory  | tests/ct  |
| ---------- | --------- |
| Purpose |- Ensure descriptors compile in different environments<br/>- Ensure descriptors exhibit expected characteristcs  |
| Methods | - matrix workflow on github <br/>- `static_assert` for constexpr properties and functions |

### Unit Tests 

| Directory  | tests/ut  |
| ---------- | --------- |
| Purpose |- Ensure descriptors produce exected data |
| Methods |- `boost::ut`<br/>-Compare actual and expected data |

`boost:ut` requires C++20, so USB++ unit tests are compiled with `-std=c++20`

### Functional Tests 

| Directory  | tests/ft  |
| ---------- | --------- |
| Purpose |- Ensure descriptors produce data understood by other software |
| Methods |- TODO |

### Common Headers and Code

Common headers, source files and 3rd party libs are places in tests/common

