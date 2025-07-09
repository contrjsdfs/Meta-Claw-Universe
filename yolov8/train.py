from ultralytics import YOLO

if __name__ == '__main__':
    model = YOLO('./ultralytics/cfg/models/v8/yolov8.yaml').load("./yolov8n.pt")#创建模型并加载预训练权重
    model.model.nc = 5  # 强制修改输出类别数
    # Train the model
    model.train(epochs=100, batch=8 ,data='./my-dataset/data.yaml')
    #epochs表示训练的轮数，batch表示每次加载图片的数量，data是数据集的路径这里使用的是之前创建的yaml文件

