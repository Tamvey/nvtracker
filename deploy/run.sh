sudo docker run \
    --ipc=host \
    --runtime=nvidia \
    -it \
    -v /media/nvidia/nvidia_tx2_files2/nvtracker/scripts:/scripts \
    -v /media/nvidia/EB6B-E36F/kitti_from_nuscenes_trainval06/training/image_02/scene-0625:/input_data \
    -v /media/nvidia/EB6B-E36F/output_data:/output_data \
ultralytics/ultralytics:latest-jetson-jetpack4