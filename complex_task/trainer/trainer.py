from ultralytics import YOLO

# Load a model
# model = YOLO("yolov8n.yaml")  # build a new model from scratch
model = YOLO("../model/best.pt")  # load a pretrained model (recommended for training)

# Use the model
# model.train(data="./rune.yaml", epochs=10,device=0,amp=False)  # train the model
# metrics = model.val()  # evaluate model performance on the validation set
path = model.export(format="onnx",opset=12)  # export the model to ONNX format