# Trusted Timestamp Demo

## Overview
A detailed description of the Trusted Timestamp demo is provided
[here](https://github.com/GaloisInc/pirate-annotations/blob/master/spec/timestamp_demo.rst).
The purpose of this README is to document implementation, testing, and execution
of the Trusted Timestamp demo components.

## Building
All demo components are built by following instructions provided
[in the main project README](https://github.com/GaloisInc/pirate-demos/blob/master/README.md).
* The build process generates PKI components, which include creation of two
  private/public key pairs. On a machine with limited resources this step may
  take a noticeable amount of time.
* To build time demo with serial GAPS channels between the proxy and the signing
  service, use ```cmake .. -DGAPS_SERIAL=ON```
* All build artifacts are located in ```build/time_demo``` directory

### Dependencies
* [libpirate](https://github.com/GaloisInc/pirate-demos/tree/master/libpirate)
* [OpenSSL](https://www.openssl.org/)
* [Valgrind](https://valgrind.org/)

## Implementation
All three demo components are based on common code for configuring and managing
GAPS channels as well as work thread creation and termination. Sections of code
for GAPS channel management will be replaced with code generated by the
compiler when GAPS source annotations became available.

Timestamp cryptography code is based on OpenSSL's support for
[RFC3161](https://tools.ietf.org/html/rfc3161) Time-Stamp Protocol (TSP) and is
located in the ```ts_crypto.c``` source file. All data structures for storing
requests and responses are designed to be agnostic to cryptographic algorithms
behind the implementation.

### Verbosity
By default all demo components run in a quiet mode. Use ```-v``` to increase
output verbosity.
- ```-v``` displays event messages
- ```-v``` or ```-vv``` displays event messages and parsed request/response
 structure components. __NOTE__ this level of verbosity is mainly used for
 debugging.

### Usage
All application binaries support help and usage options:
- ```<application binary> --help```
- ```<application binary> --usage```

#### Planner (Client)
```
Usage: planner [OPTION...] [FILE] [FILE] ...
Sign files with the trusted timestamp service

  -C, --ca_path=PATH         CA Path
  -d, --req_delay=MS         Request delay in milliseconds
  -s, --save=PATH            Save timestamp signatures to a directory
  -v, --verbose              Increase verbosity level
  -V, --validate             Validate timestamp signatures
  -?, --help                 Give this help list
      --usage                Give a short usage message
      --version              Print program version
```

#### Signing Proxy
```
Usage: signing_proxy [OPTION...]
Proxy between the client and timestamp signing service

  -p, --period=MS            Request polling period
  -q, --queue-len=LEN        Request queue length
  -v, --verbose              Increase verbosity level
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
```

#### Trusted Timestamp Service
```
Usage: signing_service [OPTION...]
Timestamp signing service

  -c, --conf=PATH            Configuration file path
  -s, --conf_sect=SECTION    Configuration section
  -v, --verbose              Increase verbosity level
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
```

#### Trusted Timestamp Cryptography Test
```
Usage: ts_test [OPTION...] [FILE] [FILE] ...
Generate TS request, TS sign, and TS validate test

  -c, --config=PATH          Configuration file
  -C, --ca_path=PATH         CA Path
  -n, --loops=ITERATIONS     Number of test iterations
  -s, --section=SECTION      Configuration TSA section
  -v, --verbosity            Increase verbosity level
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
```

## Directory Structure

### Source, Configuration, and Scripts
The build processes copies configuration resources into appropriate output
build directories. An attempt to run scripts from the `scripts` directory will
likely result in an error.
```
time_demo
├── conf
│   └── OpenSSL configuration resources
├── include
│   └── Trusted Timestamp Demo headers
├── scripts
│   └── Test and launch scripts
└── src
    └── Trusted Timestamp Demo sources
```

### Output Directory
The output directory and its components are auto-generated by the build process.
```
time_demo
├── ca
    ├── priv
    │   │   └── tsa_ca_key.pem         Certificate authority private key
    │   └── tsa_ca.pem                 Certificate authority certificate
    ├── high
    │   ├── planner                    Timestamp client application binary
    │   └── signing_proxy              Timestamp proxy application binary
    └── low
        ├── pki
        │   ├── priv
        │   │   ├── tsa_key.pem        Timestamp service private
        │   │   └── tsa_req.pem        Timestamp service CA sign request
        │   ├── tsa_cert.pem           Timestamp service certificate
        │   └── tsa.conf               Timestamp service configuration file
        ├── signing_service            Timestamp service application binary
        ├── ts_test                    Timestamp cryptography unit test binary
        └── ts_test.sh                 Timestamp cryptography memory test script
```

## Testing
Trusted timestamp sequence of
```
[Proxy Request] -> [Timestamp Request] -> [Timestamp Sign] -> [Timestamp Verify]
```
is tested in a standalone application ```low/ts_test```.

```ts_test.sh``` script executes the test application under different levels of
verbosity and checks for memory leaks and errors.

## Outstanding Tasks
- Add and test GAPS annotations
- Add a worker thread in signing proxy for sending back client replies. This
  step may be needed to monotonize proxy to service request period if
  transmission of a timestamp response over a GAPS channel takes a measurable
  amount of time
- Improve unit tests by adding proper test build targets