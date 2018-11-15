Name:           libcfile
Version:        1.0.0
Release:        1%{?dist}
Summary:        Thread-safe c++ wrapper for C file i/o

License:        Unlicense
URL:            https://github.com/astrobit/libcfile
Source0:        https://astronaos.com/libcfile/libcfile-1.0.0.tar.gz

#The versions listed for requires here are based on what is on my developer
# system; the code may work with earlier versions of each of these libraries.
# If you have a system with an older version of one of these libraries and
# cannot update it, please try building the source; if you find that it
# compiles correctly, let me know and I will make the update to the RPM, or
# please generate a pull request on github
BuildRequires:  gcc >= 6.0
Requires:       

%description
Shared library objects for thread-safe c++ wrappers for C file i/o routines.

%package        devel
Summary:        Development needs for libcfile
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}

%description    devel
This package contains headers and libraries required to build applications that
use the libcfile.


%prep
%autosetup


%build
%configure
%make_build


%install
rm -rf $RPM_BUILD_ROOT
%make_install


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
%license LICENSE
%doc README.md
%{_bindir}/*
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,-)
#%%doc examples
%license LICENSE
%doc README.md
%{_includedir}/*.h
%{_libdir}/*.a
%{_libdir}/*.la
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc


%changelog
* Thu Nov 15 2018 Brian W. Mulligan <bwmulligan@astronaos.com>
- Initial RPM release


