from flask import Flask, Response
import pyautogui
import io
import time
from PIL import Image

app = Flask(__name__)

def generate_frames():
    while True:
        screen = pyautogui.screenshot()
        img_byte_arr = io.BytesIO()
        screen.save(img_byte_arr, format='JPEG')
        img_byte_arr = img_byte_arr.getvalue()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + img_byte_arr + b'\r\n')
        time.sleep(0.1)

@app.route('/stream')
def stream():
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    port = int(input("请输入端口号："))
    app.run(host='0.0.0.0', port=port)
