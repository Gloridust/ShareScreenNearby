# ShareScreenNearby

ShareScreenNearby is a Python program for sharing screen content within a local network. With this program, users can view their screen content on other devices within the local network through a web browser.

Currently developed in Python, with an experimental version in C language.

<p align="center">
  <a href="#">
    <img width="180" src="./src/logo-r.webp">
  </a>
</p>
<h2 align="center">ShareScreenNearby</h2>
<div align="center"><p><a href="./README.md">简体中文</a>|<a href="./README_EN.md">English(US)</a></p></div>


## Features

1. **Real-time Screen Sharing**: Devices within the local network can access the screen content of the host machine by visiting the host's IP address and corresponding port number.
2. **Custom Port and Path**: Users can input custom port numbers and access paths when starting the program.

## Dependencies Installation

Before using this project, ensure you have installed the following dependencies:

```bash
pip install flask pyautogui pillow
```

## Usage Instructions

1. Clone the project to your local machine:

```bash
git clone https://github.com/Gloridust/ShareScreenNearby.git
cd ShareScreenNearby
```

2. Run the program:

```bash
python ShareScreenNearby_en.py
```

3. Input the port number and path as prompted, for example:

```plaintext
Please enter the port number: 5000
Please enter the path (default is '/'): /stream
```

4. In the web browser of other devices within the local network, enter `http://<hostIP>:<port>/<path>` to view the screen content. For example, if the host IP is `192.168.1.100`, port is `5000`, and path is `/stream`, then enter `http://192.168.1.100:5000/stream` in the browser.

## Packaging as an Executable File

Use PyInstaller to package the Python program into a standalone executable file. Below are the packaging steps:

1. Install PyInstaller:

```bash
pip install pyinstaller
```

2. Run the following command in the project root directory:

```bash
pyinstaller --onefile --icon=./src/logo-r.webp ShareScreenNearby_en.py
```

This command will generate an executable file named `ShareScreenNearby_en.exe` in the `dist` folder.

## File Structure

The file structure of the project is as follows:

```
ShareScreenNearby/
│
├── src/
│   └── logo-r.webp
├── ShareScreenNearby_en.py
└── README_EN.md
```

## Notes

1. Ensure all required libraries are correctly installed before packaging.
2. When packaging on different operating systems, it is recommended to run the packaging command on the respective operating system.