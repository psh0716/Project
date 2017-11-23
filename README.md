# 영상처리를 이용한 보행자 검출 시스템

- Haar-like feature 를 이용한 검출과 HOG 알고리즘을 이용한 검출을 통해 보행자를 검출하는 시스템

# File 설명

- Project 폴더 : 프로젝트 코드 및 실행파일

- 프로젝트.pdf : 프로젝트 설명

# 실행 방법

- Project 폴더 안의 Project.exe 를 실행하여 test.avi 파일에 대한 검출 시작

(단, haarcascade_fullbody.xml, hogcascade_pedestrians.xml 파일과 같은 폴더에 위치해야함)

# 검출함수 세부조정

- Project 폴더 안의 main.cpp 파일을 수정하여 검출성능 조정가능

- min_obj_sz( min_x, min_y ) : min_x, min_y 값을 수정하여 검출할 물체의 최소크기 설정

- max_obj_sz( max_x, max_y ) : max_x, max_y 값을 수정하여 검출할 물체의 최대크기 설정

- (haar/hog).detectMultiScale( image, found, scale, candidate, 0, min_obj_sz, max_obj_sz ) : 검출함수

image : 검출할 영역

found : 검출결과들을 저장할 <Rect>형 벡터

scale : image를 얼마나 downscale할 것인지에 대한 변수. 1.00 값 이상 설정

candidate : 검출결과 선정에 얼마나 많은 검출후보영역이 필요한지에 대한 변수. 값이 클수록 정확도는 높아지지만 그만큼 검출율 은낮아짐

# 시연 영상

- https://youtu.be/YYVQw0Iqv-g
