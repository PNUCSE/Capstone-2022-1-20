### 1. 프로젝트 소개
```
시각장애인을 위한  새로운 길찾기 안내 시스템.

RFID를 사용하여 기존의 시각 보조시설 들이 지니는 단점을 보완하면서,
적은비용으로 지속적인 유지보수가 가능한 안내 시스템을 만들어보자.
Arduino와 RFID, Bluetooth를 이용하여 사용자가 주변 정보를 자연스럽게 습득 가능한 안내 시스템.
```

### 2. 팀 소개
```
정해원,          speedheawon@pusan.ac.kr,       개발총괄
졸디쿠로바 다나,  dana_joldykulova@pusan.ac.kr,  서버개발
장대익,          jbi7336@pusan.ac.kr,           서버배포 및 아두이노 개발
```

### 3. 시스템 구성도
![시스템 구상도](https://user-images.githubusercontent.com/35110792/195494080-891fd16f-d3c3-4c89-9dc6-10bc4d00f8dd.png)

### 4. 소개 영상
[![졸업과제 20조 소개영상](http://img.youtube.com/vi/_BsLkW4PqcM/0.jpg)](https://youtu.be/_BsLkW4PqcM)

### 5. Python requirement
```
python==3.6.9
certifi==2021.5.30
click==8.0.4
colorama==0.4.5
dataclasses==0.8
Flask==2.0.3
Flask-SQLAlchemy==2.5.1
greenlet==1.1.3
importlib-metadata==4.8.3
itsdangerous==2.0.1
Jinja2==3.0.3
MarkupSafe==2.0.1
SQLAlchemy==1.4.41
typing_extensions==4.1.1
Werkzeug==2.0.3
wincertstore==0.2
zipp==3.6.0
```

### 6. 설치 및 사용 방법
```
본 프로젝트는 Arduino Uno와 그에 맞는 규격의 Shield 형식의 RFID 제품을 사용하여 제작되었습니다.
또한 서버로 사용된 AWS는 Ubuntu 18.04 버전을 사용하였습니다.
```
#### 6.1 서버 구동

```
서버구동에 필요한 라이브러리는
'pip install -r requirements.txt' 를 통해 한번에 설치하실 수 있습니다.

'python ./apps.py' 를 통해 서버를 실행한 뒤
웹 서버에 접속하여, RFID를 등록, 삭제, 수정 작업을 진행 할 수 있습니다.
또한 POST, GET 메소를 통해 JSON 형식의 파일을 주고 받을 수 있습니다.

서버가 실행 되었다면, apps.py 에서 기입한 호스트 주소로 접속하여 RFID를 기입하시면 됩니다.
```
#### 6.2 어플리케이션
```
Unity 로 제작되었으며, 최신 버젼의 Unity 를 사용하여 올려놓은 프로젝트를 불러와 빌드하시면 됩니다.
```
#### 6.3 아두이노
```
Arduino Uno와 RFID Reader (PN532, sparkFUN-M6E Nano) 를 사용하여 결합한 뒤,
Arduino 폴더의 ino 파일을 아두이노에 설치 한 뒤
블루투스와 핸드폰이 정상적으로 연결이 되는지 어플리케이션을 통해 확인합니다.
```
#### 7. 주의사항
```
sparkFUN-M6E Nano 제품을 사용할 경우 유의미한 인식거리를 얻을 수 있으나,
아두이노 발열이 심해 짧은 시간안에 쓰로틀링이 걸리게 되는 온도까지 도달 할 수 있습니다.
또한 M6E Nano 제품의 경우 EPC Gen2 UHF RFID Tag 만을 인식합니다.
```