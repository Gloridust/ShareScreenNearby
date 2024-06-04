import io
import time
from flask import Flask, Response
import pyautogui

app = Flask(__name__)

@app.route('/')
def index():
    return "屏幕共享服务器已启动，请访问 /screen 查看屏幕内容"

@app.route('/screen')
def screen():
    def generate():
        while True:
            # 捕捉屏幕内容
            screenshot = pyautogui.screenshot()
            byte_io = io.BytesIO()
            screenshot.save(byte_io, 'JPEG')
            byte_io.seek(0)
            frame = byte_io.read()
            
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
            time.sleep(0.1)

    return Response(generate(), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    port = int(input("请输入端口号: "))
    app.run(host='0.0.0.0', port=port, threaded=True)
