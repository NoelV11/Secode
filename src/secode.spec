Name: Secode
Summary: Examines C/C++ source code for security flaws
Version: 2.0.10
Release: 1%{?dist}
License: GPLv2+
Group: Development/Tools
Requires: python
BuildArch: noarch
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
Secode scans through C/C++ source code,
identifying lines ("hits") with potential security flaws.
By default it reports hits sorted by severity, with the riskiest lines first.

%prep
%setup  -q

%build
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
install -m755 -D secode ${RPM_BUILD_ROOT}%{_bindir}/secode
install -m644 -D secode.1 ${RPM_BUILD_ROOT}%{_mandir}/man1/secode.1

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc README.md ChangeLog COPYING secode.ps
%{_bindir}/*
%{_mandir}/man1/*

%changelog


# vim:set ai ts=4 sw=4:
