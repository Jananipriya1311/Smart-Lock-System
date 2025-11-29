# Smart Lock System


## Overview

The **Smart Lock System** is an IoT and AI-based automation project designed to enhance **home and office security**.  
It integrates hardware and software components to provide **contactless access control**, **real-time monitoring**, and **live camera streaming**.  

**Components used:**  
- ESP32 microcontroller for IoT connectivity  
- Ultrasonic sensor for human detection  
- Servo motor for locking/unlocking  
- Buzzer for alerts  
- Flask-based Python server for webcam streaming  
- Blynk Cloud for remote monitoring and notifications  

The system is lightweight, responsive, and cost-effective, making it ideal for smart homes and offices.

---

## Features

- Detects human presence in real-time using an ultrasonic sensor  
- Automatically locks/unlocks the door with a servo motor  
- Streams live video via Flask when movement is detected  
- Sends notifications to Blynk Cloud mobile app  
- AI-inspired rule-based logic for automated decision making  

---

## Hardware Required

- ESP32 microcontroller  
- Ultrasonic sensor (HC-SR04)  
- Servo motor  
- Buzzer  
- USB camera or PC webcam  
- Connecting wires and power supply  

---

## Software Required

- Arduino IDE (for ESP32 code)  
- Python 3.x (for Flask server)  
- Python libraries: `Flask`, `opencv-python`, `requests`  
- Blynk Cloud account and mobile app  

---

## Folder Structure

```
Smart-Lock-System/
│
├─ smart_lock_stream.py       # Flask streaming Python script
├─ ESP32_code.ino             # Arduino/ESP32 code
├─ requirements.txt           # Python dependencies
├─ images/                    # Project images
│   ├─ hardware_setup.jpg
│   ├─ serial_monitor.jpg
│   └─ blynk_app.jpg
```

---

## Step-by-Step Guide to Build

### Step 1: Hardware Setup
1. Connect the ultrasonic sensor, servo motor, buzzer, and webcam as per your schematic.  
2. Power the ESP32 via USB.  
3. Ensure all connections are correct.  

---

### Step 2: Arduino IDE Setup
1. Open Arduino IDE and paste the ESP32 code (`ESP32_code.ino`).  
2. Upload the code to the ESP32 board.  
3. Open Serial Monitor (baud rate 9600) to check distance readings and messages:  
   - "Person Detected"  
   - "No Object"  

---

### Step 3: Blynk Cloud Setup
1. Create a new template in Blynk Cloud.  
2. Add **Virtual Pin V2** (Integer, range 0-1).  
3. ESP32 updates `V2 = 1` when a person is detected.  
4. Configure event notifications in the Blynk app to display “Person Detected”.

---

### Step 4: Python Flask Streaming
1. Install required Python libraries:

```bash
pip install Flask opencv-python requests
```

2. Run the Flask server:

```bash
python smart_lock_stream.py
```

3. Flask server continuously checks Blynk Virtual Pin V2.  
4. If `V2 = 1`, the webcam starts streaming automatically.  
5. Access live feed at:

```
http://127.0.0.1:5000/video
```

or via the Blynk app using the device widget.

---

### Step 5: Testing

1. Walk near the sensor and verify:

   * ESP32 detects presence  
   * Servo motor locks/unlocks smoothly  
   * Blynk app receives notifications instantly  
   * Flask server streams live video  

2. Detection occurs in **<1 second**, and streaming is real-time.

---

## Workflow

1. Ultrasonic sensor detects a person  
2. ESP32 updates Blynk Virtual Pin V2 = 1  
3. Blynk triggers notifications to the mobile app  
4. Flask server detects V2 = 1  
5. Webcam starts streaming live video  
6. User verifies visitor and the system responds automatically  

---

## Results

* Detection range: **up to 2 meters**  
* Response time: **<1 second**  
* Accurate contactless lock/unlock with servo motor  
* Real-time notifications via Blynk app  
* Stable and responsive live camera streaming  
* Reliable AI-inspired rule-based automation
```
