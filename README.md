# Console animation

---

## 💡 Description

Simple program, that can convert video file into ASCII animation and play it in the terminal

**Checkout links:**
* [Dockerhub image](https://hub.docker.com/repository/docker/maksimphono/console_animation/general)

## 🚀 Getting started

To get started, just clone the repo and run "install" script (you probably need to change permissions), it will pull the docker image and add 'conanim' command to your "/usr/local/bin" directory, after that you should be good to go.

### 🧰 Prerequisites
* **Docker engine** this program runs inside of docker image, so it won't have to install dependency on your local machine, instructions to install docker can be found here:
    * [Download Docker Desktop](https://www.docker.com/products/docker-desktop)

### 🔨 Instalation
1. **Clone repo** First you need to clone this repo and navigate to the "/sh" derectory:
    ```bash
    git clone 
    cd sh
    ```
2. **Change permissions and install** Run instalation script, it will pull docker image:
    ```bash
    sudo chmod +x install main
    ./install v1.0.2
    ```

3. **Change path to the storage (Recommended)** Open file "config" in the root of the repo, find variable "STORAGE_PATH" and set the path to a folder you want to use to store saved files. You can also adjust default values of the parameters in this file

4. **Run the program** That's pretty much it, just run the 'conanim' (or 'sh/main') script, providing parameters
    ```bash
    conanim -i /path/to/video.mp4
    ```
    Or:
    ```bash
    conanim -n "file_name" # if you already saved a file
    ```

You can also build the docker image youself, in that case don't bother running 'install' script, build the image, tag it, and change the last line in the "sh/main" script, where image name and tag are specified, then you should be able to run the 'main' script
```bash
docker build -t my_console_animation:latest -f ./Dockerfile_production .
```

**Note, that it is recommended to provide your own path to directory in variable STORAGE_PATH in the "config" file, otherwise the instalation script will create storage in your $HOME/.local/share/console_animation directory automatically**

## ▶️ Usage
There are 2 ways, how you can use this program. 
- ① Firstly you can call it, providing path to a video file with parameter "-i", and specify other parameters (fps, size, timeframe) in such case program will create ASCII animation from that video file, applying all your (or default) parameters. Note, that in this scenario the program need some time to create the animation, and it will **use as much CPU cores as it can** to speed up the process, the larger value of each parameter will result in longer creation time, so be patient. 
- ② Secondly you can provide a name to already saved animation (if you did save any) with parameter "-n", in such case program will just read this animation from the storage and start play it in the terminal without big delay. Note, that in this scenario program **will ignore any other provided parameter**, because animation is saved to the storage with already specified parameters.

When the animation is running in the terminal, yout can hit 'q' on the keyboard to stop the program, or you can hit 's' to save the animation being played, in this case program will ask you to provide name to the file, this is how you store animation to the storage.

📹 Example:
```bash
conanim -i /path/to/video.mp4 --fps 8 --size "179x68" --time "5-15"
```
* This will create animation from the video file, specifying fps (frames per second) as 8, giving size 179 by 68 symbols **(make sure to resize your terminal to match specified size, otherwise it will look ugly)**, and taking only part of the original video from 5'th second to 15'th second. This command needs fairly long time, but it should produce high-resolution (as much as it possible with ASCII symbols) animation, that can be later saved into the storage.

## 🎙️ Credits
This program uses these open source software projects:
    - **[jp2a](https://github.com/cslarsen/jp2a)**
        - Author: **Christian Stigen Larsen**
    - **[ffmpeg](https://github.com/FFmpeg/FFmpeg)**
        - Authors: **Pavel Koshevoy** and **James Almer**

## 🛈 PS
Note, that this is NOT a multimedia software (it isn't meant to be), this is just a fun stuff, that you run in your secondary terminal just to make it a little more lively and fun. So please don't use it to actually watch the video.

If you want to report a bug 🐛 please submit an issue with description. If you can improve the source code, please submit a pull request, I will be more than happy to get some guidelines and improve the app and my skills😃.