# ShareScreenNearby

ShareScreenNearby 是一个用于在局域网内共享屏幕内容的 Python 程序。通过该程序，用户可以在局域网内的其他设备上，通过浏览器查看本机屏幕内容。

目前由Python开发，C语言版本还在实验性阶段。

<p align="center">
  <a href="#">
    <img width="180" src="./src/logo-r.webp">
  </a>
</p>
<h1 align="center">ShareScreenNearby</h1>

## 功能介绍

1. **流式屏幕共享**：局域网内的设备可以通过访问本机 IP 和对应端口号来实时查看本机屏幕内容。
2. **自定义端口和路径**：用户可以在启动程序时输入自定义的端口号和访问路径。

## 依赖包安装

在使用本项目之前，请确保已安装以下依赖包：

```bash
pip install flask pyautogui pillow
```

## 使用说明

1. 克隆项目到本地：

```bash
git clone https://github.com/Gloridust/ShareScreenNearby.git
cd ShareScreenNearby
```

2. 运行程序：

```bash
python ShareScreenNearby.py
```

3. 根据提示输入端口号和路径，例如：

```plaintext
请输入端口号：5000
请输入路径 (默认为 '/')：/stream
```

4. 在局域网内其他设备的浏览器中输入 `http://<本机IP>:<端口号>/<路径>` 查看屏幕内容。例如，假设本机 IP 为 `192.168.1.100`，端口号为 `5000`，路径为 `/stream`，则在浏览器中输入 `http://192.168.1.100:5000/stream`。

## 打包为可执行文件

使用 PyInstaller 将 Python 程序打包为独立的可执行文件，以下是打包步骤：

1. 安装 PyInstaller：

```bash
pip install pyinstaller
```

2. 在项目根目录下运行以下命令：

```bash
pyinstaller --onefile --icon=./src/logo-r.webp ShareScreenNearby.py
```

此命令会在 `dist` 文件夹中生成一个名为 `ShareScreenNearby.exe` 的可执行文件。

## 文件结构

项目的文件结构如下：

```
ShareScreenNearby/
│
├── src/
│   └── logo-r.webp
├── ShareScreenNearby.py
└── README.md
```

## 注意事项

1. 在打包时，确保所有依赖库都已正确安装。
2. 在不同的操作系统上打包时，建议在相应的操作系统上运行打包命令。
