# Console animation

---

## 💡 Description

Simple program, that can convert video file into ASCII animation and play it in the terminal

**Checkout links:**
* [GitHub repo](https://github.com/maksimphono/console_animation)
* [GitLab repo](https://gitlab.com/maksimphono/console_video)
* [OBS package](https://build.opensuse.org/package/show/home:trukhinmaksim:conanim/conanim)

## 🚀 Getting started
To get started you gonna need to install the program (see below) and a video file, for example something downloaded from YouTube. Simply call
```bash
conanim -i /path/to/video.mp4
```
and it will do it's job.

You can also provide options like size, fps and time interval in the video that will be converted, in fact I **suggest** you do that, since defult options are very limited and are mostly there for demostration purposes. So you can run
```bash
conanim -i /path/to/video.mp4 -f 12 --size 90x30 --time 5-25
```
to process video from 5'th to 25'th second with 12 frames per second and size 90 by 30 symbols. Note, that it might take some time.


### 🔨 Instalation

#### Package manager
The easiest way is to use package manager of your distro. 
The process is basically connecting package repository, getting gpg key and installing using package manager. Instalation commands for some repos are provided below

* Ubuntu 26.04
```bash
echo 'deb http://download.opensuse.org/repositories/home:/trukhinmaksim:/conanim/xUbuntu_26.04/ /' | sudo tee /etc/apt/sources.list.d/home:trukhinmaksim:conanim.list
curl -fsSL https://download.opensuse.org/repositories/home:trukhinmaksim:conanim/xUbuntu_26.04/Release.key | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/home_trukhinmaksim_conanim.gpg > /dev/null
sudo apt update
sudo apt install conanim
```

* Ubuntu 25.10
```bash
echo 'deb http://download.opensuse.org/repositories/home:/trukhinmaksim:/conanim/xUbuntu_25.10/ /' | sudo tee /etc/apt/sources.list.d/home:trukhinmaksim:conanim.list
curl -fsSL https://download.opensuse.org/repositories/home:trukhinmaksim:conanim/xUbuntu_25.10/Release.key | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/home_trukhinmaksim_conanim.gpg > /dev/null
sudo apt update
sudo apt install conanim
```

* Debian 13:
```bash
echo 'deb http://download.opensuse.org/repositories/home:/trukhinmaksim:/conanim/Debian_13/ /' | sudo tee /etc/apt/sources.list.d/home:trukhinmaksim:conanim.list
curl -fsSL https://download.opensuse.org/repositories/home:trukhinmaksim:conanim/Debian_13/Release.key | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/home_trukhinmaksim_conanim.gpg > /dev/null
sudo apt update
sudo apt install conanim
```

* Debian 12:
```bash
echo 'deb http://download.opensuse.org/repositories/home:/trukhinmaksim:/conanim/Debian_12/ /' | sudo tee /etc/apt/sources.list.d/home:trukhinmaksim:conanim.list
curl -fsSL https://download.opensuse.org/repositories/home:trukhinmaksim:conanim/Debian_12/Release.key | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/home_trukhinmaksim_conanim.gpg > /dev/null
sudo apt update
sudo apt install conanim
```

* Fedoara 44:
```bash
dnf config-manager addrepo --from-repofile=https://download.opensuse.org/repositories/home:trukhinmaksim:conanim/Fedora_44/home:trukhinmaksim:conanim.repo
dnf install conanim
```

* openSUSE Tumbleweed
```bash
zypper addrepo https://download.opensuse.org/repositories/home:trukhinmaksim:conanim/openSUSE_Tumbleweed/home:trukhinmaksim:conanim.repo
zypper refresh
zypper install conanim
```

#### Get the binary fle directly
* Alternatively to the first way you can also just download the package and install it on your machine locally. Checkout binary packages for your distro on [build system](https://software.opensuse.org/download.html?project=home%3Atrukhinmaksim%3Aconanim&package=conanim)
Some links to binaries are provided below

* Debian 13 (amd64):
https://download.opensuse.org/repositories/home:/trukhinmaksim:/conanim/Debian_13/amd64/conanim_2.0.0-1_amd64.deb

* Debian 12 (amd64):
https://download.opensuse.org/repositories/home:/trukhinmaksim:/conanim/Debian_12/amd64/conanim_2.0.0-1_amd64.deb

* Ubuntu 26.04 (amd64):
https://download.opensuse.org/repositories/home:/trukhinmaksim:/conanim/xUbuntu_26.04/amd64/conanim_2.0.0-1_amd64.deb

* Ubuntu 25.10 (amd64):
https://download.opensuse.org/repositories/home:/trukhinmaksim:/conanim/xUbuntu_25.10/amd64/conanim_2.0.0-1_amd64.deb

* Fedora 44 (x86_64):
https://download.opensuse.org/repositories/home:/trukhinmaksim:/conanim/Fedora_44/x86_64/conanim-2.0.0-91.1.x86_64.rpm

* openSUSE Tumbleweed (x86_64)
https://download.opensuse.org/repositories/home:/trukhinmaksim:/conanim/openSUSE_Tumbleweed/x86_64/conanim-0.0.0-91.1.x86_64.rpm

#### Compile from source
* Of cource if you like to build everything by yourself, you can compile the project from source. 

* Requirements:
1. ffmpeg (version>=7.1)
2. jp2a
3. ncurses
4. cmake

Install requirements via the package manager of your distro.
Then clone this repo and enter it's directory
```bash
git clone https://github.com/maksimphono/console_animation
cd console_animation
```

Then compile the code using **cmake**:
```bash
cmake .
cmake --build .
```
After that it should build the binary *conanim* at the same directory and you should be able to use it.


### ▶️ Usage
There are 3 ways, how you can use this program. 
- ➀ Firstly you can call it, providing path to a video file with parameter **-i**, and specify other parameters (fps, size, time interval) in such case program will create ASCII animation from that video file, applying all your (or default) parameters. Note, that in this scenario the program need some time to create the animation, and it will **use as much CPU cores as it can** to speed up the process, the larger value of each parameter will result in longer creation time, so be patient. You can also specify number of CPU cores to use via parameter **-c**.
- ➁ Secondly you can provide a name to already saved animation (if you did save any) with parameter "-n", in such case program will just read this animation from the storage and start play it in the terminal without big delay. Note, that in this scenario program **will ignore any other provided parameter**, because animation is saved to the storage with already specified parameters.
- ➂ Thirdly you can take animation and just dump the entire frame sequence to the teminal by setting flag **-o**. This might be useful if you're planning on using it as a child process in your own project, in this case the program will dump everything to it's STDOUT, so you can read it with pipe (or whatever you're using).

When the animation is running in the terminal, yout can hit 'q' on the keyboard to stop the program, or you can hit 's' to save the animation being played, in this case program will ask you to provide name to the file, this is how you store animation to the storage.

📹 Example:
```bash
conanim -i /path/to/video.mp4 --fps 8 --size 179x68 --time 5-15
```
* This will create animation from the video file, specifying fps (frames per second) as 8, giving size 179 by 68 symbols **(make sure to resize your terminal to match specified size, otherwise it will look ugly)**, and taking only part of the original video from 5'th second to 15'th second. This command needs fairly long time, but it should produce high-resolution (as much as it possible with ASCII symbols) animation, that can be later saved into the storage.

### 🎙️ Credits
This program uses these open source software projects:
- **[jp2a](https://github.com/cslarsen/jp2a)**
    - Author: **Christian Stigen Larsen**
- **[ffmpeg](https://github.com/FFmpeg/FFmpeg)**
    - Authors: **Pavel Koshevoy** and **James Almer**

### 🛈 PS
Note, that this is NOT a multimedia software (it isn't meant to be), this is just a fun stuff, that you run in your secondary terminal just to make it a little more lively and fun. So please don't use it to actually watch the video.

If you want to report a bug 🐛 please submit an issue with description. If you can improve the source code, please submit a pull request, I will be more than happy to get some guidelines and improve the app and my skills😃.