import fire, os, cv2
import numpy as np
import torch

from ultralytics import YOLO, RTDETR


class Detector_CLI(object):

    def convert(self, model_path='./models/', model_type='yolo11n', int8=False, fp16=False, agnostic_nms=False):
        if int8 and fp16: return 

        path = model_path + "fp32/" + model_type

        if int8:
            path = model_path + "int8/" + model_type

        if fp16:
            path = model_path + "fp16/" + model_type

        if model_type.find("yolo"):
            model = YOLO(path + ".pt")
            # Export the model to TensorRT
            model.export(format="engine", half=fp16, int8=int8, agnostic_nms=agnostic_nms, device=0)  # creates 'yolo11n.engine'
        else:
            # Load a COCO-pretrained RT-DETR-l model
            model = RTDETR(path + ".pt")

            # Display model information (optional)
            model.info()

            model.export(format="engine", half=fp16, int8=int8, agnostic_nms=agnostic_nms, device=0) 

        return True
    # python3 main.py convert --model_path=./models/yolo11n 

    def inference_dir(self, model_path='./models/fp32/yolo11n', input_path="/input_data", output_path="/output_data", draw_rects=False):       
        trt_model = YOLO(model_path + ".engine")

        img_names = [i for i in os.listdir(input_path)]

        for img_name in img_names:
            img = cv2.imread(input_path + "/" + img_name)
            results = trt_model.predict(img, device="cuda:0")
            results[0].save()
            if draw_rects:
                for box in results[0].boxes:
                    xyxy = list(map(lambda x: int(x), box.xyxy.cpu().numpy().tolist()[0]))
                    cv2.rectangle(img, (xyxy[0], xyxy[1]), (xyxy[2], xyxy[3]), (255, 0, 0))
                cv2.imwrite(output_path + "/" + img_name, img)
    # python3 main.py inference_dir --model_path=./models/yolo11n --input_path=/input_data --output_path=/output_data
            
    def inference_video(self, model_path='./models/fp32/yolo11x', input_path="/input_data", output_path="/output_data", draw_rects=False):
        trt_model = YOLO(model_path + ".engine")

        vc = cv2.VideoCapture(input_path)
        c = 0

        preprocess = []
        inference = []
        postprocess = []

        while True:
            success, img = vc.read()
            if not success: 
                break 

            results = trt_model.predict(img, device="cuda:0") 
            speed_res = results[0].speed

            preprocess += [speed_res["preprocess"]]
            inference += [speed_res["inference"]]
            postprocess += [speed_res["postprocess"]]

            if draw_rects:
                results[0].save()
                
                for box in results[0].boxes:
                    xyxy = list(map(lambda x: int(x), box.xyxy.cpu().numpy().tolist()[0]))
                    cv2.rectangle(img, (xyxy[0], xyxy[1]), (xyxy[2], xyxy[3]), (255, 0, 0))
                
                cv2.imwrite(os.path.join(output_path, f"video_{c}.png"), img)
            c += 1
        vc.release()

        preprocess.sort()
        inference.sort()
        postprocess.sort()

        print(f"Amount of frames: {c}")
        print(f"Preprocess 95%: {preprocess[int(c*0.95)]}")
        print(f"Inference 95%: {inference[int(c*0.95)]}")
        print(f"Postprocess 95%: {postprocess[int(c*0.95)]}")
        print(preprocess)

            
def main():
    fire.Fire(Detector_CLI)

if __name__ == '__main__':
  main()