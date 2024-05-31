import cv2
import requests
import time

from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing import image
import numpy as np

def load_and_preprocess_image(img_path, target_size=(224, 224)):
    img = image.load_img(img_path, target_size=target_size)
    img_array = image.img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)  # Add batch dimension
    img_array /= 255.0  # Normalize to [0, 1]
    return img_array

def khoangcach(url):
    # url = 'http://192.168.120.15:8080/setServo1'
    response = requests.get(url)
    if response.status_code == 200:
        return int(response.json())
# Mở camera
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Cannot open camera")
    exit()

while True:
    # Đọc một frame từ camera
    ret, frame = cap.read()

    if not ret:
        print("Cannot receive frame (stream end?). Exiting ...")
        break

    # Hiển thị frame
    cv2.imshow('Camera', frame)

    # Chờ phím bấm
    key = cv2.waitKey(1) & 0xFF
    kc = khoangcach("http://192.168.120.123:8080/sieuam2")

    # Nếu bấm phím '1', lưu ảnh
    # if key == ord('1'):
    if kc <= 8:
        
        cv2.imwrite('captured_image.png', frame)
        print("Image saved as captured_image.png")
        image_path = 'C:\\Users\\ADMIN\\Documents\\Zalo Received Files\\fe\\captured_image.png'
        img = load_and_preprocess_image(image_path)
        # Lấy ra nhãn từ model với ảnh captured_image

        # Predict the class of the image
        model_path = "C:\\Users\\Admin\\Downloads\\waste_model.h5"
        model = load_model(model_path)
        predictions = model.predict(img)
        max_prob_index = np.argmax(predictions)

        # Display the results
        labels=['Nonrecycle', 'Recyclable']
        label=labels[max_prob_index]
        print(label)
        if label=="Nonrecycle":
            url = 'http://192.168.120.123:8080/setServo1'
            params = {'angle': 0}
            response = requests.get(url, params=params)
            if response.status_code == 200:
                print('Request thành công!')
                time.sleep(4)
                params = {'angle': 180}
                res=requests.get(url, params=params)

        if label=="Recyclable":
            url = 'http://192.168.120.123:8080/setServo2'
            params = {'angle': 180}
            response = requests.get(url, params=params)
            if response.status_code == 200:
                print('Request thành công!')
                time.sleep(4)
                params1 = {'angle': 0}
                res=requests.get(url, params=params1)
                if res.status_code == 200:
                    print('Đóng thành công!')
                    

    

    # Thoát vòng lặp nếu bấm phím 'q'
    if key == ord('q'):
        break

# Giải phóng camera và đóng cửa sổ
cap.release()
cv2.destroyAllWindows()
