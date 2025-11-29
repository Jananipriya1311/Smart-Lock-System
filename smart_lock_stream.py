
import cv2
from flask import Flask, Response
import threading
import requests
import time

# ==== BLYNK CONFIG ====
# Replace with your own Blynk device token
DEVICE_TOKEN = "YOUR_BLYNK_DEVICE_TOKEN"
BASE_URL = "https://blr1.blynk.cloud/external/api"

app = Flask(__name__)
frame = None
streaming = False

# ==== CAMERA INITIALIZATION ====
def init_camera():
    for i in range(3):  # try first 3 cameras
        cam = cv2.VideoCapture(i, cv2.CAP_DSHOW)
        if cam.isOpened():
            print(f"✅ Camera found at index {i}")
            return cam
    print("❌ No camera detected! Please check webcam connection.")
    return None

camera = init_camera()

# ==== FRAME GENERATOR ====
def generate_frames():
    global frame
    while True:
        if frame is not None:
            success, buffer = cv2.imencode('.jpg', frame)
            if success:
                yield (b'--frame\r\n'
                       b'Content-Type: image/jpeg\r\n\r\n' + buffer.tobytes() + b'\r\n')

@app.route('/video')
def video():
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

# ==== CHECK BLYNK (ESP32) STATUS ====
def check_event():
    global streaming
    while True:
        try:
            url = f"{BASE_URL}/get?token={DEVICE_TOKEN}&V2"
            r = requests.get(url, timeout=5)

            if r.status_code == 200:
                value = r.text.strip()
                if value == "1":
                    if not streaming:
                        print("👀 Person detected — starting camera stream...")
                    streaming = True
                else:
                    if streaming:
                        print("🛑 Person gone — stopping camera stream.")
                    streaming = False
            else:
                print(f"⚠️ Blynk error: {r.status_code}")
        except Exception as e:
            print("Error checking V2:", e)
        time.sleep(2)

# ==== CAPTURE CAMERA FRAMES ====
def capture_frames():
    global frame, streaming, camera
    if camera is None:
        print("🚫 No camera available for capture.")
        return

    while True:
        if not camera.isOpened():
            print("⚠️ Camera disconnected — retrying...")
            camera = init_camera()
            time.sleep(1)
            continue

        success, img = camera.read()
        if not success:
            print("⚠️ Frame read failed — retrying...")
            time.sleep(0.2)
            continue

        if streaming:
            frame = img
        else:
            frame = None
        time.sleep(0.05)

# ==== MAIN ====
if __name__ == "__main__":
    print("🌐 Flask camera server running at http://127.0.0.1:5000/video")
    print("💡 When ESP32 detects someone, camera will start automatically.")
    threading.Thread(target=check_event, daemon=True).start()
    threading.Thread(target=capture_frames, daemon=True).start()
    app.run(host="0.0.0.0", port=5000)
