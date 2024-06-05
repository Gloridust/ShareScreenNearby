from flask import Flask, Response
import pyautogui
import io
import time
from PIL import Image

app = Flask(__name__)

def generate_frames(frame_rate=10, quality=75):
    interval = 1 / frame_rate
    while True:
        screen = pyautogui.screenshot()
        img_byte_arr = io.BytesIO()
        screen.save(img_byte_arr, format='JPEG', quality=quality)
        img_byte_arr = img_byte_arr.getvalue()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + img_byte_arr + b'\r\n')
        time.sleep(interval)

@app.route('/<path:path>')
def stream(path):
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    print("请设置相关信息，按下ENTER可取默认值：")
    
    port = input("请输入端口号(默认为 80)：")
    if port.strip() == "":
        port = "80"
    port = int(port)
        
    route = input("请输入路径 (默认为 '/')：")
    if route.strip() == "":
        route = "/"

    frame_rate = input("请输入帧率 (默认为 15)：")
    if frame_rate.strip() == "":
        frame_rate = 15
    else:
        frame_rate = int(frame_rate)

    quality = input("请输入图片质量 (1-100，默认为 50)：")
    if quality.strip() == "":
        quality = 50
    else:
        quality = int(quality)

    @app.route(route)
    def custom_route():
        return Response(generate_frames(frame_rate=frame_rate, quality=quality), mimetype='multipart/x-mixed-replace; boundary=frame')

    app.run(host='0.0.0.0', port=port)
