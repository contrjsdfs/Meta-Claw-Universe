# **Meta-Claw-Universe 项目说明**
## **项目简介**
Embedded development based on RK3588
- **嵌入式端 (main)**：rk3588
- **Qt 客户端 (qt/01_test)**：采用 Qt 4.7 开发的图形化控制界面
- **YOLOv8 训练 (yolov8)**：目标检测模型训练与部署

---

## **项目结构**
```
.
├── main/                  # 嵌入式端代码（硬件控制）
│   └── ...
│
├── qt/01_test/            # Qt 4.7 客户端
│   ├── CPP/h/               # 源代码
│   ├── ui/                # 界面设计文件
│   └── ...
│
└── yolov8/                # YOLOv8 训练代码
    ├── datasets/          # 训练数据集
    ├── weights/           # 模型权重
    └── ...
```

