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
    print("Please set the relevant information, press ENTER to use the default value:")
    
    port = input("Please enter the port number (default is 80): ")
    if port.strip() == "":
        port = "80"
    port = int(port)
        
    route = input("Please enter the route (default is '/'): ")
    if route.strip() == "":
        route = "/"

    frame_rate = input("Please enter the frame rate (default is 15): ")
    if frame_rate.strip() == "":
        frame_rate = 15
    else:
        frame_rate = int(frame_rate)

    quality = input("Please enter the image quality (1-100, default is 50): ")
    if quality.strip() == "":
        quality = 50
    else:
        quality = int(quality)

    @app.route(route)
    def custom_route():
        return Response(generate_frames(frame_rate=frame_rate, quality=quality), mimetype='multipart/x-mixed-replace; boundary=frame')

    app.run(host='0.0.0.0', port=port)
