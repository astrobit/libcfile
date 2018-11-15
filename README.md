# libcfile

A c++ wrapper for the standard C functions that is also thread safe and ensures closure of files. 


### Prerequisites

- g++ or other compiler with c++11
- autotools (autoconf, automake, libtool)

## Compiling

RPM and .deb will be available soon. For now:
- clone this repository or get source from a tag.
```
aclocal
libtoolize
autoconf
automake --add-missing
autoreconf
./configure
make
```

### Installing

RPM and .deb will be available soon. For now, follow the steps above in [Compiling](Compiling), then

```
make install
```

## Contributing

Contributions are welcome.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/libcfile/tags). 

## Authors

* **Brian W. Mulligan** - *Initial work* - [Astrobit](https://github.com/astrobit)

See also the list of [contributors](https://github.com/libcfile/contributors) who participated in this project.

## License

This project is licensed under the Unlicense License - see the [LICENSE](LICENSE) file for details

## Acknowledgments

* cppreference.com for language reference
