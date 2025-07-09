from ultralytics import YOLO

# Load a model
model = YOLO('./runs/detect/train10/weights/best.pt')

# Export the model
model.export(format='rknn',opset=12)

