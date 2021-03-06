# 目录
<!-- TOC -->

- [目录](#目录)
- [Faster R-CNN描述](#faster-r-cnn描述)
- [模型架构](#模型架构)
- [数据集](#数据集)
- [环境要求](#环境要求)
- [快速入门](#快速入门)
- [脚本说明](#脚本说明)
    - [脚本及样例代码](#脚本及样例代码)
    - [训练过程](#训练过程)
        - [用法](#用法)
        - [结果](#结果)
    - [评估过程](#评估过程)
        - [用法](#用法-1)
        - [结果](#结果-1)
- [模型描述](#模型描述)
    - [性能](#性能)
        - [训练性能](#训练性能)
        - [评估性能](#评估性能)
- [ModelZoo主页](#modelzoo主页)

<!-- /TOC -->

# Faster R-CNN描述

在Faster R-CNN之前，目标检测网络依靠区域候选算法来假设目标的位置，如SPPNet、Fast R-CNN等。研究结果表明，这些检测网络的运行时间缩短了，但区域方案的计算仍是瓶颈。

Faster R-CNN提出，基于区域检测器（如Fast R-CNN）的卷积特征映射也可以用于生成区域候选。在这些卷积特征的顶部构建区域候选网络（RPN）需要添加一些额外的卷积层（与检测网络共享整个图像的卷积特征，可以几乎无代价地进行区域候选），同时输出每个位置的区域边界和客观性得分。因此，RPN是一个全卷积网络，可以端到端训练，生成高质量的区域候选，然后送入Fast R-CNN检测。

[论文](https://arxiv.org/abs/1506.01497)：   Ren S , He K , Girshick R , et al. Faster R-CNN: Towards Real-Time Object Detection with Region Proposal Networks[J]. IEEE Transactions on Pattern Analysis and Machine Intelligence, 2015, 39(6).

# 模型架构

Faster R-CNN是一个两阶段目标检测网络，该网络采用RPN，可以与检测网络共享整个图像的卷积特征，可以几乎无代价地进行区域候选计算。整个网络通过共享卷积特征，进一步将RPN和Fast R-CNN合并为一个网络。

# 数据集

使用的数据集：[COCO 2017](<https://cocodataset.org/>)

- 数据集大小：19G
  - 训练集：18G，118,000个图像  
  - 验证集：1G，5000个图像
  - 标注集：241M，实例，字幕，person_keypoints等
- 数据格式：图像和json文件
  - 注意：数据在dataset.py中处理。

# 环境要求

- 安装[MindSpore](https://www.mindspore.cn/install)。

- 下载数据集COCO 2017。

- 本示例默认使用COCO 2017作为训练数据集，您也可以使用自己的数据集。

    1. 若使用COCO数据集，**执行脚本时选择数据集COCO。**
        安装Cython和pycocotool，也可以安装mmcv进行数据处理。

        ```
        pip install Cython

        pip install pycocotools

        pip install mmcv==0.2.14
        ```
        在`config.py`中更改COCO_ROOT和其他您需要的设置。目录结构如下：


        ```
        .
        └─cocodataset
          ├─annotations
            ├─instance_train2017.json
            └─instance_val2017.json
          ├─val2017
          └─train2017
    
        ```

    2. 若使用自己的数据集，**执行脚本时选择数据集为other。**
        将数据集信息整理成TXT文件，每行内容如下：

        ```
        train2017/0000001.jpg 0,259,401,459,7 35,28,324,201,2 0,30,59,80,2
        ```

        每行是按空间分割的图像标注，第一列是图像的相对路径，其余为[xmin,ymin,xmax,ymax,class]格式的框和类信息。从`IMAGE_DIR`（数据集目录）图像路径以及`ANNO_PATH`（TXT文件路径）的相对路径中读取图像。`IMAGE_DIR`和`ANNO_PATH`可在`config.py`中设置。

# 快速入门

通过官方网站安装MindSpore后，您可以按照如下步骤进行训练和评估： 

注意：1. 第一次运行生成MindRecord文件，耗时较长。
      2. 预训练模型是在ImageNet2012上训练的ResNet-50检查点。
      3. VALIDATION_JSON_FILE为标签文件。CHECKPOINT_PATH是训练后的检查点文件。

```
# 单机训练
sh run_standalone_train_ascend.sh [PRETRAINED_MODEL]

# 分布式训练
sh run_distribute_train_ascend.sh [RANK_TABLE_FILE] [PRETRAINED_MODEL]

# 评估
sh run_eval_ascend.sh [VALIDATION_JSON_FILE] [CHECKPOINT_PATH]
```

# 脚本说明

## 脚本及样例代码

```shell
.
└─faster_rcnn      
  ├─README.md    // Faster R-CNN相关说明
  ├─scripts
    ├─run_standalone_train_ascend.sh    // Ascend单机shell脚本
    ├─run_distribute_train_ascend.sh    // Ascend分布式shell脚本
    └─run_eval_ascend.sh    // Ascend评估shell脚本
  ├─src
    ├─FasterRcnn
      ├─__init__.py    // init文件
      ├─anchor_generator.py    // 锚点生成器
      ├─bbox_assign_sample.py    // 第一阶段采样器
      ├─bbox_assign_sample_stage2.py    // 第二阶段采样器
      ├─faster_rcnn_r50.py    // Faster R-CNN网络
      ├─fpn_neck.py    // 特征金字塔网络
      ├─proposal_generator.py    // 候选生成器
      ├─rcnn.py    // R-CNN网络
      ├─resnet50.py    // 骨干网络
      ├─roi_align.py    // ROI对齐网络
      └─rpn.py    //  区域候选网络
    ├─config.py    // 总配置
    ├─dataset.py    // 创建并处理数据集
    ├─lr_schedule.py    // 学习率生成器
    ├─network_define.py    // Faster R-CNN网络定义
    └─util.py    // 例行操作
  ├─eval.py    // 评估脚本
  └─train.py    // 训练脚本
```

## 训练过程

### 用法

```
# Ascend单机训练
sh run_standalone_train_ascend.sh [PRETRAINED_MODEL]

# Ascend分布式训练
sh run_distribute_train_ascend.sh [RANK_TABLE_FILE] [PRETRAINED_MODEL]
```
 
> 运行分布式任务时需要用到RANK_TABLE_FILE指定的rank_table.json。您可以使用[hccl_tools](https://gitee.com/mindspore/mindspore/tree/master/model_zoo/utils/hccl_tools)生成该文件。
> PRETRAINED_MODEL应该是在ImageNet 2012上训练的ResNet-50检查点。现成的pretrained_models目前不可用。敬请期待。
> config.py中包含原数据集路径，可以选择“coco_root”或“image_dir”。

### 结果

训练结果保存在示例路径中，文件夹名称以“train”或“train_parallel”开头。您可以在loss_rankid.log中找到检查点文件以及结果，如下所示。


```
# 分布式训练结果（8P）
epoch: 1 step: 7393, rpn_loss: 0.12054, rcnn_loss: 0.40601, rpn_cls_loss: 0.04025, rpn_reg_loss: 0.08032, rcnn_cls_loss: 0.25854, rcnn_reg_loss: 0.14746, total_loss: 0.52655
epoch: 2 step: 7393, rpn_loss: 0.06561, rcnn_loss: 0.50293, rpn_cls_loss: 0.02587, rpn_reg_loss: 0.03967, rcnn_cls_loss: 0.35669, rcnn_reg_loss: 0.14624, total_loss: 0.56854
epoch: 3 step: 7393, rpn_loss: 0.06940, rcnn_loss: 0.49658, rpn_cls_loss: 0.03769, rpn_reg_loss: 0.03165, rcnn_cls_loss: 0.36353, rcnn_reg_loss: 0.13318, total_loss: 0.56598
...
epoch: 10 step: 7393, rpn_loss: 0.03555, rcnn_loss: 0.32666, rpn_cls_loss: 0.00697, rpn_reg_loss: 0.02859, rcnn_cls_loss: 0.16125, rcnn_reg_loss: 0.16541, total_loss: 0.36221
epoch: 11 step: 7393, rpn_loss: 0.19849, rcnn_loss: 0.47827, rpn_cls_loss: 0.11639, rpn_reg_loss: 0.08209, rcnn_cls_loss: 0.29712, rcnn_reg_loss: 0.18115, total_loss: 0.67676
epoch: 12 step: 7393, rpn_loss: 0.00691, rcnn_loss: 0.10168, rpn_cls_loss: 0.00529, rpn_reg_loss: 0.00162, rcnn_cls_loss: 0.05426, rcnn_reg_loss: 0.04745, total_loss: 0.10859
```

## 评估过程

### 用法

```
# Ascend评估
sh run_eval_ascend.sh [VALIDATION_JSON_FILE] [CHECKPOINT_PATH]
```

> 在训练过程中生成检查点。

### 结果

评估结果将保存在示例路径中，文件夹名为“eval”。在此文件夹下，您可以在日志中找到类似以下的结果。

```
 Average Precision  (AP) @[ IoU=0.50:0.95 | area=   all | maxDets=100 ] = 0.360
 Average Precision  (AP) @[ IoU=0.50      | area=   all | maxDets=100 ] = 0.586
 Average Precision  (AP) @[ IoU=0.75      | area=   all | maxDets=100 ] = 0.385
 Average Precision  (AP) @[ IoU=0.50:0.95 | area= small | maxDets=100 ] = 0.229
 Average Precision  (AP) @[ IoU=0.50:0.95 | area=medium | maxDets=100 ] = 0.402
 Average Precision  (AP) @[ IoU=0.50:0.95 | area= large | maxDets=100 ] = 0.441
 Average Recall     (AR) @[ IoU=0.50:0.95 | area=   all | maxDets=  1 ] = 0.299
 Average Recall     (AR) @[ IoU=0.50:0.95 | area=   all | maxDets= 10 ] = 0.487
 Average Recall     (AR) @[ IoU=0.50:0.95 | area=   all | maxDets=100 ] = 0.515
 Average Recall     (AR) @[ IoU=0.50:0.95 | area= small | maxDets=100 ] = 0.346
 Average Recall     (AR) @[ IoU=0.50:0.95 | area=medium | maxDets=100 ] = 0.562
 Average Recall     (AR) @[ IoU=0.50:0.95 | area= large | maxDets=100 ] = 0.631
```

# 模型描述

## 性能

### 训练性能

| 参数 | Faster R-CNN |
| -------------------------- | ----------------------------------------------------------- |
| 模型版本 | V1 |
| 资源 | Ascend 910；CPU 2.60GHz，192核；内存：755G |
| 上传日期 | 2020/8/31 |
| MindSpore版本 | 1.0.0 |
| 数据集 | COCO 2017 |
| 训练参数 | epoch=12, batch_size=2 |
| 优化器 | SGD |
| 损失函数 | Softmax交叉熵，Sigmoid交叉熵，SmoothL1Loss |
| 速度 | 1卡：190毫秒/步；8卡：200毫秒/步 |
| 总时间 | 1卡：37.17小时；8卡：4.89小时 |
| 参数(M) | 250 |
| 脚本 | [Faster R-CNN脚本](https://gitee.com/mindspore/mindspore/tree/r1.0/model_zoo/office/cv/faster_rcnn) |

### 评估性能

| 参数 | Faster R-CNN |
| ------------------- | --------------------------- |
| 模型版本 | V1 |
| 资源 | Ascend 910 |
| 上传日期 | 2020/8/31 |
| MindSpore版本 | 1.0.0 |
| 数据集 | COCO2017 |
| batch_size | 2 |
| 输出 | mAP |
| 准确率 | IoU=0.50：57.6%  |
| 推理模型 | 250M（.ckpt文件） |

# ModelZoo主页
 请浏览官网[主页](https://gitee.com/mindspore/mindspore/tree/master/model_zoo)。
