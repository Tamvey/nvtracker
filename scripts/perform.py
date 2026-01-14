from sahi.predict import get_sliced_prediction
from sahi import AutoDetectionModel

import cv2

model_path = "models/yolov5n.pt"
input_image_path = "demo_data/terrain2.png"


video = cv2.VideoCapture("/media/matvey/EB6B-E36F/flash_data/animation_color.mp4")
c = 0
while True:
    # img = cv2.imread(input_image_path)
    res, img = video.read()
    save_path = f"demo_data/save_{c}.png"
    cv2.imwrite(save_path, img)

    detection_model = AutoDetectionModel.from_pretrained(
        model_type="ultralytics",
        model_path=model_path,
        confidence_threshold=0.6,
        device="cuda:0",  # or 'cuda:0'
    )
    result = get_sliced_prediction(
        save_path,
        detection_model,
        slice_height=640,
        slice_width=640,
        overlap_height_ratio=0.2,
        overlap_width_ratio=0.2,
    )

    object_prediction_list = result.object_prediction_list

    for po in object_prediction_list:
        bbox = po.bbox 

        font = cv2.FONT_HERSHEY_SIMPLEX 
        font_scale = 1.5 
        color = (0, 255, 0) 
        thickness = 2 
        line_type = cv2.LINE_AA

        cv2.rectangle(img, (int(bbox.minx), int(bbox.miny)), (int(bbox.maxx), int(bbox.maxy)), (0, 0, 255), 2)
        cv2.putText(img, po.category.name, (int(bbox.minx), int(bbox.miny)), font, font_scale, color, thickness, line_type)

    cv2.imwrite(save_path, img)
    c += 1