# ESP32遠端環境監控開發
透過ESP32、Node-RED及MQTT來完成遠端控制及觀測的裝置。

# 列表
- [架構圖](https://github.com/sheng411/mqtt_esp32?tab=readme-ov-file#%E6%9E%B6%E6%A7%8B%E5%9C%96)
- [目的](https://github.com/sheng411/mqtt_esp32?tab=readme-ov-file#%E7%9B%AE%E7%9A%84)
- [實際展示](https://github.com/sheng411/mqtt_esp32?tab=readme-ov-file#%E5%AF%A6%E9%9A%9B%E5%B1%95%E7%A4%BA)
  - [主介面](https://github.com/sheng411/mqtt_esp32?tab=readme-ov-file#-%E4%B8%BB%E4%BB%8B%E9%9D%A2)
  - [特定區域](https://github.com/sheng411/mqtt_esp32?tab=readme-ov-file#-%E7%89%B9%E5%AE%9A%E5%8D%80%E5%9F%9F)
  - [遠端控制](https://github.com/sheng411/mqtt_esp32?tab=readme-ov-file#-%E9%81%A0%E7%AB%AF%E6%8E%A7%E5%88%B6)


# 架構圖
![image](https://github.com/user-attachments/assets/148020fe-ff16-4d4a-92aa-fb0dc3d87b79)


# 目的
將ESP32端所觀測到的資料透過MQTT的方式上傳到MQTT Server上，並透過Nord-RED把資料以可視化的方式顯示，讓使用者更直觀的了解該端狀況。

# 實際展示

Node-RED設計架構
![image](https://github.com/user-attachments/assets/f3b2dd8b-3e2c-4205-a5ff-72426c8feae8)
![image](https://github.com/user-attachments/assets/d67ae399-0238-4a02-8013-b8aa2226a380)

  
在Node-RED中設定使用LINE Notify自動推播至LINE上供使用者更方便閱覽
![S__29392903](https://github.com/user-attachments/assets/e67cf4a6-20c8-4b7d-a9a6-1392c451c4b2)

  
### -主介面 
可以查看所有裝置的運行狀況及近10分鐘的資訊
![image](https://github.com/user-attachments/assets/192a671b-faba-473b-9dc4-09130f5d4f52)


### -特定區域  
可從此區域詳細查看該端點運行狀況(1小時及1天的資訊)
![image](https://github.com/user-attachments/assets/9b7218c5-248d-40d7-9a75-1bc94762c45e)

### -遠端控制  
可透過電腦 / 手機進行遠端控制(暫時以LED燈做為展示)
**開啟**
>![image](https://github.com/user-attachments/assets/25272be0-1b10-47dd-80e7-b2fd00d60d9c)
>![S__29392898_0](https://github.com/user-attachments/assets/14028672-33d1-469c-aa5e-d4676343ff68)
  
**關閉**
>![image](https://github.com/user-attachments/assets/fab26997-e823-4785-9d02-b856e1a0d090)
>![S__29392900_0](https://github.com/user-attachments/assets/6ddb6a94-7d3e-4634-a61b-90d5005904ea)
