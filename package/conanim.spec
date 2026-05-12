Name:           conanim
Version:        0.0.0
Release:        1%{?dist}
Summary:        Visualizing the woods in the terminal

License:        MIT
URL:            https://github.com/maksimphono/console_animation
Source0:        conanim-0.0.0.tar.gz

# Required to COMPILE the code
BuildRequires:  gcc-c++
BuildRequires:  cmake
BuildRequires:  ncurses-devel
BuildRequires:  fmt-devel

# Required to RUN the code (found via execvp)
Requires:       ncurses
Requires:       ffmpeg
Requires:       jp2a
Requires:       fmt
%description
(Experimental build)
Simple program, that can convert video file into ASCII animation and play it in the terminal 

%prep
%setup -q -n conanim-0.0.0
#%autosetup

%build
%cmake
%cmake_build

%install
%cmake_install

%files
%{_bindir}/conanim
%license LICENSE
%doc README.md

%changelog
* Mon May 12 2026 Maksim Trukhin <trukhinmaksimof@proton.me> - 0.0.0-1
- Initial build with ffmpeg and jp2a runtime support.
