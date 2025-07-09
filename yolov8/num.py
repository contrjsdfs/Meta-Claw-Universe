from ultralytics import YOLO

model = YOLO('./runs/detect/train5/weights/best.pt')
print(model.names)  # 输出类别名称，确保是 6 个
