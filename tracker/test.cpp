
#include "opencv4/opencv2/opencv.hpp"

#include "ByteTrack/BYTETracker.h"
#include "ByteTrack/Object.h"
#include "detector/YOLO11Seg.hpp"
#include <algorithm>
#include <iterator>

byte_track::Object getObjFromSeg(const Segmentation &seg) {
  std::cout << "Convert Obj to Seg\n";
  return byte_track::Object{
      byte_track::Rect<float>(static_cast<float>(seg.box.x),
                              static_cast<float>(seg.box.y),
                              static_cast<float>(seg.box.width),
                              static_cast<float>(seg.box.height)),
      seg.classId, seg.conf};
}

int main(int argc, char *argv[]) {

  byte_track::BYTETracker tracker;

  //
  cv::VideoCapture vc("/media/matvey/EB6B-E36F/flash_data/animation.mp4");
  while (true) {
    const std::string labelsPath =
        "/home/matvey/projects/models/coco.names"; // Path to class labels
    const std::string modelPath =
        "/home/matvey/projects/models/yolo11n-seg.onnx"; // Path to YOLO11 model
    const std::string imagePath =
        "/media/matvey/EB6B-E36F/flash_data/animation_color.mp4"; // Path to
                                                                  // input image
    bool isGPU = true; // Set to false for CPU processing

    // Initialize the YOLO11 segmentor
    YOLOv11SegDetector segmentor(modelPath, labelsPath, isGPU);

    cv::Mat image;
    // Load an image
    auto result = vc.read(image);
    std::vector<byte_track::Object> objects;

    // Perform object segmentation to get segmentation masks and bboxs
    std::vector<Segmentation> results = segmentor.segment(image, 0.5f, 0.7f);

    // Draw bounding boxes on the image
    segmentor.drawSegmentationsAndBoxes(image, results);

    std::transform(results.begin(), results.end(), std::back_inserter(objects),
                   getObjFromSeg);

    auto tracks = tracker.update(objects);

    cv::imshow("Video", image);
    cv::waitKey(25);
  }
  return 0;
}