# Tetris

# 사양
- Windows 10 버전 1803(10.0.17134.0) 이상 
  - '28f56b9ba3d52034bf0f7f2ec396202e159b1533' 커밋에 추가된 'setlocale(LC_ALL, ".UTF8")'의 ['UTF-8'을 해당 버전 이후 부터 지원](https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=msvc-170#utf-8-support)
        


## 개발

### Log
#### 파일
    - Log.h
    - Log.cpp 
#### 클래스
    - Logger 
#### 기능
    - Log 쓰기
    - Log 가져오기
#### 의도  
    - 개발중 Log기능을 사용하기 위한 기능
    - 버퍼에 문자열을 남기고, 버퍼 내용을 가져오는 단순한 기능 -> 로그를 남기는 로직과, 로그를 보여주는 기능의 분리를 위해서 이렇게 작성함
    - 문자열에 대한 심화 학습을 위해 string을 사용하지 않기로 함
   
#### 고민했던 점
##### Log(format, args) 형태로 사용자에게 기능을 제공하고 싶었다
      - 해당 기능을 구현하기 위해, 'c++ 가변 인수'와 'define'을 적극 활용하였다
##### Buffer 크기 관리
      - 배열의 크기 관리를 모자란 크기만큼 확장 시키는 것이 아니라, 덩어리로 확장 하도록 함
      - 잦은 메모리 복사, 메모리 할당은 속도가 느리기 때문에 이러한 방식을 사용하기로 결정
##### 문자열 붙이기
- 'strcat'을 사용하는 방식을 생각했었으나, 임시 버퍼가 생기는 것이 마음에 들지 않아, 포인터를 활용하여 버퍼에 추가하는 방식으로 구현
- TODO : strcat의 성능을 파악하고 비교 해볼 것, strcat을 사용하는 방식으로 구현 했을때와 속도 비교
``` c++
TCHAR* catStart = mLogBuffer + mLogBufferUsedSize;
_vstprintf_s(catStart, mLogBufferSize - mLogBufferUsedSize, format, args); // NULL이 추가되는 부분
```
